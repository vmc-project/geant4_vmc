// $Id: TG4StackPopper.cxx,v 1.1 2007/05/10 14:44:53 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4StackPopper
// ---------------------
// See the class description in the header file.

#include "TG4StackPopper.h"
#include "TG4ParticlesManager.h"
#include "TG4G3Units.h"
#include "TG4TrackInformation.h"

#include <G4Track.hh>
#include <G4IonTable.hh>

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TParticle.h>


TG4StackPopper* TG4StackPopper::fgInstance = 0;

//_____________________________________________________________________________
TG4StackPopper::TG4StackPopper(const G4String& processName)
  : G4VContinuousProcess(processName),
    fNofDoneTracks(0)
{
//
  if (fgInstance) { 
    TG4Globals::Exception(
      "TG4StackPopper", "TG4StackPopper", 
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;
}

//_____________________________________________________________________________
TG4StackPopper::~TG4StackPopper() {
//
  fgInstance = 0;
}

//
// public methods
//


//_____________________________________________________________________________
G4VParticleChange* TG4StackPopper::AlongStepDoIt(const G4Track& track, 
                                                 const G4Step& /*step*/)
{
/// Add particles from the stack as secondaries to the current particle
 
  aParticleChange.Initialize(track);
  
  if ( gMC->GetStack()->GetNtrack() == fNofDoneTracks ) 
    return &aParticleChange;

  TG4ParticlesManager* particlesManager = TG4ParticlesManager::Instance();
  TVirtualMCStack* stack = gMC->GetStack();
  Int_t currentTrackId = stack->GetCurrentTrackNumber();
  Int_t nofTracksToPop = stack->GetNtrack()-fNofDoneTracks;
  aParticleChange.SetNumberOfSecondaries(
                      aParticleChange.GetNumberOfSecondaries()+nofTracksToPop);
 
  for (G4int i=0; i<nofTracksToPop; ++i) {

    // Pop particle from the stack
    G4int itrack;
    TParticle* particle = stack->PopNextTrack(itrack);
    ++fNofDoneTracks;

    if (!particle) {
      TG4Globals::Exception(
        "TG4StackPopper", "AlongStepDoIt", "No particle popped from stack!");
      return &aParticleChange;
    }  
      

    //G4cout << "TG4StackPopper::AlongStepDoIt: Popped particle = " 
    //       << particle->GetName() 
    //       << " trackID = "<< itrack << G4endl;

    // Create dynamic particle
    G4DynamicParticle* dynamicParticle 
      = particlesManager->CreateDynamicParticle(particle);
    if ( ! dynamicParticle ) {
      TG4Globals::Exception(
        "TG4StackPopper", "AlongStepDoIt",
        "Conversion from Root particle -> G4 particle failed.");
    }    
 
    // Define track

    G4ThreeVector position 
      = particlesManager->GetParticlePosition(particle);
    G4double time = particle->T()*TG4G3Units::Time(); 

    G4Track* track  
      = new G4Track(dynamicParticle, time, position);

    // set track information here to avoid saving track in the stack
    // for the second time  
    TG4TrackInformation* trackInformation 
      = new TG4TrackInformation(itrack);
        // the track information is deleted together with its
        // G4Track object  
    trackInformation->SetIsUserTrack(true);
    track->SetUserInformation(trackInformation);
      
    // Add track as a secondary
    aParticleChange.AddSecondary(track);
  }
  
  // Set back current track number in the track
  // (as stack may have changed it with popping particles)
  stack->SetCurrentTrack(currentTrackId);
  
  return &aParticleChange;
}

//_____________________________________________________________________________
void  TG4StackPopper::Notify()
{
/// Increment the number of done tracks

  // G4cout << "TG4StackPopper::Update" << G4endl;
  ++fNofDoneTracks;
}                           

//_____________________________________________________________________________
void  TG4StackPopper::Reset()
{
/// Reset the number of done tracks to the number od tracks in stack
/// (when starting track)

  // G4cout << "TG4StackPopper::Reset" << G4endl;
  fNofDoneTracks = gMC->GetStack()->GetNtrack();
}                           

