//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4StackPopper.cxx
/// \brief Implementation of the TG4StackPopper class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4StackPopper.h"
#include "TG4ParticlesManager.h"
#include "TG4G3Units.h"
#include "TG4TrackInformation.h"

#include <G4Track.hh>
#include <G4IonTable.hh>

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TParticle.h>


G4ThreadLocal TG4StackPopper* TG4StackPopper::fgInstance = 0;

//_____________________________________________________________________________
TG4StackPopper::TG4StackPopper(const G4String& processName)
  : G4VProcess(processName, fUserDefined),
    fMCStack(0),
    fNofDoneTracks(0),
    fDoExclusiveStep(false)
{
/// Standard constructor

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4StackPopper", "TG4StackPopper",
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;

  // Cache thread-local pointers
  fMCStack = gMC->GetStack();
}

//_____________________________________________________________________________
TG4StackPopper::~TG4StackPopper()
{
/// Destructor

  fgInstance = 0;
}

//
// public methods
//

//_____________________________________________________________________________
G4double TG4StackPopper::PostStepGetPhysicalInteractionLength(
                             const G4Track& /*track*/,
                             G4double /*notUsed*/,
                             G4ForceCondition* condition)
{
/// Do not limit step, set condition to StronglyForced if there are
/// popped tracks in the stack

  *condition = InActivated;
  if ( HasPoppedTracks() ) {
    if ( fDoExclusiveStep ) {
      *condition = ExclusivelyForced;
    } else {
      *condition = StronglyForced;
    }
  }

  return DBL_MAX;
}

//_____________________________________________________________________________
G4VParticleChange* TG4StackPopper::PostStepDoIt(const G4Track& track,
                                                const G4Step& /*step*/)
{
/// Add particles from the stack as secondaries to the current particle

  aParticleChange.Initialize(track);

  if ( fMCStack->GetNtrack() == fNofDoneTracks )
    return &aParticleChange;

  Int_t currentTrackId = fMCStack->GetCurrentTrackNumber();
  Int_t nofTracksToPop = fMCStack->GetNtrack()-fNofDoneTracks;
  aParticleChange.SetNumberOfSecondaries(
                      aParticleChange.GetNumberOfSecondaries()+nofTracksToPop);

  for (G4int i=0; i<nofTracksToPop; ++i) {

    // Pop particle from the stack
    G4int itrack;
    TParticle* particle = fMCStack->PopNextTrack(itrack);
    ++fNofDoneTracks;

    if (!particle) {
      TG4Globals::Exception(
        "TG4StackPopper", "PostStepDoIt", "No particle popped from stack!");
      return &aParticleChange;
    }


    //G4cout << "TG4StackPopper::PostStepDoIt: Popped particle = "
    //       << particle->GetName()
    //       << " trackID = "<< itrack << G4endl;

    // Create dynamic particle
    G4DynamicParticle* dynamicParticle
      = TG4ParticlesManager::Instance()->CreateDynamicParticle(particle);
    if ( ! dynamicParticle ) {
      TG4Globals::Exception(
        "TG4StackPopper", "PostStepDoIt",
        "Conversion from Root particle -> G4 particle failed.");
    }

    // Define track

    G4ThreeVector position
      = TG4ParticlesManager::Instance()->GetParticlePosition(particle);
    G4double time = particle->T()*TG4G3Units::Time();

    G4Track* secondaryTrack
      = new G4Track(dynamicParticle, time, position);

    // set track information here to avoid saving track in the stack
    // for the second time
    TG4TrackInformation* trackInformation
      = new TG4TrackInformation(itrack);
        // the track information is deleted together with its
        // G4Track object
    trackInformation->SetIsUserTrack(true);
    trackInformation->SetPDGEncoding(particle->GetPdgCode());
    secondaryTrack->SetUserInformation(trackInformation);

    // Add track as a secondary
    aParticleChange.AddSecondary(secondaryTrack);
  }

  // Set back current track number in the track
  // (as stack may have changed it with popping particles)
  fMCStack->SetCurrentTrack(currentTrackId);

  // Set the kept track status if in exclusive step
  if ( fDoExclusiveStep ) {
    aParticleChange.ProposeTrackStatus(fTrackStatus);
    fDoExclusiveStep = false;
  }

  return &aParticleChange;
}

//_____________________________________________________________________________
void  TG4StackPopper::Notify()
{
/// Increment the number of done tracks

  ++fNofDoneTracks;
}

//_____________________________________________________________________________
void  TG4StackPopper::Reset()
{
/// Reset the number of done tracks to the number od tracks in stack
/// (when starting track)

  fNofDoneTracks = fMCStack->GetNtrack();
}

//_____________________________________________________________________________
void TG4StackPopper::SetDoExclusiveStep(G4TrackStatus trackStatus)
{
/// Activate performing exclusive step and keep the track status

  fDoExclusiveStep = true;
  fTrackStatus = trackStatus;
}

//_____________________________________________________________________________
G4bool TG4StackPopper::HasPoppedTracks() const
{
/// Return true if there are user tracks in stack

  return ( gMC->GetStack()->GetNtrack() != fNofDoneTracks );
}
