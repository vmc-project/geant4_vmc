// $Id: Ex03RunConfiguration2.cxx,v 1.3 2006/04/12 10:39:33 brun Exp $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration2
// ---------------------------
// See the class description in the header file.

#include "Ex03RunConfiguration2.h"
//#include "ExN03PrimaryGeneratorAction.hh"
//#include "ExN03DetectorConstruction.hh"

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

  return new LHEP_BERT();
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
