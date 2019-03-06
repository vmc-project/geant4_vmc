//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4TrackManager.cxx
/// \brief Implementation of the TG4TrackManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4TrackManager.h"
#include "TG4TrackInformation.h"
#include "TG4StepManager.h"
#include "TG4PhysicsManager.h"
#include "TG4ParticlesManager.h"
#include "TG4StackPopper.h"
#include "TG4SensitiveDetector.h"
#include "TG4SDServices.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"

#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMC.h>

#include <G4TrackVector.hh>
#include <G4TrackingManager.hh>
#include <G4UImanager.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>
#include <G4SystemOfUnits.hh>

// static data members
G4ThreadLocal TG4TrackManager* TG4TrackManager::fgInstance = 0;

//_____________________________________________________________________________
TG4TrackManager::TG4TrackManager()
  : TG4Verbose("trackManager"),
    fG4TrackingManager(0),
    fTrackSaveControl(kSaveInPreTrack),
    fMCStack(0),
    fStackPopper(0),
    fSaveDynamicCharge(false),
    fTrackCounter(0),
    fCurrentTrackID(0),
    fNofSavedSecondaries(0)
{
/// Default constructor

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4TrackManager", "TG4TrackManager",
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;
}

//_____________________________________________________________________________
TG4TrackManager::~TG4TrackManager()
{
/// Destructor

  fgInstance = 0;
}

//
// public methods
//

//_____________________________________________________________________________
void  TG4TrackManager::LateInitialize()
{
/// Cache thread-local pointers

  fStackPopper = TG4StackPopper::Instance();
}

//_____________________________________________________________________________
void  TG4TrackManager::AddPrimaryParticleId(G4int id)
{
/// Puts the given id in fPrimaryParticleIds.
/// This is then used to keep the primary track Ids identical
/// with Ids in VMC stack.

  fPrimaryParticleIds.push_back(id);
}

//_____________________________________________________________________________
G4int TG4TrackManager::SetTrackInformation(const G4Track* track, G4bool overWrite)
{
/// Set track index in VMC stack to track information
/// and return its value

#ifdef MCDEBUG
  if ( !fG4TrackingManager ) {
    TG4Globals::Exception("TG4TrackManager", "SetParentToTrackInformation",
      "G4TrackingManager has not been set.");
    return 0;
  }
#endif

  TG4TrackInformation* trackInfo = GetTrackInformation(track);

  if ( ! trackInfo ) {
    // create track information and set it to G4Track
    // if it does not yet exist
    trackInfo = new TG4TrackInformation();
    fG4TrackingManager->SetUserTrackInformation(trackInfo);
        // the track information is deleted together with its
        // G4Track object
  }

  // track index in the particles array
  Int_t trackIndex = trackInfo->GetTrackParticleID();
  if ( trackIndex < 0 ) {
     // Do not reset particle ID if it is already set
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    if ( parentID == 0 ) {
      // in VMC track numbering starts from 0
      // trackIndex = trackID-1;
      trackIndex = fPrimaryParticleIds[trackID-1];
    }
    else {
      if ( fTrackSaveControl != kDoNotSave ) {
        trackIndex = fMCStack->GetNtrack();
        if ( overWrite ) trackIndex--;
      }
      else
        trackIndex = fTrackCounter;
            // if secondaries are not stacked in VMC stack
            // use own counter for setting track index
    }
    if ( VerboseLevel() > 1 )
      G4cout << "TG4TrackManager::SetTrackInformation: setting " << trackIndex << G4endl;

    trackInfo->SetTrackParticleID(trackIndex);
  }

  // set current track number
  // fMCStack->SetCurrentTrack(trackIndex);
  ++fTrackCounter;

  return trackIndex;
}

//_____________________________________________________________________________
void TG4TrackManager::SetParentToTrackInformation(const G4Track* track)
{
/// Set parent track particle index to the secondary tracks.


#ifdef MCDEBUG
  if ( !fG4TrackingManager ) {
    TG4Globals::Exception("TG4TrackManager", "SetParentToTrackInformation",
      "G4TrackingManager has not been set.");
    return;
  }
#endif

  const G4TrackVector* secondaryTracks
    = fG4TrackingManager->GetSteppingManager()->GetSecondary();

  if ( ! secondaryTracks ) return;

  for ( G4int i=fNofSavedSecondaries; i<G4int(secondaryTracks->size()); i++) {
    G4Track* secondary = (*secondaryTracks)[i];

    // get parent track index
    TG4TrackInformation* parentInfo = GetTrackInformation(track);
#ifdef MCDEBUG
    if ( ! parentInfo ) {
      TG4Globals::Exception("TG4TrackManager", "SetParentToTrackInformation",
        "Parent track has no TG4TrackInformation set.");
      return;
    }
#endif
    G4int parentParticleID = parentInfo->GetTrackParticleID();

    // get or create track information and set it to the G4Track
    TG4TrackInformation* trackInfo = GetTrackInformation(secondary);
    if ( ! trackInfo ) {
      trackInfo = new TG4TrackInformation(-1);
      //G4cout << "TG4TrackManager::SetParentToTrackInformation: new trackInfo"
      //       << trackInfo << G4endl;
      // the track information is deleted together with its
      // G4Track object
    }
    trackInfo->SetParentParticleID(parentParticleID);
    secondary->SetUserInformation(trackInfo);
  }
}

//_____________________________________________________________________________
void  TG4TrackManager::SetBackPDGLifetime(const G4Track* aTrack)
{
/// Set back the PDG lifetime to the G4ParticleDefinition,
/// if it has been modified by user

    TG4TrackInformation* trackInfo = GetTrackInformation(aTrack);
    if ( trackInfo->GetPDGLifetime() > 0.0 ) {

      G4ParticleDefinition* particle
        = aTrack->GetDynamicParticle()->GetDefinition();
      particle->SetPDGLifeTime(trackInfo->GetPDGLifetime());
    }
}

#ifdef STACK_WITH_KEEP_FLAG
//_____________________________________________________________________________
void TG4TrackManager::TrackToStack(const G4Track* track, G4bool overWrite)
#else
//_____________________________________________________________________________
void TG4TrackManager::TrackToStack(const G4Track* track, G4bool /*overWrite*/)
#endif
{
/// Get all needed parameters from G4track and pass them
/// to the VMC stack.

  if ( VerboseLevel() > 2 )
    G4cout << "TG4TrackManager::TrackToStack" << G4endl;

  // parent particle index
  G4int parentID = track->GetParentID();
  G4int motherIndex;
  if (parentID == 0) {
    motherIndex = -1;
  }
  else {
    motherIndex = GetTrackInformation(track)->GetParentParticleID();
  }

  // PDG code
  G4int pdg
    = TG4ParticlesManager::Instance()->GetPDGEncoding(track->GetDefinition());

  // track kinematics
  G4ThreeVector momentum = track->GetMomentum();
  momentum *= 1./(TG4G3Units::Energy());

  G4double px = momentum.x();
  G4double py = momentum.y();
  G4double pz = momentum.z();
  G4double e = track->GetTotalEnergy()/TG4G3Units::Energy();

  G4ThreeVector position = track->GetPosition();
  position *= 1./(TG4G3Units::Length());
  G4double vx = position.x();
  G4double vy = position.y();
  G4double vz = position.z();
  G4double t = track->GetGlobalTime()/TG4G3Units::Time();


  G4ThreeVector polarization = track->GetPolarization();
  G4double polX = polarization.x();
  G4double polY = polarization.y();
  G4double polZ = polarization.z();

  // production process
  TMCProcess mcProcess;
  const G4VProcess* kpProcess = track->GetCreatorProcess();
  if (!kpProcess) {
    mcProcess = kPPrimary;
  }
  else {
    mcProcess = TG4PhysicsManager::Instance()->GetMCProcess(kpProcess);
    // distinguish kPDeltaRay from kPEnergyLoss
    if (mcProcess == kPEnergyLoss) mcProcess = kPDeltaRay;
  }

  G4double weight = track->GetWeight();

  G4int status = 0;
  if ( fSaveDynamicCharge ) {
    // Store the dynamic particle charge (which in case of ion may
    // be different from PDG charge) as status as there is no other
    // place where we can do it
    status = G4int(track->GetDynamicParticle()->GetCharge()/eplus);
  }

  G4int ntr;
#ifdef STACK_WITH_KEEP_FLAG
  // create particle
  fMCStack
    ->PushTrack(0, motherIndex, pdg, px, py, pz, e, vx, vy, vz, t,
                polX, polY, polZ, mcProcess, ntr, weight, status,
                overWrite);
        // Experimental code with flagging tracks in stack for overwrite;
        // not yet available in distribution
#else
  fMCStack
    ->PushTrack(0, motherIndex, pdg, px, py, pz, e, vx, vy, vz, t,
                polX, polY, polZ, mcProcess, ntr, weight, status);
#endif
}

//_____________________________________________________________________________
void TG4TrackManager::PrimaryToStack(const G4PrimaryVertex* vertex,
                                     const G4PrimaryParticle* particle)
{
/// Add primary particle to VMC stack

  // Mother particle index
  G4int motherIndex = -1;

  // PDG code
  G4int pdg
    = TG4ParticlesManager::Instance()->GetPDGEncoding(particle->GetG4code());

  // track kinematics
  G4ThreeVector momentum = particle->GetMomentum();
  momentum *= 1./(TG4G3Units::Energy());
  G4double px = momentum.x();
  G4double py = momentum.y();
  G4double pz = momentum.z();
  G4double mass = particle->GetMass();
  G4double e = sqrt(momentum.mag()*momentum.mag() + mass*mass);
  e /= (TG4G3Units::Energy());

  G4ThreeVector position = vertex->GetPosition();
  position *= 1./(TG4G3Units::Length());
  G4double vx = position.x();
  G4double vy = position.y();
  G4double vz = position.z();
  G4double t = vertex->GetT0()/TG4G3Units::Time();;

  G4ThreeVector polarization = particle->GetPolarization();
  G4double polX = polarization.x();
  G4double polY = polarization.y();
  G4double polZ = polarization.z();

  // production process
  TMCProcess mcProcess = kPPrimary;

  G4double weight = particle->GetWeight();

  G4int status = 1;
  if ( fSaveDynamicCharge ) {
    // Store the dynamic particle charge (which in case of ion may
    // be different from PDG charge) as status as there is no other
    // place where we can do it
    status = G4int(particle->GetCharge()/eplus);
  }

  G4int ntr;
  // create particle
  fMCStack->PushTrack(1, motherIndex, pdg, px, py, pz, e, vx, vy, vz, t,
                      polX, polY, polZ, mcProcess, ntr, weight, status);
}


//_____________________________________________________________________________
void TG4TrackManager::SaveSecondaries(const G4Track* track,
                                      const G4TrackVector* secondaries)
{
/// Save the secondary particles on VMC stack

  if ( track->GetTrackID() != fCurrentTrackID ) {
    fCurrentTrackID = track->GetTrackID();
    fNofSavedSecondaries = 0;
  }

  // Store parent track Id
  SetParentToTrackInformation(track);

  for ( G4int i=fNofSavedSecondaries; i<G4int(secondaries->size()); ++i) {

    G4Track* secondary =  (*secondaries)[i];

    //G4cout << i << "th secondary to be saved: "
    //       << secondary->GetDefinition()->GetParticleName()
    //       << G4endl;

    if ( GetTrackInformation(secondary) &&
         GetTrackInformation(secondary)->IsUserTrack() ) return;

    // Set track Id
    SetTrackInformation(secondary);

    // Save track in stack
    TrackToStack(secondary);

    // Notify a stack popper (if activated) about saving this secondary
    if ( fStackPopper ) fStackPopper->Notify();
    ++fNofSavedSecondaries;
  }
}

//_____________________________________________________________________________
void TG4TrackManager::ResetPrimaryParticleIds()
{
/// Clear the vector with the VMC stack primary particle Ids

  fPrimaryParticleIds.clear();
}

//_____________________________________________________________________________
TG4TrackInformation* TG4TrackManager::GetTrackInformation(
                                           const G4Track* track) const
{
/// Return user track information.

#ifdef MCDEBUG
  G4VUserTrackInformation* trackInfo = track->GetUserInformation();
  if (!trackInfo) return 0;

  //TG4TrackInformation* tg4TrackInfo
  //  = dynamic_cast<TG4TrackInformation*>(trackInfo);
  TG4TrackInformation* tg4TrackInfo
    = static_cast<TG4TrackInformation*>(trackInfo);
  if (!tg4TrackInfo) {
     TG4Globals::Exception(
       "TG4TrackManager", "GetTrackInformation",
       "Unknown track information type");
  }

  return tg4TrackInfo;
#else
  return (TG4TrackInformation*)track->GetUserInformation();
#endif
}

//_____________________________________________________________________________
G4bool TG4TrackManager::IsUserTrack(const G4Track* track) const
{
/// Return true if the secondary track was defined by user

  return
   GetTrackInformation(track) != 0x0 && GetTrackInformation(track)->IsUserTrack();
}
