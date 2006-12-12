// $Id: TG4RunConfiguration.cxx,v 1.6 2006/07/28 06:09:36 brun Exp $
// Category: run
//
// Class TG4RunConfiguration
// --------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4RunConfiguration.h"
#include "TG4DetConstruction.h"
#include "TG4ModularPhysicsList.h"
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
                                         Bool_t specialStacking)
  : fUserGeometry(userGeometry),
    fSpecialStacking(specialStacking),
    fPhysicsList(0),
    fPhysicsListOptions(),
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
      "User geometry " + userGeometry + " not recognized." + TG4Globals::Endl() +
      "Available options:"                                 + TG4Globals::Endl() +
      "geomVMCtoGeant4 geomVMCtoRoot geomRoot geomRootToGeant4 geomGeant4");
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
TG4RunConfiguration::~TG4RunConfiguration(){
//
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

  fPhysicsList = new TG4ModularPhysicsList(fPhysicsListOptions);
  return fPhysicsList;
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
void TG4RunConfiguration::SetPhysicsListOptions(
                              const TG4PhysicsListOptions& options)
{
/// Set physics list options

  if ( fPhysicsList ) {
    TG4Globals::Exception(
      "TG4RunConfiguration", "SetPhysicsListOptions",
      "Physics list is already constructed." + TG4Globals::Endl() +
      "Cannot set options for its building.");  
  }

  fPhysicsListOptions = options;
}                                   

//_____________________________________________________________________________
G4VUserPhysicsList* TG4RunConfiguration::GetPhysicsList() const
{
/// Return its physics list.
  
  return fPhysicsList;
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

