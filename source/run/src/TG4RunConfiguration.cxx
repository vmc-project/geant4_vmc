// $Id: TG4RunConfiguration.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: run
//
// Class TG4RunConfiguration
// --------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4RunConfiguration.h"
#include "TG4DetConstruction.h"
#include "TG4SDConstruction.h"
#include "TG4ModularPhysicsList.h"
#include "TG4PrimaryGeneratorAction.h"
#include "TG4RunAction.h"
#include "TG4EventAction.h"
#include "TG4TrackingAction.h"
#include "TG4SteppingAction.h"
#include "TG4SpecialStackingAction.h"
#include "TG4Globals.h"

#include <G4RunManager.hh>

//_____________________________________________________________________________
TG4RunConfiguration::TG4RunConfiguration(Bool_t specialStacking)
  : TG4VRunConfiguration(),
    fSpecialStacking(specialStacking) {
//

  CreateUserConfiguration();
}

//_____________________________________________________________________________
TG4RunConfiguration::~TG4RunConfiguration(){
//
}

//
// protected methods
//


//_____________________________________________________________________________
void TG4RunConfiguration::CreateUserConfiguration()
{
/// Create the mandatory Geant4 classes and 
/// the other user action classes. 

  // create mandatory Geant4 classes
  fDetectorConstruction = new TG4DetConstruction();
  fSDConstruction = new TG4SDConstruction();
  fPhysicsList = new TG4ModularPhysicsList();
  fPrimaryGenerator = new TG4PrimaryGeneratorAction();

  // create the other user action classes
  fRunAction  = new TG4RunAction();
  fEventAction  = new TG4EventAction();
  fTrackingAction = new TG4TrackingAction();
  fSteppingAction = new TG4SteppingAction();

  // conditionally set special stacking action
  if (fSpecialStacking)
    fStackingAction = new TG4SpecialStackingAction();
}
