#ifndef TG4_PARTICLES_CHECKER_H
#define TG4_PARTICLES_CHECKER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ParticlesChecker.h
/// \brief Definition of the TG4ParticlesChecker class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"
#include "TG4ParticlesCheckerMessenger.h"

#include <set>

class G4ParticleDefinition;

class TParticlePDG;

/// \ingroup physics
/// \brief A helper class for comparing the basic particles properties in between
/// Root and Geant4
///
/// Verbose level:
/// - 1 : standard output
/// - 2 : printing also Root particles which have no equivalent in G4
/// - 3 : printing name candidates which are compared and more messages
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ParticlesChecker : public TG4Verbose
{
  public:
    /// The enumeration of "checkable" particle properties
    enum ParticleProperty {
      kName,     ///< mass
      kMass,     ///< mass
      kCharge,   ///< charge
      kLifetime, ///< lifetime
      kWidth,    ///< width
      kParity,   ///< parity
      kSpin,     ///< spin
      kIsospin,  ///< isospin
      kIsospin3, ///< isospin3
      kNone      ///< no property
    };

  public:
    TG4ParticlesChecker();
    ~TG4ParticlesChecker();

    static G4String GetParticlePropertyName(ParticleProperty property);
    static ParticleProperty GetParticleProperty(const G4String& propertyName);

    G4bool CheckParticles() const;
    G4bool CheckParticle(G4int pdgEncoding) const;

    // set methods
    void SetChecking(ParticleProperty property, G4bool check);
    void SetPrecision(G4double precision);

    // get methods
    const std::set<ParticleProperty>& GetAvailableProperties() const;
    const std::set<ParticleProperty>& GetCheckedProperties() const;

  private:
    /// Not implemented
    TG4ParticlesChecker(const TG4ParticlesChecker& right);
    /// Not implemented
    TG4ParticlesChecker& operator=(const TG4ParticlesChecker& right);

    // methods
    G4bool IsEqualRel(G4double dx, G4double dy, G4double epsilon) const;

    G4double GetPropertyValue(ParticleProperty property,
                              G4ParticleDefinition* g4Particle) const;
    G4double GetPropertyValue(ParticleProperty property,
                              TParticlePDG* rtParticle) const;

    void PrintCheckedProperties() const;

    G4bool CheckName(G4ParticleDefinition* g4Particle, TParticlePDG* rtParticle) const;
    G4bool CheckProperty(ParticleProperty property,
                         G4ParticleDefinition* g4Particle, TParticlePDG* rtParticle) const;
    G4bool CheckParticle(G4ParticleDefinition* g4Particle, TParticlePDG* rtParticle) const;


    // static data members
    static const G4double fgkDefaultPrecision;  ///< default precision

    // data members

    /// messenger for this class
    TG4ParticlesCheckerMessenger  fMessenger;

    /// set of available properties
    std::set<ParticleProperty>    fAvailableProperties;

    /// set of properties selected for checking
    std::set<ParticleProperty>    fCheckedProperties;

    /// precision for checking
    G4double fPrecision;
};

// inline functions

inline void TG4ParticlesChecker::SetPrecision(G4double precision) {
  /// Set the precision for checking.
  fPrecision = precision;
}

inline const std::set<TG4ParticlesChecker::ParticleProperty>&
TG4ParticlesChecker::GetAvailableProperties() const {
  /// Return the set of available properties
  return fAvailableProperties;
}

inline const std::set<TG4ParticlesChecker::ParticleProperty>&
TG4ParticlesChecker::GetCheckedProperties() const {
  /// Return the set of properties selected for checking
  return fCheckedProperties;
}

#endif //TG4_PARTICLES_CHECKER_H

