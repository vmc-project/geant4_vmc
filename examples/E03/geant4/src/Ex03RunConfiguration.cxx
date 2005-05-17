// $Id: $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration
// --------------------------
// See the class description in the header file.

#include "Ex03RunConfiguration.h"

#include "TG4DetConstruction.h"
#include "TG4SDConstruction.h"
#include "TG4PrimaryGeneratorAction.h"
#include "TG4RunAction.h"
#include "TG4EventAction.h"
#include "TG4TrackingAction.h"
#include "TG4SteppingAction.h"

#include <LHEP_BERT.hh>

//_____________________________________________________________________________
Ex03RunConfiguration::Ex03RunConfiguration()
  : TG4VRunConfiguration() {
//

  CreateUserConfiguration();
}

//_____________________________________________________________________________
Ex03RunConfiguration::~Ex03RunConfiguration(){
//
}

//
// protected methods
//


//_____________________________________________________________________________
void Ex03RunConfiguration::CreateUserConfiguration()
{
// Creates the mandatory Geant4 classes and 
// the other user action classes. 
// ---

  // create mandatory Geant4 classes
  fDetectorConstruction = new TG4DetConstruction();
  fSDConstruction = new TG4SDConstruction();
  fPhysicsList = new LHEP_BERT();
  fPrimaryGenerator = new TG4PrimaryGeneratorAction();

  // create the other user action classes
  fRunAction  = new TG4RunAction();
  fEventAction  = new TG4EventAction();
  fTrackingAction = new TG4TrackingAction();
  fSteppingAction = new TG4SteppingAction();
}

//_____________________________________________________________________________
G4bool Ex03RunConfiguration::IsSpecialStacking() const
{
// Returns false as no special stacking mechanism is used.
// ---

  return false;
}  
