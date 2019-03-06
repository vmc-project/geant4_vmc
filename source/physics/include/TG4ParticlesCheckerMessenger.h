#ifndef TG4_PARTICLES_CHECKER_MESSENGER_H
#define TG4_PARTICLES_CHECKER_MESSENGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ParticlesCheckerMessenger.h
/// \brief Definition of the TG4ParticlesCheckerMessenger class
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

#include <set>

class TG4ParticlesChecker;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;

/// \ingroup physics
/// \brief Messenger class that defines commands for TG4ParticlesChecker
///
/// Implements commands:
/// - /mcParticlesChecker/checkParticles
/// - /mcParticlesChecker/checkParticle  pdgEncoding
/// - /mcParticlesChecker/selectProperty propertyName
/// - /mcParticlesChecker/setChecking true|false
/// - /mcParticlesChecker/setPrecision epsilon
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4ParticlesCheckerMessenger: public G4UImessenger
{
  public:
    TG4ParticlesCheckerMessenger(TG4ParticlesChecker* particlesChecker);
    virtual ~TG4ParticlesCheckerMessenger();

    // methods
    void Init();
    virtual void SetNewValue(G4UIcommand* command, G4String string);


  private:
    /// Not implemented
    TG4ParticlesCheckerMessenger();
    /// Not implemented
    TG4ParticlesCheckerMessenger(const TG4ParticlesCheckerMessenger& right);
    /// Not implemented
    TG4ParticlesCheckerMessenger& operator=(const TG4ParticlesCheckerMessenger& right);

    // data members
    TG4ParticlesChecker*  fParticlesChecker; ///< associated class
    G4UIdirectory*        fDirectory;        ///< command directory
    G4String              fSelectedProperty; ///< selected property

    G4UIcmdWithoutParameter*  fCheckParticlesCmd;   ///< command: checkParticles
    G4UIcmdWithAnInteger*     fCheckParticleCmd;    ///< command: checkParticle
    G4UIcmdWithAString*       fSelectPropertyCmd;   ///< command: selectProperty
    G4UIcmdWithABool*         fSetCheckingCmd;      ///< command: setChecking
    G4UIcmdWithADouble*       fSetPrecisionCmd;     ///< command: setPrecision
};

#endif //TG4_PARTICLES_CHECKER_MESSENGER_H
