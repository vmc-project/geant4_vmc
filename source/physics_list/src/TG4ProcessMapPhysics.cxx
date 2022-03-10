//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2022 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessMapPhysics.cxx
/// \brief Implementation of the TG4ProcessMapPhysics class
///
/// \author I. Hrivnacova; IJCLab, Orsay

#include "TG4ProcessMapPhysics.h"
#include "TG4Globals.h"
#include "TG4ProcessMap.h"
#include "TG4StackPopper.h"

#include <TVirtualMC.h>
#include <TVirtualMCDecayer.h>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4Version.hh>

// G4 process code headers
#include <G4DecayProcessType.hh>
#include <G4EmProcessSubType.hh>
#include <G4FastSimulationProcessType.hh>
#include <G4HadronicProcessType.hh>
#include <G4OpProcessSubType.hh>
#include <G4ProcessType.hh>
#include <G4TransportationProcessType.hh>

#include <set>
#include <map>

//_____________________________________________________________________________
TG4ProcessMapPhysics::TG4ProcessMapPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name)
{
  /// Standard constructor

  FillMap();
}

//_____________________________________________________________________________
TG4ProcessMapPhysics::TG4ProcessMapPhysics(G4int theVerboseLevel,
  const G4String& name)
  : TG4VPhysicsConstructor(name, theVerboseLevel)
{
  /// Standard constructor

  FillMap();
}

//_____________________________________________________________________________
TG4ProcessMapPhysics::~TG4ProcessMapPhysics()
{
  /// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4ProcessMapPhysics::FillMap()
{
  /// Fill the process codes map with known G4 process names

  TG4ProcessMap* pMap = TG4ProcessMap::Instance();

  // clang-format off
  // G4EmProcessSubType: 1 - 25
  pMap->Add(fCoulombScattering, kPCoulombScattering, kMULS);      // G4 value:  1
  pMap->Add(fIonisation, kPEnergyLoss, kG3LOSS);                  // G4 value:  2
  pMap->Add(fBremsstrahlung, kPBrem, kBREM );                     // G4 value:  3
  pMap->Add(fPairProdByCharged, kPPair, kPAIR);                   // G4 value:  4
  pMap->Add(fAnnihilation, kPAnnihilation, kANNI);                // G4 value:  5
  pMap->Add(fAnnihilationToMuMu, kPAnnihilation, kANNI);          // G4 value:  6
    // Add code
  pMap->Add(fAnnihilationToHadrons, kPAnnihilation, kANNI);       // G4 value:  7
    // Add code
  pMap->Add(fNuclearStopping, kPCoulombScattering, kMULS);        // G4 value:  8
    // CHECK ??
  pMap->Add(fElectronGeneralProcess, kPNull, kNoG3Controls);      // G4 value:  9

  pMap->Add(fMultipleScattering, kPMultipleScattering, kMULS);    // G4 value: 10

  pMap->Add(fRayleigh, kPRayleigh, kRAYL);                        // G4 value: 11
  pMap->Add(fPhotoElectricEffect, kPPhotoelectric, kPHOT);        // G4 value: 12
  pMap->Add(fComptonScattering, kPCompton, kCOMP);                // G4 value: 13
  pMap->Add(fGammaConversion, kPPair, kPAIR);                     // G4 value: 14
  pMap->Add(fGammaConversionToMuMu, kPPair, kPAIR);               // G4 value: 15
    // Add code
  pMap->Add(fGammaGeneralProcess, kPNull, kNoG3Controls);          // G4 value: 16
  pMap->Add(fPositronGeneralProcess, kPNull, kNoG3Controls);       // G4 value: 17
  pMap->Add(fAnnihilationToTauTau, kPAnnihilation, kANNI);         // G4 value: 18
    // Add code

  pMap->Add(fCerenkov, kPCerenkov, kCKOV);                         // G4 value: 21
  pMap->Add(fScintillation, kPScintillation, kNoG3Controls);       // G4 value: 22
  pMap->Add(fSynchrotronRadiation, kPSynchrotron, kSYNC);          // G4 value: 23
  pMap->Add(fTransitionRadiation, kPTransitionRadiation, kNoG3Controls); // G4 value: 24

  pMap->Add(fSurfaceReflection, kPNull, kNoG3Controls);            // G4 value: 25
     // low energy G4MicroElecSurface process

  // G4OpProcessSubType: 31 - 36
  pMap->Add(fOpAbsorption, kPLightAbsorption, kLABS);              // G4 value: 31
  pMap->Add(fOpBoundary, kPLightScattering, kLABS);                // G4 value: 32
  pMap->Add(fOpRayleigh, kPRayleigh, kRAYL);                       // G4 value: 33
  pMap->Add(fOpWLS, kPLightWLShifting, kNoG3Controls);             // G4 value: 34
  pMap->Add(fOpMieHG, kPLightScattering, kNoG3Controls);           // G4 value: 35
    // Add code
  pMap->Add(fOpWLS2, kPLightWLShifting, kNoG3Controls);            // G4 value: 36
    // Add code

  // G4HadronicProcessType: 111 - 161; 210; 310
  pMap->Add(fHadronElastic, kPHElastic, kHADR);                    // G4 value: 111
  pMap->Add(fHadronInelastic, kPHInhelastic, kHADR);               // G4 value: 121
  pMap->Add(fCapture, kPNCapture, kHADR);                          // G4 value: 131
  pMap->Add(fMuAtomicCapture, kPHadronic, kHADR);                  // G4 value: 132
    // Add code
    // was: kPNCapture, kHADR
  pMap->Add(fFission, kPNuclearFission, kHADR);                    // G4 value: 141
  pMap->Add(fHadronAtRest, kPHadronic, kHADR);                     // G4 value: 151
    // Add code - G4HadronStoppingProcess, "hadronCaptureAtRest"
    // was: kPNCapture, kHADR
  pMap->Add(fLeptonAtRest, kPHadronic, kHADR);                     // G4 value: 152
    // No process defined with this code
  pMap->Add(fChargeExchange, kPHadronic, kHADR);                   // G4 value: 161
    // Not present in G4 physics lists (?)
  pMap->Add(fRadioactiveDecay, kPDecay, kDCAY);                    // G4 value: 210
  pMap->Add(fEMDissociation, kPHadronic, kHADR);                   // G4 value: 310
    // No process defined with this code

  // TG4HadronicProcessType: 171 - 174
  pMap->Add(fElectronNuclear, kPElectronNuclear, kHADR);           // TG4 value: 171
  pMap->Add(fPositronNuclear, kPPositronNuclear, kHADR);           // TG4 value: 172
  pMap->Add(fMuonNuclear, kPMuonNuclear, kMUNU);                   // TG4 value: 173
  pMap->Add(fPhotoNuclear, kPPhotoNuclear, kHADR);                 // TG4 value: 174

  // G4DecayProcessType: 201 - 231
  pMap->Add(DECAY, kPDecay, kDCAY);                                // G4 value: 201
  pMap->Add(DECAY_WithSpin, kPDecay, kDCAY);                       // G4 value: 202
  pMap->Add(DECAY_PionMakeSpin, kPDecay, kDCAY);                   // G4 value: 203
  // DECAY_Radioactive ( G4 value: 210) - already added with G4HadronicProcessType
  pMap->Add(DECAY_Unknown, kPDecay, kDCAY);                        // G4 value: 211
  pMap->Add(DECAY_MuAtom, kPDecay, kDCAY);                         // G4 value: 221
  pMap->Add(DECAY_External, kPDecay, kDCAY);                       // G4 value: 231

  // G4TransportationProcessType: 91, 92
  pMap->Add(TRANSPORTATION, kPTransportation, kNoG3Controls);         // G4 value: 91
  pMap->Add(COUPLED_TRANSPORTATION, kPTransportation, kNoG3Controls); // G4 value: 92

  // G4FastSimulationProcessType: 301
  pMap->Add(FASTSIM_ManagerProcess, kPNull, kNoG3Controls);        // G4 value: 301

  // G4TransportationProcessType: 401 - 491
  // following processes belong to 'General' type
  pMap->Add(STEP_LIMITER, kStepMax, kNoG3Controls);                // G4 value: 401
  pMap->Add(USER_SPECIAL_CUTS, kPStop, kNoG3Controls);             // G4 value: 402
  pMap->Add(NEUTRON_KILLER, kPStop, kNoG3Controls);                // G4 value: 403
    // was kPHadronic, kHADR
  pMap->Add(PARALLEL_WORLD_PROCESS, kPNull, kNoG3Controls);        // G4 value: 491

  // TG4BiasingProcessType: 501
  pMap->Add(fBiasing, kPNull, kNoG3Controls);                      // TG4 value: 501

  // TG4StackPopperProcessType: 601
  pMap->Add(fStackPopper, kPNull, kNoG3Controls);                  // TG4 value: 601
}

//_____________________________________________________________________________
void TG4ProcessMapPhysics::UpdateHadronicProcessSubType(G4VProcess* process)
{
  /// Update process sub type codes for hadronic processes
  /// which are not set in Geant4 itself


  auto processName = process->GetProcessName();

  if (! G4StrUtil::contains(processName, "Nuclear")) return;

  std::set<std::pair<G4String, G4int>> newCodes =
    {{ "electron", fElectronNuclear },
     { "positron", fPositronNuclear },
     { "muon", fMuonNuclear },
     { "mu-", fMuonNuclear },
     { "photon", fPhotoNuclear }};

  for ( auto newCode : newCodes) {
    if ( G4StrUtil::contains(processName, newCode.first)) {
      process->SetProcessSubType(newCode.second);
      return;
    }
  }
}

//_____________________________________________________________________________
G4bool TG4ProcessMapPhysics::UpdateOrSkipBiasingProcess(G4VProcess* process)
{
  /// Skip a biasing wrapper process, if it has not defined sub type

  // Do nothing if process has defined sub type
  if (process->GetProcessSubType() > 0) return false;

  auto processName = process->GetProcessName();

  if (G4StrUtil::contains(processName, "biasLimiter")) {
    process->SetProcessSubType(STEP_LIMITER);
    return false;
  }

  if (G4StrUtil::contains(processName, "biasWrapper")) {
    // do not update sub type and skip this process evaluation
    return true;
  }

  return false;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4ProcessMapPhysics::ConstructParticle()
{
  /// Instantiate particles - nothing to be done here
}

//_____________________________________________________________________________
void TG4ProcessMapPhysics::ConstructProcess()
{
  /// Loop over all particles and their processes and check if
  /// the process is present in the map

  if (VerboseLevel() > 1) {
    G4cout << "TG4ProcessMapPhysics::ConstructProcess: " << G4endl;
  }

  auto pMap = TG4ProcessMap::Instance();
  G4bool success = true;

  auto aParticleIterator = GetParticleIterator();
  aParticleIterator->reset();
  while ((*aParticleIterator)()) {
    // skip iteration if particle does not have a process manager
    if (!aParticleIterator->value()->GetProcessManager()) continue;

    auto processVector =
      aParticleIterator->value()->GetProcessManager()->GetProcessList();

    for (size_t i = 0; i < processVector->length(); i++) {

      if (UpdateOrSkipBiasingProcess((*processVector)[i])) continue;

      if ((*processVector)[i]->GetProcessSubType() == fHadronInelastic) {
        UpdateHadronicProcessSubType((*processVector)[i]);
      }

      auto newCodes = pMap->GetCodes((*processVector)[i]);
      if (newCodes.first == kPNoProcess) {
        G4String text = "Unknown process codes for ";
        text += (*processVector)[i]->GetProcessName();
        text += " process type: ";
        text += std::to_string((*processVector)[i]->GetProcessType());
        text += " process sub type: ";
        text += std::to_string((*processVector)[i]->GetProcessSubType());
        TG4Globals::Warning("TG4ProcessMapPhysics", "ConstructProcess", text);
        success = false;
      }
    }
  }

  if (VerboseLevel() > 0) {
    if (success)
      G4cout << "### Processes mapped to VMC codes ok" << G4endl;
    else
      G4cout << "### Processes mapped to VMC codes: some proceesses unknown."
             << G4endl;
  }
}
