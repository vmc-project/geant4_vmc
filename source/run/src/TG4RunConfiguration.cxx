// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4RunConfiguration.cxx
/// \brief Implementation of the TG4RunConfiguration class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunConfiguration.h"
#include "TG4DetConstruction.h"
#include "TG4ComposedPhysicsList.h"
#include "TG4EmPhysicsList.h"
#include "TG4HadronPhysicsList.h"
#include "TG4OpticalPhysicsList.h"
#include "TG4SpecialPhysicsList.h"
#include "TG4PrimaryGeneratorAction.h"
#include "TG4RunAction.h"
#include "TG4EventAction.h"
#include "TG4TrackingAction.h"
#include "TG4SteppingAction.h"
#include "TG4SpecialStackingAction.h"
#include "TG4LVTree.h"
#include "TG4Globals.h"

#include <G4UImessenger.hh>

#ifdef USE_VGM
#include "TG4VGMMessenger.h"
#include <XmlVGM/AGDDExporter.h>
#include <XmlVGM/GDMLExporter.h>
#endif

//_____________________________________________________________________________
TG4RunConfiguration::TG4RunConfiguration(const TString& userGeometry,
                                         const TString& physicsList,
                                         const TString& specialProcess,
                                         Bool_t specialStacking)
  : fUserGeometry(userGeometry),
    fPhysicsListSelection(physicsList),
    fSpecialProcessSelection(specialProcess),
    fSpecialStacking(specialStacking),
    fAGDDMessenger(0),
    fGDMLMessenger(0)
    
{
/// Standard constructor

  if ( userGeometry != "geomVMCtoGeant4"  &&
       userGeometry != "geomVMCtoRoot"    &&
       userGeometry != "geomRoot"         &&
       userGeometry != "geomRootToGeant4" &&
       userGeometry != "geomGeant4" ) {
       
    TG4Globals::Exception(
      "TG4RunConfiguration", "TG4RunConfiguration",
      "User geometry " + userGeometry + " not recognized." 
         + TG4Globals::Endl() +
      "Available options:"                                 
         + TG4Globals::Endl() +
      "geomVMCtoGeant4 geomVMCtoRoot geomRoot geomRootToGeant4 geomGeant4");
  }
   

  G4int itoken = 0;
  G4String token;
  do {
    token = TG4Globals::GetToken(itoken++, physicsList);
    
    if ( ! TG4EmPhysicsList::IsAvailableSelection(token) &&
         ! TG4HadronPhysicsList::IsAvailableSelection(token) &&
         token != "optical" ) {
         
      TG4Globals::Exception(
        "TG4RunConfiguration", "TG4RunConfiguration",
        "Physics list selection " + physicsList + " not recognized." 
          + TG4Globals::Endl() +
        "Available options:"                                         
          + TG4Globals::Endl() +
        "EM, EM+optical, Hadron, Hadron+optical"           
          + TG4Globals::Endl() +
        "  where EM     = " + TString(TG4EmPhysicsList::AvailableSelections())
          + TG4Globals::Endl() +
        "        Hadron = " + TString(TG4HadronPhysicsList::AvailableSelections())
          + TG4Globals::Endl() +
        "  The EM selections are cumulative, while Hadron selections are exlusive."
          + TG4Globals::Endl());
    }
  }  
  while ( token != "");         

  if ( ! TG4SpecialPhysicsList::IsAvailableSelection(specialProcess.Data()) ) {

     TG4Globals::Exception(
      "TG4RunConfiguration", "TG4RunConfiguration",
      "Special process selection " + specialProcess + " is not recognized." 
        + TG4Globals::Endl() +
      "Available options:"                                               
        + TG4Globals::Endl() +
      TString(TG4SpecialPhysicsList::AvailableSelections())); 
  } 

  // instantiate LVtree browser
  TG4LVTree::Instance();

#ifdef USE_VGM
  // instantiate XML messengers
  fAGDDMessenger = new TG4VGMMessenger("AGDD");
  fGDMLMessenger = new TG4VGMMessenger("GDML");
#endif    
}

//_____________________________________________________________________________
TG4RunConfiguration::~TG4RunConfiguration()
{
/// Destructor

  delete TG4LVTree::Instance();
  delete fAGDDMessenger;
  delete fGDMLMessenger;
}

//
// public methods
//

//_____________________________________________________________________________
G4VUserDetectorConstruction* TG4RunConfiguration::CreateDetectorConstruction()
{
/// Create and return Geant4 VMC default detector construction

  if ( fUserGeometry == "Root" ) return 0;
   
  return new TG4DetConstruction();
}  


//_____________________________________________________________________________
G4VUserPhysicsList* TG4RunConfiguration::CreatePhysicsList()
{
/// Create default Geant4 VMC physics list

  TG4ComposedPhysicsList* builder = new TG4ComposedPhysicsList();
  
  // Decompose physics list selection
  G4int itoken = 0;
  G4String emSelection;
  G4String hadronSelection;
  G4String opticalSelection;
  G4String token;
  do {
    token = TG4Globals::GetToken(itoken++, fPhysicsListSelection);

    if ( token.contains("em") ) {
      emSelection += token;
      emSelection += " ";
    }
    else if ( token == "optical" ) {
      opticalSelection = token;
    }
    else if ( token != "" ) {
      hadronSelection = token;
    }
  }    
  while ( token != "");         
    
  if ( emSelection != "" && hadronSelection != "" ) {
    TG4Globals::Exception(
      "TG4RunConfiguration", "TG4RunConfiguration",
      "Physics list selection " + fPhysicsListSelection + " is not valid." 
        + TG4Globals::Endl() +
      "The EM selections cannot be combined with Hadron selections."
        + TG4Globals::Endl());
  }     

  if ( emSelection != "" ) {
    G4cout << "Adding EMPhysicsList " << emSelection << G4endl;
    builder->AddPhysicsList(new TG4EmPhysicsList(emSelection));
  }  
  
  if ( hadronSelection != "" ) {
    G4cout << "Adding HadronPhysicsList " << hadronSelection << G4endl;
    builder->AddPhysicsList(new TG4HadronPhysicsList(hadronSelection));
  }  
    
  if ( opticalSelection != "" ) {
    G4cout << "Adding OpticalPhysicsList " << G4endl;
    builder->AddPhysicsList(new TG4OpticalPhysicsList());
  }  
    
  // add option here
  G4cout << "Adding SpecialPhysicsList " << fSpecialProcessSelection.Data() << G4endl;
  builder->AddPhysicsList(new TG4SpecialPhysicsList(
                                 fSpecialProcessSelection.Data()));
  
  return builder;  
}  

//_____________________________________________________________________________
G4VUserPrimaryGeneratorAction* TG4RunConfiguration::CreatePrimaryGenerator()
{
/// Create and return Geant4 VMC default primary generator

  return new TG4PrimaryGeneratorAction();
}  

//_____________________________________________________________________________
G4UserRunAction* TG4RunConfiguration::CreateRunAction()
{
/// Create and return Geant4 VMC default run action

  return new TG4RunAction();
}  
 
//_____________________________________________________________________________
G4UserEventAction*  TG4RunConfiguration::CreateEventAction()
{
/// Create and return Geant4 VMC default event action

  return new TG4EventAction();
}  

//_____________________________________________________________________________
TG4TrackingAction*  TG4RunConfiguration::CreateTrackingAction()
{
/// Create and return Geant4 VMC default tracking action

  return new TG4TrackingAction();
}  

//_____________________________________________________________________________
TG4SteppingAction*  TG4RunConfiguration::CreateSteppingAction()
{
/// Create and return Geant4 VMC default stepping action

  return new TG4SteppingAction();
}  

//_____________________________________________________________________________
G4UserStackingAction* TG4RunConfiguration::CreateStackingAction()
{
/// Create and return Geant4 VMC special stacking action

  if ( fSpecialStacking )
    return new TG4SpecialStackingAction();
    
  return 0;
}    

//_____________________________________________________________________________
TString TG4RunConfiguration::GetUserGeometry() const
{
/// Return the way user geometry is built

  // strip out "geom"from the name
  TString userGeometry = fUserGeometry;
  userGeometry = userGeometry.Remove(0, 4);

  return userGeometry;
}  

//_____________________________________________________________________________
Bool_t TG4RunConfiguration::IsSpecialStacking() const
{
/// Return true if special stacking is activated

  return fSpecialStacking;
}  

