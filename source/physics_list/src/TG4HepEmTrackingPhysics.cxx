//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2025 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4HepEmTrackingPhysics.cxx
/// \brief Implementation of the TG4HepEmTrackingPhysics class

#ifdef USE_G4HEPEM

#include "TG4HepEmTrackingPhysics.h"

#include <G4HepEmConfig.hh>
#include <G4HepEmTrackingManager.hh>

#include <G4BuilderType.hh>
#include <G4Electron.hh>
#include <G4EmBuilder.hh>
#include <G4EmModelActivator.hh>
#include <G4EmParameters.hh>
#include <G4Version.hh>
#include <G4Gamma.hh>
#include <G4GenericIon.hh>
#include <G4PhysicsListHelper.hh>
#include <G4Positron.hh>
#include <G4SystemOfUnits.hh>
#include <G4hMultipleScattering.hh>
#include <G4ionIonisation.hh>

//_____________________________________________________________________________
TG4HepEmTrackingPhysics::TG4HepEmTrackingPhysics(G4int verbose)
  : G4VPhysicsConstructor("G4EmStandard_opt1")
{
  // the EM parameters of G4EmStandardPhysics_option1;
  // G4HepEm takes its e-/e+ defaults from them
  SetVerboseLevel(verbose);
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetVerbose(verbose);
  param->SetApplyCuts(true);
  param->SetStepFunction(0.8, 1 * CLHEP::mm);
  param->SetMscRangeFactor(0.2);
  param->SetMscStepLimitType(fMinimal);
  SetPhysicsType(bElectromagnetic);
}

//_____________________________________________________________________________
void TG4HepEmTrackingPhysics::ConstructParticle()
{
  G4EmBuilder::ConstructMinimalEmSet();
}

//_____________________________________________________________________________
void TG4HepEmTrackingPhysics::ConstructProcess()
{
  // G4HepEm transports e-, e+ and gamma
  auto trackingManager = new G4HepEmTrackingManager(verboseLevel);
  // the e-/e+ step function of option 1, which G4HepEm does not take from G4EmParameters
  trackingManager->GetConfig()->SetEnergyLossStepLimitFunctionParameters(0.8, 1 * CLHEP::mm);
  // several msc steps per transportation step only if requested, as in Geant4
  // (/process/em/transportationWithMsc)
#if G4VERSION_NUMBER >= 1110
  trackingManager->GetConfig()->SetMultipleStepsInMSCWithTransportation(
    G4EmParameters::Instance()->TransportationWithMsc() ==
    G4TransportationWithMscType::fMultipleSteps);
#else
  trackingManager->GetConfig()->SetMultipleStepsInMSCWithTransportation(false);
#endif
  G4Electron::Definition()->SetTrackingManager(trackingManager);
  G4Positron::Definition()->SetTrackingManager(trackingManager);
  G4Gamma::Definition()->SetTrackingManager(trackingManager);

  // the other particles as in G4EmStandardPhysics_option1
  G4EmBuilder::PrepareEMPhysics();
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  G4hMultipleScattering* hmsc = new G4hMultipleScattering("ionmsc");
  G4ParticleDefinition* particle = G4GenericIon::GenericIon();
  ph->RegisterProcess(hmsc, particle);
  ph->RegisterProcess(new G4ionIonisation(), particle);

  G4EmBuilder::ConstructCharged(hmsc, nullptr);

  G4EmModelActivator mact(GetPhysicsName());

  if (verboseLevel > 0) {
    G4cout << "### TG4HepEmTrackingPhysics: G4HepEm for e-, e+, gamma; "
           << "EM option 1 for other particles" << G4endl;
  }
}

#endif
