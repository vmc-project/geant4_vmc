// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
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

//_____________________________________________________________________________
TG4VSpecialCuts::TG4VSpecialCuts(const G4String& processName)
  : G4VProcess(processName, fUserDefined),
    fLossTableManager(G4LossTableManager::Instance())
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
  G4double minStep = (1.0e-9)*m;
  
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
    = TG4TrackManager::Instance()->GetTrackInformation(&track); 
  if ( trackInformation && trackInformation->IsStop() ) {
    return minStep;
  }  

  // max track length
  proposedStep 
    = (limits->GetUserMaxTrackLength(track) - track.GetTrackLength());
  if (proposedStep < 0.) return minStep;

  // max time limit
  G4double beta 
    = (track.GetDynamicParticle()->GetTotalMomentum()) /
      (track.GetTotalEnergy());
  G4double time
    = (limits->GetUserMaxTime(track) - track.GetGlobalTime());
  G4double temp = beta*c_light*time;
  if (temp < 0.) return minStep;
  if (proposedStep > temp) proposedStep = temp;

  G4ParticleDefinition* particle = track.GetDefinition();
  if (particle->GetPDGCharge() != 0.) {

    // min remaining range
    G4double kinEnergy = track.GetKineticEnergy();
    const G4MaterialCutsCouple* couple = track.GetMaterialCutsCouple();
    if ( ! couple ) {
      G4String text = "No material cuts couple defined for volume =  ";
      text = text + track.GetVolume()->GetLogicalVolume()->GetName();
      TG4Globals::Exception(
        "TG4VSpecialCuts", "PostStepGetPhysicalInteractionLength", text);
    }    
    
    G4double rangeNow = fLossTableManager->GetRange(particle, kinEnergy, couple);
    G4double rmin = limits->GetUserMinRange(track);
    if (rmin > DBL_MIN) {
      temp = rangeNow - rmin;
      if (temp < 0.) return 0.;
      if (proposedStep > temp) proposedStep = temp;
    }         
    if (temp < 0.) return minStep;
    if (proposedStep > temp) proposedStep = temp;

    // min kinetic energy (from limits)
    // the kin energy cut can be applied only in case
    // G4EnergyLossTables are defined for the particle    
    G4double minEkine = GetMinEkine(*limits, track);
    if (minEkine > DBL_MIN) {
         rmin = fLossTableManager->GetRange(particle, minEkine, couple);
         temp = rangeNow - rmin;
         if (temp < 0.) return 0.;
         if (proposedStep > temp) proposedStep = temp;
       }         
  }
  else {  
    // min kinetic energy (from limits)
    // for neutral particles
    G4double minEkine = GetMinEkine(*limits, track);
    if (track.GetKineticEnergy() <= minEkine) return minStep;
  }
  return proposedStep;
}

//_____________________________________________________________________________
G4VParticleChange* TG4VSpecialCuts::PostStepDoIt(const G4Track& track, 
                                                 const G4Step& /*step*/)
{
/// Kill the current particle, if requested by G4UserLimits.
 
  aParticleChange.Initialize(track);
  aParticleChange.ProposeEnergy(0.) ;
  aParticleChange.ProposeLocalEnergyDeposit(track.GetKineticEnergy()) ;
  aParticleChange.ProposeTrackStatus(fStopAndKill);
  return &aParticleChange;
}
