//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4PrimaryGeneratorAction.cxx
/// \brief Implementation of the TG4PrimaryGeneratorAction class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4PrimaryGeneratorAction.h"
#include "TG4EventAction.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"
#include "TG4ParticlesManager.h"
#include "TG4PrimaryGeneratorMessenger.h"
#include "TG4RunManager.h"
#include "TG4StateManager.h"
#include "TG4TrackManager.h"
#include "TG4UserIon.h"

#include <G4Event.hh>
#include <G4IonTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4RunManager.hh>

#include <TMCManagerStack.h>
#include <TMCParticleStatus.h>
#include <TParticle.h>
#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMCStack.h>

// Moved after Root includes to avoid shadowed variables
// generated from short units names
#include <G4SystemOfUnits.hh>

namespace
{

TG4EventAction* GetEventAction()
{
  return static_cast<TG4EventAction*>(const_cast<G4UserEventAction*>(
    G4RunManager::GetRunManager()->GetUserEventAction()));
}

} // namespace

//_____________________________________________________________________________
TG4PrimaryGeneratorAction::TG4PrimaryGeneratorAction()
  : TG4Verbose("primaryGeneratorAction"),
    fMessenger(0),
    fParticlesManager(0),
    fTrackManager(0),
    fMCStack(0),
    fMCManagerStack(0),
    fCached(false),
    fSkipUnknownParticles(false)
{
  /// Default constructor

  fMessenger = new TG4PrimaryGeneratorMessenger(this);
}

//_____________________________________________________________________________
TG4PrimaryGeneratorAction::~TG4PrimaryGeneratorAction()
{
  /// Destructor

  delete fMessenger;
}

//
// private methods
//

//_____________________________________________________________________________
G4bool TG4PrimaryGeneratorAction::CheckVMCStack(TVirtualMCStack* mcStack) const
{

  if (!mcStack) {
    TG4Globals::Warning(
      "TG4PrimaryGeneratorAction", "CheckVMCStack", "Found nullptr for stack.");
    return false;
  }

  if (mcStack->GetNtrack() <= 0) {
    TG4Globals::Warning("TG4PrimaryGeneratorAction", "CheckVMCStack",
      "No primary particles found on the stack.");
    return false;
  }
  return true;
}

//_____________________________________________________________________________
G4bool TG4PrimaryGeneratorAction::CheckParticleDefinition(
  const G4ParticleDefinition* particleDefinition,
  const TParticle* particle) const
{
  if (!particleDefinition) {
    TString text =
      "TG4PrimaryGeneratorAction::CheckParticleDefinition() failed for ";
    text += TString(particle->GetName());
    text += "  pdgEncoding=";
    text += particle->GetPdgCode();
    text += ".";
    if (fSkipUnknownParticles) {
      TG4Globals::Warning(
        "TG4PrimaryGeneratorAction", "CheckParticleDefinition", text);
    }
    else {
      TG4Globals::Exception(
        "TG4PrimaryGeneratorAction", "CheckParticleDefinition", text);
    }
    return false;
  }
  return true;
}

//_____________________________________________________________________________
G4double TG4PrimaryGeneratorAction::GetProperCharge(
  const G4ParticleDefinition* particleDefinition,
  const TParticle* particle) const
{
  G4double charge = particleDefinition->GetPDGCharge();
  if (G4IonTable::IsIon(particleDefinition) &&
      particleDefinition->GetParticleName() != "proton") {
    // Get dynamic charge defined by user
    TG4UserIon* userIon =
      fParticlesManager->GetUserIon(particle->GetName(), false);
    if (userIon) charge = userIon->GetQ() * eplus;
  }
  return charge;
}

//_____________________________________________________________________________
G4PrimaryVertex* TG4PrimaryGeneratorAction::AddParticleToVertex(G4Event* event,
  G4PrimaryVertex* vertex, const G4ParticleDefinition* particleDefinition,
  const G4ThreeVector& position, G4double time, const G4ThreeVector& momentum,
  G4double energy, const G4ThreeVector& polarization, G4double charge,
  G4double weight) const
{
  auto thisVertex = vertex;
  auto primaryParticle = new G4PrimaryParticle(
    particleDefinition, momentum.x(), momentum.y(), momentum.z(), energy);
  primaryParticle->SetCharge(charge);
  primaryParticle->SetWeight(weight);
  primaryParticle->SetPolarization(polarization);
  if (vertex == 0 || vertex->GetPosition() != position ||
      time != vertex->GetT0()) {
    // Create a new vertex in case position and time of particle are different
    // from previous values
    thisVertex = new G4PrimaryVertex(position, time);
    event->AddPrimaryVertex(thisVertex);
  }
  thisVertex->SetPrimary(primaryParticle);
  // Verbose
  if (VerboseLevel() > 1) {
    G4cout << "Add primary particle to vertex: " << G4endl;
    primaryParticle->Print();
  }
  return thisVertex;
}

//_____________________________________________________________________________
void TG4PrimaryGeneratorAction::TransformPrimaries(G4Event* event)
{
  /// Create a new G4PrimaryVertex objects for each TParticle
  /// in the VMC stack.

  CheckVMCStack(fMCStack);

  G4int nofParticles = fMCStack->GetNtrack();

  if (VerboseLevel() > 1)
    G4cout << "TG4PrimaryGeneratorAction::TransformPrimaries: " << nofParticles
           << " particles" << G4endl;

  G4PrimaryVertex* previousVertex = 0;

  for (G4int i = 0; i < nofParticles; i++) {

    // get the particle from the stack
    TParticle* particle = fMCStack->PopPrimaryForTracking(i);

    if (particle) {

      // Pass this particle Id (in the VMC stack) to Track manager
      fTrackManager->AddPrimaryParticleId(i);

      // Get particle definition from TG4ParticlesManager
      G4ParticleDefinition* particleDefinition =
        fParticlesManager->GetParticleDefinition(particle, false);

      if (!CheckParticleDefinition(particleDefinition, particle)) {
        continue;
      }

      // Current particle's position and time
      G4ThreeVector position = fParticlesManager->GetParticlePosition(particle);
      G4double time = particle->T() * TG4G3Units::Time();

      // Current particle's momentum and energy
      G4ThreeVector momentum = fParticlesManager->GetParticleMomentum(particle);
      G4double energy = particle->Energy() * TG4G3Units::Energy();

      // Particle's charge,  weight and polarization
      G4double charge = GetProperCharge(particleDefinition, particle);
      G4double weight = particle->GetWeight();
      TVector3 polarization;
      particle->GetPolarisation(polarization);
      G4ThreeVector g4Polarization(
        polarization.X(), polarization.Y(), polarization.Z());

      // Create new G4PrimaryParticle and add to G4PrimaryVertex.
      previousVertex =
        AddParticleToVertex(event, previousVertex, particleDefinition, position,
          time, momentum, energy, g4Polarization, charge, weight);
    }
  }
}

//_____________________________________________________________________________
void TG4PrimaryGeneratorAction::TransformTracks(G4Event* event)
{
  /// Create a new G4PrimaryVertex objects for each TParticle
  /// in the VMC stack.

  // The TMCManagerStack has additional info on the current track status,
  // e.g. kinematics and geometry state.
  // Calling TG4PrimaryGeneratorAction::TransformTracks implies that
  // it exists, it is the same as the TVirtualMCStack above

  CheckVMCStack(fMCManagerStack);

  if (VerboseLevel() > 1)
    G4cout << "TG4PrimaryGeneratorAction::TransformTracks: "
           << fMCManagerStack->GetNtrack() << " particles" << G4endl;

  G4PrimaryVertex* previousVertex = 0;

  const TParticle* particle = 0;

  Int_t trackId = -1;

  while ((particle = fMCManagerStack->PopNextTrack(trackId))) {

    if (particle) {
      const TMCParticleStatus* particleStatus =
        fMCManagerStack->GetParticleStatus(trackId);

      // Pass current status of the particle to the trackManager containing
      // information about potential made steps and track length != 0 in case
      // the track was transported before
      fTrackManager->AddParticleStatus(particleStatus);

      // Get particle definition from TG4ParticlesManager
      G4ParticleDefinition* particleDefinition =
        fParticlesManager->GetParticleDefinition(particle, false);

      if (!CheckParticleDefinition(particleDefinition, particle)) {
        continue;
      }

      // Current particle's position and time
      const TLorentzVector& particlePosition = particleStatus->fPosition;
      G4ThreeVector position(particlePosition.X() * TG4G3Units::Length(),
        particlePosition.Y() * TG4G3Units::Length(),
        particlePosition.Z() * TG4G3Units::Length());
      G4double time = particlePosition.T() * TG4G3Units::Time();

      // Current particle's momentum and energy
      const TLorentzVector& particleMomentum = particleStatus->fMomentum;
      G4ThreeVector momentum(particleMomentum.Px() * TG4G3Units::Energy(),
        particleMomentum.Py() * TG4G3Units::Energy(),
        particleMomentum.Pz() * TG4G3Units::Energy());
      G4double energy = particleMomentum.Energy() * TG4G3Units::Energy();

      // Particle's charge,  weight and polarization
      G4double charge = GetProperCharge(particleDefinition, particle);
      G4double weight = particleStatus->fWeight;
      const TVector3& polarization = particleStatus->fPolarization;
      G4ThreeVector g4Polarization(
        polarization.X(), polarization.Y(), polarization.Z());

      // Create new G4PrimaryParticle and add to G4PrimaryVertex.
      previousVertex =
        AddParticleToVertex(event, previousVertex, particleDefinition, position,
          time, momentum, energy, g4Polarization, charge, weight);
    }
  }
}

//
// public methods
//

//_____________________________________________________________________________
void TG4PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  /// Generate primary particles by the selected generator.

  // Cache pointers to thread-local objects
  TG4RunManager* runManager = TG4RunManager::Instance();
  TG4EventAction* eventAction = GetEventAction();
  TVirtualMCApplication* mcApplication = TVirtualMCApplication::Instance();
  TVirtualMC* vmc = gMC;

  // Begin of event
  TG4StateManager::Instance()->SetNewState(kInEvent);

  if (!fCached) {
    fParticlesManager = TG4ParticlesManager::Instance();
    fTrackManager = TG4TrackManager::Instance();
    fMCManagerStack = vmc->GetManagerStack();
    fCached = true;
  }

  // If TG4RunManager::IsInterruptibleEvent(), rely on BeginEvent() has been
  // called already.
  if (!eventAction->IsInterruptibleEvent()) {
    mcApplication->BeginEvent();
  }

  // Update cached pointer to MC stack which can changed in some applications
  // in MCApplication::BeginEvent()
  fMCStack = vmc->GetStack();
  runManager->CacheMCStack();

  // Clear cached VMC stack info from former event
  fTrackManager->ResetPrimaryParticleIds();
  fTrackManager->ResetParticlesStatus();

  // Don't generate primaries if this is a complex interruptible event
  if (!fMCManagerStack) {
    // Generate primaries and fill the VMC stack
    mcApplication->GeneratePrimaries();
    TransformPrimaries(event);
  }
  else {
    TransformTracks(event);
  }
}
