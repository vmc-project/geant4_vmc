// $Id: TG4RunConfiguration.cxx,v 1.3 2004/11/10 11:39:28 brun Exp $
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
#include "TG4Globals.h"

//_____________________________________________________________________________
TG4RunConfiguration::TG4RunConfiguration(Bool_t specialStacking)
  : fSpecialStacking(specialStacking),
    fPhysicsList(0),
    fPhysicsListOptions()
    
{
//
}

//_____________________________________________________________________________
TG4RunConfiguration::TG4RunConfiguration(const TG4RunConfiguration& right)
{
//
  TG4Globals::Exception("TG4RunConfiguration is protected from copying.");
}

//_____________________________________________________________________________
TG4RunConfiguration::~TG4RunConfiguration(){
//
}

//
// operators
//

//_____________________________________________________________________________
TG4RunConfiguration& TG4RunConfiguration::operator=(
                                const TG4RunConfiguration& right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception("TG4RunConfiguration is protected from assigning.");

  return *this;  
}    
          
//
// public methods
//

//_____________________________________________________________________________
G4VUserDetectorConstruction* TG4RunConfiguration::CreateDetectorConstruction()
{
/// Create and return Geant4 VMC default detector construction

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
    G4String text = "TG4RunConfiguration::SetPhysicsListOptions: \n";
    text = text + "     Physics list already constructed.";  
    text = text + "     Cannot set options for its building.";  
    TG4Globals::Exception(text);
  }

  fPhysicsListOptions = options;
}     			      

//_____________________________________________________________________________
Bool_t TG4RunConfiguration::IsSpecialStacking() const
{
/// Return true if special stacking is activated

  return fSpecialStacking;
}  

//_____________________________________________________________________________
G4VUserPhysicsList* TG4RunConfiguration::GetPhysicsList() const
{
/// Return its physics list.
  
  return fPhysicsList;
}
