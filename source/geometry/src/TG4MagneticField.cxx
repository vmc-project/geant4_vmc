//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4MagneticField.cxx
/// \brief Implementation of the TG4MagneticField class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4MagneticField.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"

#include <TVirtualMCApplication.h>
#include <TVirtualMC.h>

#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>
#include <G4ChordFinder.hh>
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
TG4MagneticField::TG4MagneticField(const TG4FieldParameters& parameters)
  : G4MagneticField()
{
/// Default constructor

  Update(parameters);
}

//_____________________________________________________________________________
TG4MagneticField::~TG4MagneticField() 
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
G4EquationOfMotion* 
TG4MagneticField::CreateEquation(EquationType equation)
{
/// Set the equation of motion of a particle in a field

  switch ( equation ) {
    case kMagUsualEqRhs: 
      return new G4Mag_UsualEqRhs(this); 
      break;      
                     
    case kMagSpinEqRhs: 
      return new G4Mag_SpinEqRhs(this); 
      break;      
                     
    case kEqMagElectric:
      return new G4EqMagElectricField(this);
      break;      
                     
    case kEqEMFieldWithSpin:
      return new G4EqEMFieldWithSpin(this); 
      break;      
                     
    case kEqEMFieldWithEDM:
      return new G4EqEMFieldWithEDM(this); 
      break; 
  } 
  
  TG4Globals::Exception(
    "TG4MagneticField", "CreateEquation:",
    "Unknown equation type.");
  return 0;        
}                     
                     
//_____________________________________________________________________________
G4MagIntegratorStepper* 
TG4MagneticField::CreateStepper(G4EquationOfMotion* equation,
                                StepperType stepper)
{
/// Set the integrator of particle's equation of motion

  // Check steppers which require equation of motion of G4Mag_EqRhs type
  G4Mag_EqRhs* eqRhs = dynamic_cast<G4Mag_EqRhs*>(equation);
  if ( ! eqRhs && stepper > kSimpleRunge ) {
    TG4Globals::Exception(
      "TG4MagneticField", "CreateStepper:",
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
  }  
  
  TG4Globals::Exception(
    "TG4MagneticField", "CreateStepper:",
    "Unknown stepper type.");
  return 0;  
}      

//
// public methods
//

//_____________________________________________________________________________
void TG4MagneticField::GetFieldValue(const G4double point[3], G4double* bfield) const
{
/// Return the bfield values in the given point.

  // Set units
  const G4double g3point[3] = { point[0] / TG4G3Units::Length(),
                                point[1] / TG4G3Units::Length(),
                                point[2] / TG4G3Units::Length() };

  if ( gMC->GetMagField() ) {
    gMC->GetMagField()->Field(g3point, bfield);
  }
  else {  
    static Bool_t warn = true;
    if (warn) { 
      TG4Globals::Warning(
        "TG4MagneticField", "GetFieldValue", 
        TString("Using deprecated function TVirtualMCApplication::Field().")
        + TG4Globals::Endl()
        + TString("New TVirtualMagField interface should be used instead."));
      warn = false;
    }        

    TVirtualMCApplication::Instance()->Field(g3point, bfield);
  }  
  
  // Set units
  for (G4int i=0; i<3; i++) bfield[i] = bfield[i] * TG4G3Units::Field();
}

//_____________________________________________________________________________
void TG4MagneticField::Update(const TG4FieldParameters& parameters)
{
/// Update field with new field parameters

  G4FieldManager* fieldManager
     = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  fieldManager->SetDetectorField(this);
  
  // Geant4 default stepper
  G4EquationOfMotion* equation 
    = CreateEquation(parameters.GetEquationType());
  G4MagIntegratorStepper* stepper 
    = CreateStepper(equation, parameters.GetStepperType());
  
  // Chord finder
  G4ChordFinder* chordFinder
    = new G4ChordFinder(this, parameters.GetStepMinimum(), stepper);
  chordFinder->SetDeltaChord(parameters.GetDeltaChord());
  fieldManager->SetChordFinder(chordFinder);
  
  fieldManager->SetMinimumEpsilonStep(parameters.GetMinimumEpsilonStep());
  fieldManager->SetMaximumEpsilonStep(parameters.GetMaximumEpsilonStep());
  fieldManager->SetDeltaOneStep(parameters.GetDeltaOneStep());
  fieldManager->SetDeltaIntersection(parameters.GetDeltaIntersection());
}
