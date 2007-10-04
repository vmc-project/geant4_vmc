// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: event
// Class TG4TrackManager
// -----------------------
// See the class description in the header file.

#include "TG4TrackManager.h"
#include "TG4TrackInformation.h"
#include "TG4StepManager.h"
#include "TG4PhysicsManager.h"
#include "TG4ParticlesManager.h"
#include "TG4StackPopper.h"
#include "TG4SensitiveDetector.h"
#include "TG4SDServices.h"
#include "TG4Globals.h"

#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMC.h>
#include <TMCProcess.h>

#include <G4TrackVector.hh>
#include <G4TrackingManager.hh>
#include <G4UImanager.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>

// static data members
TG4TrackManager* TG4TrackManager::fgInstance = 0;

//_____________________________________________________________________________
TG4TrackManager::TG4TrackManager()     
  : TG4Verbose("trackManager"),
    fG4TrackingManager(0),   
    fSaveSecondaries(true),
    fSaveSecondariesInStep(false),
    fTrackCounter(0),
    fCurrentTrackID(0),
    fNofSavedSecondaries(0)
{
//
  if (fgInstance) { 
    TG4Globals::Exception(
      "TG4TrackManager", "TG4TrackManager", 
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;
}

//_____________________________________________________________________________
TG4TrackManager::~TG4TrackManager() {
//
  fgInstance = 0;
}

//
// private methods
//

//_____________________________________________________________________________
TG4TrackInformation* TG4TrackManager::GetTrackInformation(
                                           const G4Track* track) const
{
/// Return user track information.
 
#ifdef MCDEBUG
  G4VUserTrackInformation* trackInfo = track->GetUserInformation();
  if (!trackInfo) return 0;  

  TG4TrackInformation* tg4TrackInfo
    = dynamic_cast<TG4TrackInformation*>(trackInfo);
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
  
//
// public methods
//

//_____________________________________________________________________________
G4int TG4TrackManager::SetTrackInformation(const G4Track* track)
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
      trackIndex = trackID-1; 
    } 
    else { 
      if ( GetSaveSecondaries() )
        trackIndex = gMC->GetStack()->GetNtrack();
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
  // gMC->GetStack()->SetCurrentTrack(trackIndex);
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

  G4TrackVector* secondaryTracks 
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
void TG4TrackManager::TrackToStack(const G4Track* track)
{
/// Get all needed parameters from G4track and pass them
/// to the VMC stack.

  if ( VerboseLevel() > 1 )
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
  
  G4double px = momentum.x()/GeV;
  G4double py = momentum.y()/GeV;
  G4double pz = momentum.z()/GeV;
  G4double e = track->GetTotalEnergy()/GeV;

  G4ThreeVector position = track->GetPosition(); 
  G4double vx = position.x()/cm;
  G4double vy = position.y()/cm;
  G4double vz = position.z()/cm;
  // time of production - check if ekvivalent with G4
  G4double t = track->GetGlobalTime();

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
  
  G4double weight = 1.;

  // Track charge
  // Store the dynamic particle charge (which in case of ion may
  // be different from PDG charge) as status as there is no other 
  // place where we can do it
  G4int charge = G4int(track->GetDynamicParticle()->GetCharge()/eplus);
  G4int status = charge;   
  
  G4int ntr;
  // create particle 
  gMC->GetStack()->PushTrack(0, motherIndex, pdg, px, py, pz, e, vx, vy, vz, t,
                            polX, polY, polZ, mcProcess, ntr, weight, status);  
                   
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
    = TG4ParticlesManager::Instance()
      ->GetPDGEncoding(particle->GetG4code());

  // track kinematics  
  G4ThreeVector momentum = particle->GetMomentum(); 
  G4double px = momentum.x()/GeV;
  G4double py = momentum.y()/GeV;
  G4double pz = momentum.z()/GeV;
  G4double mass = particle->GetMass();
  G4double e = sqrt(momentum.mag()*momentum.mag() + mass*mass);

  G4ThreeVector position = vertex->GetPosition(); 
  G4double vx = position.x()/cm;
  G4double vy = position.y()/cm;
  G4double vz = position.z()/cm;
  // time of production - check if ekvivalent with G4
  G4double t = particle->GetProperTime();

  G4ThreeVector polarization = particle->GetPolarization(); 
  G4double polX = polarization.x();
  G4double polY = polarization.y();
  G4double polZ = polarization.z();

  // production process
  TMCProcess mcProcess = kPPrimary; 
  
  G4double weight = particle->GetWeight();

  // Track charge
  // Store the dynamic particle charge (which in case of ion may
  // be different from PDG charge) as status as there is no other 
  // place where we can do it
  G4int charge = G4int(particle->GetCharge()/eplus);
  G4int status = charge;   
  
  G4int ntr;
  // create particle 
  gMC->GetStack()->PushTrack(1, motherIndex, pdg, px, py, pz, e, vx, vy, vz, t,
                            polX, polY, polZ, mcProcess, ntr, weight, status);  
}                   


//_____________________________________________________________________________
void TG4TrackManager::SaveSecondaries(const G4Track* track,
                                      G4TrackVector* secondaries)
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
    if ( TG4StackPopper::Instance() ) TG4StackPopper::Instance()->Notify();
    ++fNofSavedSecondaries;  
  }    
}

//_____________________________________________________________________________
void TG4TrackManager::SetSaveSecondaries(G4bool saveSecondaries, 
                                                G4bool inStep)
{ 
/// Set control for saving secondaries in the VMC stack

  if ( ! inStep )
    fSaveSecondaries = saveSecondaries;
  else   
    fSaveSecondariesInStep = saveSecondaries;
}

//_____________________________________________________________________________
G4bool TG4TrackManager::IsUserTrack(const G4Track* track) const
{
/// Return true if the secondary track was defined by user

  return  
   GetTrackInformation(track) != 0x0 && GetTrackInformation(track)->IsUserTrack();
}   
