//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VSpecialCuts.cxx
/// \brief Implementation of the TG4VSpecialCuts class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VSpecialCuts.h"
#include "TG4G3CutVector.h"
#include "TG4GeometryServices.h"
#include "TG4TrackManager.h"
#include "TG4TrackInformation.h"
#include "TG4Limits.h"

#include <G4UserLimits.hh>
#include <G4EnergyLossTables.hh>
#include <G4LossTableManager.hh>
#include <G4PhysicalConstants.hh>

//_____________________________________________________________________________
TG4VSpecialCuts::TG4VSpecialCuts(const G4String& processName)
  : G4VProcess(processName, fUserDefined),
    fLossTableManager(G4LossTableManager::Instance()),
    fTrackManager(TG4TrackManager::Instance())
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4VSpecialCuts::~TG4VSpecialCuts()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
G4double TG4VSpecialCuts::PostStepGetPhysicalInteractionLength(
                           const G4Track& track, G4double /*previousStepSize*/,
                           G4ForceCondition* condition)
{
/// Return the Step-size (actual length) which is allowed
/// by this process.

  // set condition
  *condition = NotForced;
  G4double proposedStep = DBL_MAX;

  // get limits
#ifdef MCDEBUG
  TG4Limits* limits
     = TG4GeometryServices::Instance()
         ->GetLimits(track.GetVolume()->GetLogicalVolume()->GetUserLimits());
#else
  TG4Limits* limits
    = (TG4Limits*) track.GetVolume()->GetLogicalVolume()->GetUserLimits();
#endif

  if (!limits) return proposedStep;

  // tracks flagged to stop
  TG4TrackInformation* trackInformation
    = fTrackManager->GetTrackInformation(&track);
  if ( trackInformation && trackInformation->IsStop() ) {
    return 0.;
  }

  // min kinetic energy (from limits)
  G4double minEkine = GetMinEkine(*limits, track);
  if ( track.GetKineticEnergy() <= minEkine ) return 0.;

  // max track length
  proposedStep
    = (limits->GetUserMaxTrackLength(track) - track.GetTrackLength());
  if (proposedStep < 0.) return 0.;

  // max time limit
  G4double tlimit = limits->GetUserMaxTime(track);
  if(tlimit < DBL_MAX) {
    G4double beta  = (track.GetDynamicParticle()->GetTotalMomentum())
      /(track.GetTotalEnergy());
    G4double dTime = (tlimit - track.GetGlobalTime());
    G4double temp  = beta*c_light*dTime;
    if (temp < 0.) { return 0.; }
    if (proposedStep > temp) { proposedStep = temp; }
  }

  // min remaining range
  // (only for charged particle except for chargedGeantino)
  G4double rmin = limits->GetUserMinRange(track);
  if (rmin > DBL_MIN) {
    G4ParticleDefinition* particle = track.GetDefinition();
    if ( ( particle->GetPDGCharge() != 0. ) &&
         ( particle->GetPDGMass() > 0.0 ) )  {
      G4double ekin = track.GetKineticEnergy();
      const G4MaterialCutsCouple* couple = track.GetMaterialCutsCouple();
      G4double rangeNow = fLossTableManager->GetRange(particle, ekin, couple);
      G4double temp = rangeNow - rmin;
      if ( temp < 0. ) { return 0.; }
      if ( proposedStep > temp ) { proposedStep = temp; }
    }
  }
  return proposedStep;
}

//_____________________________________________________________________________
G4VParticleChange* TG4VSpecialCuts::PostStepDoIt(const G4Track& track,
                                                 const G4Step& /*step*/)
{
/// Kill the current particle, if requested by G4UserLimits.
/// Set its status fStopButAlive (instead of previously used fStopAndKill
/// to let execute the at rest processes (decay)

  aParticleChange.Initialize(track);
  aParticleChange.ProposeEnergy(0.) ;
  aParticleChange.ProposeLocalEnergyDeposit(track.GetKineticEnergy()) ;
  aParticleChange.ProposeTrackStatus(fStopButAlive);
  return &aParticleChange;
}
