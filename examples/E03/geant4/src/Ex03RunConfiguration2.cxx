// $Id: Ex03RunConfiguration2.cxx,v 1.1 2006/12/12 16:21:38 brun Exp $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration2
// ---------------------------
// See the class description in the header file.

#include "Ex03RunConfiguration2.h"
//#include "ExN03PrimaryGeneratorAction.hh"
//#include "ExN03DetectorConstruction.hh"

#include "TG4ComposedPhysicsList.h"
#include "TG4SpecialPhysicsList.h"
#include <LHEP_BERT.hh>

//_____________________________________________________________________________
Ex03RunConfiguration2::Ex03RunConfiguration2(const TString& userGeometry)
  : TG4RunConfiguration(userGeometry) {
//
}

//_____________________________________________________________________________
Ex03RunConfiguration2::~Ex03RunConfiguration2(){
//
}

//
// protected methods
//


//_____________________________________________________________________________
G4VUserPhysicsList*  Ex03RunConfiguration2::CreatePhysicsList()
{
// Create LHEP_BERT physics list

  TG4ComposedPhysicsList* physicsList = new TG4ComposedPhysicsList();
  
  // physics list from G4
  physicsList->AddPhysicsList(new LHEP_BERT());
  
  // special processes from Geant4 VMC
  TG4PhysicsListOptions options;
  options.SetSpecialCutsPhysics(true);
  fSpecialPhysicsList = new TG4SpecialPhysicsList(options);
  physicsList->AddPhysicsList(fSpecialPhysicsList);

  return physicsList;
}  


/*
//_____________________________________________________________________________
G4VUserDetectorConstruction*  Ex03RunConfiguration2::CreateDetectorConstruction()
{
// Create detector construction

  return new ExN03DetectorConstruction();
}   


//_____________________________________________________________________________
G4VUserPrimaryGeneratorAction* Ex03RunConfiguration2::CreatePrimaryGenerator()
{
// Create primary generator

  return new ExN03PrimaryGeneratorAction();
}  
*/
