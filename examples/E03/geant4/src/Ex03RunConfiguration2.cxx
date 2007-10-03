// $Id: Ex03RunConfiguration2.cxx,v 1.2 2007/08/09 13:43:33 brun Exp $
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
Ex03RunConfiguration2::Ex03RunConfiguration2(const TString& userGeometry,
                                             const TString& specialProcess)
  : TG4RunConfiguration(userGeometry, "emStandard", specialProcess) {
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
// Override the default physics list with user defined physics list;
// LHEP_BERT physics list should be replaced with user own physics list

  TG4ComposedPhysicsList* builder = new TG4ComposedPhysicsList();
  
  // User physics list
  G4cout << "Adding user physics list " << G4endl;
  builder->AddPhysicsList(new LHEP_BERT());
    
  G4cout << "Adding SpecialPhysicsList " << G4endl;
  builder->AddPhysicsList(new TG4SpecialPhysicsList(
                                 fSpecialProcessSelection.Data()));
  
  return builder;  
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
