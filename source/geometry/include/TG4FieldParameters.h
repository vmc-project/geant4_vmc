#ifndef TG4_FIELD_PARAMETERS_H
#define TG4_FIELD_PARAMETERS_H

//-------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4FieldParameters.h
/// \brief Definition of the TG4FieldParameters class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4MagneticField.hh>
#include <globals.hh>

class TG4FieldParametersMessenger;

class G4EquationOfMotion;
class G4MagIntegratorStepper;

/// The available fields in Geant4
enum FieldType {
  kMagnetic,          ///< magnetic field
  kElectroMagnetic,   ///< electromagnetic field
  kGravity            ///< gravity field
};

/// The available equations of motion of a particle in a field
/// in Geant4
enum EquationType {
  kMagUsualEqRhs,     ///< G4Mag_UsualEqRhs: the standard right-hand side for equation
                      /// of motion.
  kMagSpinEqRhs,      ///< G4Mag_SpinEqRhs: the equation of motion for a particle with spin
                      /// in a pure magnetic field
  kEqMagElectric,     ///< G4EqMagElectricField: Equation of motion in a combined
                      ///  electric and magnetic field
  kEqEMFieldWithSpin, ///< G4EqEMFieldWithSpin: Equation of motion for a particle with spin
                      ///  in a combined electric and magnetic field
  kEqEMFieldWithEDM,  ///< G4EqEMFieldWithEDM: Equation of motion in a combined
                      /// electric and magnetic field, with spin tracking for both MDM and
                      /// EDM terms
  kUserEquation       ///< User defined equation of motion
};

/// The available integrator of particle's equation of motion
/// in Geant4
enum StepperType {
  // steppers with equation of motion of generic type (G4EquationOfMotion)
  kCashKarpRKF45,     ///< G4CashKarpRKF45
  kClassicalRK4,      ///< G4ClassicalRK4
  kExplicitEuler,     ///< G4ExplicitEuler
  kImplicitEuler,     ///< G4ImplicitEuler
  kSimpleHeum,        ///< G4SimpleHeum
  kSimpleRunge,       ///< G4SimpleRunge

  // steppers with equation of motion of G4Mag_UsualEqRhs type
  kConstRK4,          ///< G4ConstRK4
  kExactHelixStepper, ///< G4ExactHelixStepper
  kHelixExplicitEuler,///< G4HelixExplicitEuler
  kHelixHeum,         ///< G4HelixHeum
  kHelixImplicitEuler,///< G4HelixImplicitEuler
  kHelixMixedStepper, ///< G4HelixMixedStepper
  kHelixSimpleRunge,  ///< G4HelixSimpleRunge
  kNystromRK4,        ///< G4NystromRK4
  kRKG3Stepper,       ///< G4RKG3_Stepper
  kUserStepper        ///< User defined stepper
};

/// \ingroup geometry
/// \brief The magnetic field parameters
///
/// The class defines the type of equation of motion of a particle
/// in a field and the integration method, as well as other accuracy
/// parameters.
///
/// The default values correspond to the defaults set in Geant4
/// (taken from Geant4 9.3 release.)
/// As Geant4 classes to not provide access methods for these defaults,
/// the defaults have to be checked with each new Geant4 release.
///
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4FieldParameters
{
  public:
    TG4FieldParameters(const G4String& volumeName = "");
    virtual ~TG4FieldParameters();

    // methods
    void PrintParameters() const;

    static G4String FieldTypeName(FieldType field);
    static G4String EquationTypeName(EquationType equation);
    static G4String StepperTypeName(StepperType stepper);
    static FieldType    GetFieldType(const G4String& name);
    static EquationType GetEquationType(const G4String& name);
    static StepperType  GetStepperType(const G4String& name);

    // set methods
    void SetFieldType(FieldType field);
    void SetEquationType(EquationType equation);
    void SetStepperType(StepperType stepper);
    void SetUserEquationOfMotion(G4EquationOfMotion* equation);
    void SetUserStepper(G4MagIntegratorStepper* stepper);

    void SetStepMinimum(G4double value);
    void SetDeltaChord(G4double value);
    void SetDeltaOneStep(G4double value);
    void SetDeltaIntersection(G4double value);
    void SetMinimumEpsilonStep(G4double value);
    void SetMaximumEpsilonStep(G4double value);
    void SetConstDistance(G4double value);
    void SetIsMonopole(G4bool isMonopole);

    // get methods
    G4String  GetVolumeName() const;

    FieldType GetFieldType() const;
    EquationType GetEquationType() const;
    StepperType  GetStepperType() const;
    G4EquationOfMotion*     GetUserEquationOfMotion() const;
    G4MagIntegratorStepper* GetUserStepper() const;

    G4double GetStepMinimum() const;
    G4double GetDeltaChord() const;
    G4double GetDeltaOneStep() const;
    G4double GetDeltaIntersection() const;
    G4double GetMinimumEpsilonStep() const;
    G4double GetMaximumEpsilonStep() const;
    G4double GetConstDistance() const;
    G4bool   GetIsMonopole() const;

  private:
    // static data members
    //
    /// Default minimum step in G4ChordFinder
    static const G4double  fgkDefaultStepMinimum;
    /// Default delta chord in G4ChordFinder
    static const G4double  fgkDefaultDeltaChord;
    /// Default delta one step in global field manager
    static const G4double  fgkDefaultDeltaOneStep;
    /// Delta intersection in global field manager
    static const G4double  fgkDefaultDeltaIntersection;
    /// Default minimum epsilon step in global field manager
    static const G4double  fgkDefaultMinimumEpsilonStep;
    /// Default maximum epsilon step in global field manager
    static const G4double  fgkDefaultMaximumEpsilonStep;
    /// Default constant distance
    static const G4double  fgkDefaultConstDistance;

    // data members
    //
    /// Messenger for this class
    TG4FieldParametersMessenger* fMessenger;

    /// The name of associated volume, if local field
    G4String  fVolumeName;

    /// Minimum step in G4ChordFinder
    G4double  fStepMinimum;
    /// Delta chord in G4ChordFinder
    G4double  fDeltaChord;
    /// Delta one step in global field manager
    G4double  fDeltaOneStep;
    /// Delta intersection in global field manager
    G4double  fDeltaIntersection;
    /// Minimum epsilon step in global field manager
    G4double  fMinimumEpsilonStep;
    /// Maximum epsilon step in global field manager
    G4double  fMaximumEpsilonStep;

    /// Type of field
    FieldType  fField;

    /// Type of equation of motion of a particle in a field
    EquationType  fEquation;

    /// Type of integrator of particle's equation of motion
    StepperType  fStepper;

    /// User defined equation of motion
    G4EquationOfMotion* fUserEquation;

    /// User defined integrator of particle's equation of motion
    G4MagIntegratorStepper*  fUserStepper;

    /// The distance within which the field is considered constant
    G4double  fConstDistance;

    /// An option to create an extra monopole field integrator
    /// which will be activated directly by G4MonopoleTransportation
    G4bool fIsMonopole;
};

// inline functions

/// Set type of field
inline void TG4FieldParameters::SetFieldType(FieldType field) {
  fField = field;
}

/// Set the type of equation of motion of a particle in a field
inline void TG4FieldParameters::SetEquationType(EquationType equation) {
  fEquation = equation;
}

/// Set the type of integrator of particle's equation of motion
inline void TG4FieldParameters::SetStepperType(StepperType stepper) {
  fStepper = stepper;
}

/// Set minimum step in G4ChordFinder
inline void TG4FieldParameters::SetStepMinimum(G4double value) {
   fStepMinimum = value;
}

/// Set delta chord in G4ChordFinder
inline void TG4FieldParameters::SetDeltaChord(G4double value) {
   fDeltaChord = value;
}

/// Set delta one step in global field manager
inline void TG4FieldParameters::SetDeltaOneStep(G4double value) {
  fDeltaOneStep = value;
}

/// Set delta intersection in global field manager
inline void TG4FieldParameters::SetDeltaIntersection(G4double value) {
  fDeltaIntersection = value;
}

/// Set minimum epsilon step in global field manager
inline void TG4FieldParameters::SetMinimumEpsilonStep(G4double value) {
  fMinimumEpsilonStep = value;
}

/// Set maximum epsilon step in global field manager
inline void TG4FieldParameters::SetMaximumEpsilonStep(G4double value) {
  fMaximumEpsilonStep = value;
}

/// Set the distance within which the field is considered constant
inline void TG4FieldParameters::SetConstDistance(G4double value) {
  fConstDistance = value;
}

/// Set the option to create an extra monopole field integrator
/// which will be activated directly by G4MonopoleTransportation
inline void TG4FieldParameters::SetIsMonopole(G4bool isMonopole) {
  fIsMonopole = isMonopole;
}

/// Return the name of associated volume, if local field
inline  G4String  TG4FieldParameters::GetVolumeName() const {
  return fVolumeName;
}

/// Return the type of field
inline FieldType TG4FieldParameters::GetFieldType() const {
  return fField;
}

/// Return the type of equation of motion of a particle in a field
inline EquationType TG4FieldParameters::GetEquationType() const {
  return fEquation;
}

/// Return the type of integrator of particle's equation of motion
inline StepperType TG4FieldParameters::GetStepperType() const {
  return fStepper;
}

/// Return the user defined equation of motion
inline G4EquationOfMotion*  TG4FieldParameters::GetUserEquationOfMotion() const {
  return fUserEquation;
}

/// Return the user defined integrator of particle's equation of motion
inline G4MagIntegratorStepper* TG4FieldParameters::GetUserStepper() const {
  return fUserStepper;
}

/// Return minimum step in G4ChordFinder
inline G4double TG4FieldParameters::GetStepMinimum() const {
  return fStepMinimum;
}

/// Return delta chord in G4ChordFinder
inline G4double TG4FieldParameters::GetDeltaChord() const {
  return fDeltaChord;
}

/// Return delta one step in global field manager
inline G4double TG4FieldParameters::GetDeltaOneStep() const {
  return fDeltaOneStep;
}

/// Return delta intersection in global field manager
inline G4double TG4FieldParameters::GetDeltaIntersection() const {
  return fDeltaIntersection;
}

/// Return minimum epsilon step in global field manager
inline G4double TG4FieldParameters::GetMinimumEpsilonStep() const {
  return fMinimumEpsilonStep;
}

/// Return maximum epsilon step in global field manager
inline G4double TG4FieldParameters::GetMaximumEpsilonStep() const {
  return fMaximumEpsilonStep;
}

/// Return the distance within which the field is considered constant
inline G4double TG4FieldParameters::GetConstDistance() const {
  return fConstDistance;
}

/// Return the option to create an extra monopole field integrator
/// which will be activated directly by G4MonopoleTransportation
inline G4bool TG4FieldParameters::GetIsMonopole () const {
  return fIsMonopole;
}

#endif //TG4_FIELD_PARAMETERS_H
