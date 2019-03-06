#ifndef TG4_FIELD_PARAMETERS_MESSENGER_H
#define TG4_FIELD_PARAMETERS_MESSENGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4FieldParametersMessenger.h
/// \brief Definition of the TG4FieldParametersMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4FieldParameters;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

/// \ingroup geometry
/// \brief Messenger class that defines commands for TG4DetConstruction.
///
/// Implements commands:
/// - /mcMagField/fieldType fieldType     \n
///       fieldType = Magnetic | ElectroMagnetic | Gravity
/// - /mcMagField/equationType eqType     \n
///       eqType = MagUsualEqRhs | MagSpinEqRhs | EqMagElectric | EMFieldWithSpin |
///                EqEMFieldWithEDM
/// - /mcMagField/stepperType stepperType \n
///       stepperType = CashKarpRKF45 | ClassicalRK4 | ExplicitEuler | ImplicitEuler |
///                     SimpleHeum | SimpleRunge | ConstRK4 | ExactHelixStepper |
///                     HelixExplicitEuler | HelixHeum | HelixImplicitEuler |
///                     HelixMixedStepper | HelixSimpleRunge | NystromRK4 | RKG3Stepper
/// - /mcMagField/setStepMinimum value
/// - /mcMagField/setDeltaChord  value
/// - /mcMagField/setDeltaOneStep value
/// - /mcMagField/setDeltaIntersection value
/// - /mcMagField/setMinimumEpsilonStep value
/// - /mcMagField/setMaximumEpsilonStep value
/// - /mcMagField/setConstDistance value
/// - /mcMagField/setIsMonopole true|false
/// - /mcMagField/printParameters
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4FieldParametersMessenger: public G4UImessenger
{
  public:
    TG4FieldParametersMessenger(TG4FieldParameters* fieldParameters);
    virtual ~TG4FieldParametersMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);

  private:
    /// Not implemented
    TG4FieldParametersMessenger();
    /// Not implemented
    TG4FieldParametersMessenger(const TG4FieldParametersMessenger& right);
    /// Not implemented
    TG4FieldParametersMessenger& operator=(
                                const TG4FieldParametersMessenger &right);

    // \data members
    TG4FieldParameters* fFieldParameters; ///< associated class
    G4UIdirectory*      fDirectory;       ///< command directory

    //
    // commands data members

    /// command: fieldType
    G4UIcmdWithAString*         fFieldTypeCmd;

    /// command: equationType
    G4UIcmdWithAString*         fEquationTypeCmd;

    /// command: stepperType
    G4UIcmdWithAString*         fStepperTypeCmd;

    /// command: setStepMinimum
    G4UIcmdWithADoubleAndUnit*  fSetStepMinimumCmd;

    /// command: setDeltaChord
    G4UIcmdWithADoubleAndUnit*  fSetDeltaChordCmd;

    /// command: setDeltaOneStep
    G4UIcmdWithADoubleAndUnit*  fSetDeltaOneStepCmd;

    /// command: setDeltaIntersection
    G4UIcmdWithADoubleAndUnit*  fSetDeltaIntersectionCmd;

    /// command: setMinimumEpsilon
    G4UIcmdWithADouble*         fSetMinimumEpsilonStepCmd;

    /// command: setMaximumEpsilon
    G4UIcmdWithADouble*         fSetMaximumEpsilonStepCmd;

    /// command: setConstDistance
    G4UIcmdWithADoubleAndUnit*  fSetConstDistanceCmd;

    /// command: setIsMonopole
    G4UIcmdWithABool*           fSetIsMonopoleCmd;

    /// command: printParameters
    G4UIcmdWithoutParameter*    fPrintParametersCmd;
};

#endif //TG4_FIELD_PARAMETERS_MESSENGER_H/

