//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03G4DetectorConstruction.cc
/// \brief Implementation of the Ex03G4DetectorConstruction class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex03G4DetectorConstruction.h"

#include "TG4GeometryManager.h"

#include <G4Mag_UsualEqRhs.hh>
#include "G4NystromRK4.hh"

//_____________________________________________________________________________
Ex03G4DetectorConstruction::Ex03G4DetectorConstruction()
 : TG4DetConstruction()
{}

//_____________________________________________________________________________
Ex03G4DetectorConstruction::~Ex03G4DetectorConstruction()
{ 
}

//_____________________________________________________________________________
void Ex03G4DetectorConstruction::ConstructSDandField()
{ 
/// Create user defined objects and set them to TG4GeometryManager;
/// then call the base class function and return the Geant4 VMC detector construction

  // Create "user" equation of motion;
  // G4Mag_UsualEqRhs eqution and G4NystromRK4 stepper should be replaced with users 
  // own objects
  G4cout << "Create user defined equation of motion and stepper" << G4endl; 
  G4Mag_EqRhs* equation = new G4Mag_UsualEqRhs(0);
  G4MagIntegratorStepper* stepper = new G4NystromRK4(equation);

  TG4GeometryManager::Instance()->SetUserEquationOfMotion(equation);
  TG4GeometryManager::Instance()->SetUserStepper(stepper);

  // Call method defined in the base class
  TG4DetConstruction::ConstructSDandField();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
