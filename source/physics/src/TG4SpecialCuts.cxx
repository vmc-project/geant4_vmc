//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SpecialCuts.cxx
/// \brief Implementation of the TG4SpecialCutsFor* classes
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SpecialCuts.h"
#include "TG4Limits.h"

//
//  Class TG4SpecialCutsForChargedHadron implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForChargedHadron::TG4SpecialCutsForChargedHadron(
                                                 const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForChargedHadron::~TG4SpecialCutsForChargedHadron()
{
/// Destructor
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForChargedHadron::GetMinEkine(const TG4Limits& limits,
                                                     const G4Track& track) const
{
/// Return the min kinetic energy cut from limits.
// ---

  return limits.GetMinEkineForChargedHadron(track);
}

//
//  Class TG4SpecialCutsForElectron implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForElectron::TG4SpecialCutsForElectron(const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard and default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForElectron::~TG4SpecialCutsForElectron()
{
/// Destructor
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForElectron::GetMinEkine(const TG4Limits& limits,
                                                const G4Track& track) const
{
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForElectron(track);
}

//
//  Class TG4SpecialCutsForEplus implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForEplus::TG4SpecialCutsForEplus(const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard and default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForEplus::~TG4SpecialCutsForEplus()
{
/// Destructor
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForEplus::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForEplus(track);
}

//_____________________________________________________________________________
G4VParticleChange* TG4SpecialCutsForEplus::PostStepDoIt(const G4Track& track,
                                                 const G4Step& /*step*/)
{
/// Override method from G4VSpecialCuts and set energy deposit
/// to total e+ energy

  aParticleChange.Initialize(track);
  aParticleChange.ProposeEnergy(0.) ;
  aParticleChange.ProposeLocalEnergyDeposit(track.GetTotalEnergy()) ;
  aParticleChange.ProposeTrackStatus(fStopAndKill);
  return &aParticleChange;
}

//
//  Class TG4SpecialCutsForGamma implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForGamma::TG4SpecialCutsForGamma(const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard and default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForGamma::~TG4SpecialCutsForGamma()
{
/// Destructor
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForGamma::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForGamma(track);
}

//
//  Class TG4SpecialCutsForMuon implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForMuon::TG4SpecialCutsForMuon(const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard and default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForMuon::~TG4SpecialCutsForMuon()
{
/// Destructor
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForMuon::GetMinEkine(const TG4Limits& limits,
                                            const G4Track& track) const
{
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForMuon(track);
}

//
//  Class TG4SpecialCutsForNeutralHadron implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForNeutralHadron::TG4SpecialCutsForNeutralHadron(
                                              const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard and default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForNeutralHadron::~TG4SpecialCutsForNeutralHadron()
{
/// Destructor
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForNeutralHadron::GetMinEkine(const TG4Limits& limits,
                                                     const G4Track& track) const
{
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForNeutralHadron(track);
}

//
//  Class TG4SpecialCutsForOther implementation
//

//_____________________________________________________________________________
TG4SpecialCutsForOther::TG4SpecialCutsForOther(const G4String& processName)
  : TG4VSpecialCuts(processName)
{
/// Standard and default constructor
}

//_____________________________________________________________________________
TG4SpecialCutsForOther::~TG4SpecialCutsForOther()
{
/// Destructor
}

//_____________________________________________________________________________
G4double TG4SpecialCutsForOther::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{
/// Return the min kinetic energy cut from limits.

  return limits.GetMinEkineForOther(track);
}
