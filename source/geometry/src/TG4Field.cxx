//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4Field.cxx
/// \brief Implementation of the TG4Field class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Field.h"
#include "TG4MagneticField.h"
#include "TG4CachedMagneticField.h"
// #include "TG4ElectroMagneticField.h"
// #include "TG4GravityField.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"

#include <TVirtualMCApplication.h>
#include <TVirtualMagField.h>

#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>
#include <G4ChordFinder.hh>
#include "G4MagIntegratorDriver.hh"
#include <G4Mag_EqRhs.hh>
#include <G4Mag_UsualEqRhs.hh>
#include <G4Mag_SpinEqRhs.hh>
#include <G4EqMagElectricField.hh>
#include <G4EqEMFieldWithSpin.hh>
#include <G4EqEMFieldWithEDM.hh>
#include <G4ConstRK4.hh>
#include <G4ClassicalRK4.hh>
#include <G4SimpleRunge.hh>
#include <G4ExplicitEuler.hh>
#include <G4SimpleHeum.hh>
#include <G4ImplicitEuler.hh>
#include <G4NystromRK4.hh>
#include <G4RKG3_Stepper.hh>
#include <G4MagHelicalStepper.hh>
#include <G4HelixHeum.hh>
#include <G4HelixSimpleRunge.hh>
#include <G4HelixExplicitEuler.hh>
#include <G4HelixMixedStepper.hh>
#include <G4ExactHelixStepper.hh>
#include <G4HelixImplicitEuler.hh>
#include <G4MagErrorStepper.hh>
#include <G4CashKarpRKF45.hh>

//_____________________________________________________________________________
TG4Field::TG4Field(const TG4FieldParameters& parameters,
                   TVirtualMagField* magField,
                   G4LogicalVolume* lv)
  : fVirtualMagField(magField),
    fLogicalVolume(lv),
    fEquation(0),
    fStepper(0)
{
/// Default constructor

  // Consistency check
  if ( ! magField ) {
    TG4Globals::Exception(
      "TG4Field", "TG4Field:",
      "No TVirtualMagField is defined.");
  }

  Update(parameters);
}

//_____________________________________________________________________________
TG4Field::~TG4Field()
{
/// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
G4Field* TG4Field::CreateG4Field(const TG4FieldParameters& parameters,
                                 TVirtualMagField* magField)
{
/// Create magnetic, electromagnetic or garvity field according to
/// the provided field type

  if ( parameters.GetFieldType() == kMagnetic ) {
    if ( parameters.GetConstDistance() > 0. ) {
      fG4Field = new TG4CachedMagneticField(magField, parameters.GetConstDistance());
    } else {
      fG4Field = new TG4MagneticField(magField);
    }
  }
  // else if ( parameters.GetFieldType() == kElectroMagnetic ) {
  //   fG4Field = new TG4ElectroMagneticField(magField);
  // }

  // add em, gravity field

  return fG4Field;
}

//_____________________________________________________________________________
G4EquationOfMotion*
TG4Field::CreateEquation(EquationType equation)
{
/// Set the equation of motion of a particle in a field

  // magnetic fields
  G4MagneticField* magField = 0;
  if ( equation == kMagUsualEqRhs || equation == kMagSpinEqRhs ) {
    magField = dynamic_cast<G4MagneticField*>(fG4Field);
    if ( ! magField) {
     // add warning
     return 0;
    }
  }

  // electromagnetic fields
  G4ElectroMagneticField* elMagField = 0;
  if ( equation >= kEqMagElectric && equation <= kEqEMFieldWithEDM ) {
    elMagField = dynamic_cast<G4ElectroMagneticField*>(fG4Field);
    if ( ! elMagField) {
       // add warning
     return 0;
    }
  }

  // electromagnetic fields
  switch ( equation ) {
   case kMagUsualEqRhs:
     return new G4Mag_UsualEqRhs(magField);
     break;

   case kMagSpinEqRhs:
     return new G4Mag_SpinEqRhs(magField);
     break;

   case kEqMagElectric:
      return new G4EqMagElectricField(elMagField);
      break;

    case kEqEMFieldWithSpin:
      return new G4EqEMFieldWithSpin(elMagField);
      break;

    case kEqEMFieldWithEDM:
      return new G4EqEMFieldWithEDM(elMagField);
      break;
    case kUserEquation:
      // nothing to be done
      return 0;
      break;
  }

  TG4Globals::Exception(
    "TG4Field", "CreateEquation:",
    "Unknown equation type.");
  return 0;
}

//_____________________________________________________________________________
G4MagIntegratorStepper*
TG4Field::CreateStepper(G4EquationOfMotion* equation,
                                StepperType stepper)
{
/// Set the integrator of particle's equation of motion

  // Check steppers which require equation of motion of G4Mag_EqRhs type
  G4Mag_EqRhs* eqRhs = dynamic_cast<G4Mag_EqRhs*>(equation);
  if ( ! eqRhs && stepper > kSimpleRunge ) {
    TG4Globals::Exception(
      "TG4Field", "CreateStepper:",
      "The stepper type requires equation of motion of G4Mag_EqRhs type.");
    return 0;
  }

  switch ( stepper ) {
    case kCashKarpRKF45:
      return new G4CashKarpRKF45(equation);
      break;

    case kClassicalRK4:
      return new G4ClassicalRK4(equation);
      break;

    case kExplicitEuler:
      return new G4ExplicitEuler(equation);
      break;

    case kImplicitEuler:
      return new G4ImplicitEuler(equation);
      break;

    case kSimpleHeum:
      return new G4SimpleHeum(equation);
      break;

    case kSimpleRunge:
      return new G4SimpleRunge(equation);
      break;

    case kConstRK4:
      return new G4ConstRK4(eqRhs);
      break;

    case kExactHelixStepper:
      return new G4ExactHelixStepper(eqRhs);
      break;

    case kHelixExplicitEuler:
      return new G4HelixExplicitEuler(eqRhs);
      break;

    case kHelixHeum:
      return new G4HelixHeum(eqRhs);
      break;

    case kHelixImplicitEuler:
      return new G4HelixImplicitEuler(eqRhs);
      break;

    case kHelixMixedStepper:
      return new G4HelixMixedStepper(eqRhs);
      break;

    case kHelixSimpleRunge:
      return new G4HelixSimpleRunge(eqRhs);
      break;

    case kNystromRK4:
      return new G4NystromRK4(eqRhs);
      break;

    case kRKG3Stepper:
      return new G4RKG3_Stepper(eqRhs);
      break;
    case kUserStepper:
      // nothing to be done
      return 0;
      break;
  }

  TG4Globals::Exception(
    "TG4Field", "CreateStepper:",
    "Unknown stepper type.");
  return 0;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4Field::Update(const TG4FieldParameters& parameters)
{
/// Update field with new field parameters

  // Create field
  CreateG4Field(parameters, fVirtualMagField);

  G4FieldManager* fieldManager = 0;

  if ( ! fLogicalVolume ) {
     // global field
     fieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  } else {
     // local field
     fieldManager = new G4FieldManager();
     G4bool forceToAllDaughters = true;
     fLogicalVolume->SetFieldManager(fieldManager, forceToAllDaughters);
  }
  fieldManager->SetDetectorField(fG4Field);

  // Create equation of motion (or get the user one if defined)
  if ( parameters.GetEquationType() == kUserEquation ) {
    fEquation = parameters.GetUserEquationOfMotion();
    fEquation->SetFieldObj(fG4Field);
  } else {
    fEquation = CreateEquation(parameters.GetEquationType());
  }

  // Create stepper  (or get the user one if defined)
  if ( parameters.GetStepperType() == kUserStepper ) {
    fStepper = parameters.GetUserStepper();
  } else {
    fStepper = CreateStepper(fEquation, parameters.GetStepperType());
  }

  // Chord finder
  G4ChordFinder* chordFinder = 0;
  if ( parameters.GetFieldType() == kMagnetic ) {
    // Chord finder
    chordFinder
      = new G4ChordFinder(
              static_cast<G4MagneticField*>(fG4Field), parameters.GetStepMinimum(), fStepper);
    chordFinder->SetDeltaChord(parameters.GetDeltaChord());
  }
  else if ( parameters.GetFieldType() == kElectroMagnetic ) {
    G4MagInt_Driver* intDriver
      = new G4MagInt_Driver(parameters.GetStepMinimum(), fStepper, fStepper->GetNumberOfVariables());
    if ( intDriver ) {
      // Chord finder
      chordFinder = new G4ChordFinder(intDriver);
    }
  }
  fieldManager->SetChordFinder(chordFinder);

  fieldManager->SetMinimumEpsilonStep(parameters.GetMinimumEpsilonStep());
  fieldManager->SetMaximumEpsilonStep(parameters.GetMaximumEpsilonStep());
  fieldManager->SetDeltaOneStep(parameters.GetDeltaOneStep());
  fieldManager->SetDeltaIntersection(parameters.GetDeltaIntersection());
}
