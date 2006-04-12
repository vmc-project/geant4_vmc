// $Id: Ex03RunConfiguration.cxx,v 1.2 2006/01/13 16:58:58 brun Exp $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration
// --------------------------
// See the class description in the header file.

#include "Ex03RunConfiguration.h"
//#include "ExN03PrimaryGeneratorAction.hh"
//#include "ExN03DetectorConstruction.hh"

#include <LHEP_BERT.hh>

//_____________________________________________________________________________
Ex03RunConfiguration::Ex03RunConfiguration()
  : TG4RunConfiguration(false) {
//
}

//_____________________________________________________________________________
Ex03RunConfiguration::~Ex03RunConfiguration(){
//
}

//
// protected methods
//

//_____________________________________________________________________________
G4VUserPhysicsList*  Ex03RunConfiguration::CreatePhysicsList()
{
// Create LHEP_BERT physics list

  return new LHEP_BERT();
}  

/*
//_____________________________________________________________________________
G4VUserDetectorConstruction*  Ex03RunConfiguration::CreateDetectorConstruction()
{
// Create detector construction

  return new ExN03DetectorConstruction();
}   

//_____________________________________________________________________________
G4VUserPrimaryGeneratorAction* Ex03RunConfiguration::CreatePrimaryGenerator()
{
// Create primary generator

  return new ExN03PrimaryGeneratorAction();
}  
*/
