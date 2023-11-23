//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ComposedPhysicsList.cxx
/// \brief Implementation of the TG4ComposedPhysicsList class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ComposedPhysicsList.h"
#include "TG4G3PhysicsManager.h"
#include "TG4PhysicsManager.h"
#include "TG4ProcessMapPhysics.h"

#include <G4Electron.hh>
#include <G4EmParameters.hh>
#include <G4Gamma.hh>
#include <G4GammaConversionToMuons.hh>
#include <G4HadronicProcess.hh>
#include <G4PhysicsListHelper.hh>
#include <G4Positron.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessTable.hh>
#include <G4Proton.hh>
#include <G4RegionStore.hh>
#include <G4SystemOfUnits.hh>
#include <G4Transportation.hh>
#include <G4VUserPhysicsList.hh>

namespace
{

G4Transportation* FindTransportation(
  const G4ParticleDefinition* particleDefinition)
{
  const auto* processManager = particleDefinition->GetProcessManager();
  return dynamic_cast<G4Transportation*>(
    processManager->GetProcess("Transportation"));
}

G4String GetProcessTypeName(G4int hadronicProcessSubType)
{
  switch(hadronicProcessSubType) {
    // G4HadronicProcessType
    case fHadronElastic:   return "HadronElastic"; break;
    // Not yet in 10.0.px
    // case fNeutronGeneral:  return "NeutronGeneral"; break;
    case fHadronInelastic: return "HadronInelastic"; break;
    case fCapture:         return "Capture"; break;
    case fMuAtomicCapture: return "MuAtomicCapture"; break;
    case fFission:         return "Fission"; break;
    case fHadronAtRest:    return "HadronAtRest"; break;
    case fLeptonAtRest:    return "LeptonAtRest"; break;
    case fChargeExchange:  return "ChargeExchange"; break;
    // case fNuOscillation:   return "NuOscillation"; break;
    // case fNuElectron:      return "NuElectron"; break;
    // case fNuNucleus:       return "NuNucleus"; break;
    case fRadioactiveDecay:return "RadioactiveDecay"; break;
    case fEMDissociation:  return "EMDissociation"; break;
    //  TG4HadronicProcessType
    case fElectronNuclear: return "ElectronNuclear"; break;
    case fPositronNuclear: return "PositronNuclear"; break;
    case fMuonNuclear:     return "MuonNuclear"; break;
    case fPhotoNuclear:    return "PhotoNuclear"; break;
  }

  TString text;
  text += hadronicProcessSubType;
  TG4Globals::Warning(
    "TG4ComposedPhysicsList", "GetProcessTypeName:",
    "Unknown hadronic process type: " + text);
  G4cout << hadronicProcessSubType << G4endl;
  return "";
}

G4int GetProcessSubType(
  const G4String& processTypeName, G4int& errorCode)
{
  errorCode = 0;

  // G4HadronicProcessType
  if (processTypeName == "HadronElastic")    return fHadronElastic;
  // Not yet in 10.0.px
  // if (processTypeName == "NeutronGeneral")   return fNeutronGeneral;
  if (processTypeName == "HadronInelastic")  return fHadronInelastic;
  if (processTypeName == "Capture")          return fCapture;
  if (processTypeName == "MuAtomicCapture")  return fMuAtomicCapture;
  if (processTypeName == "Fission")          return fFission;
  if (processTypeName == "HadronAtRest")     return fHadronAtRest;
  if (processTypeName == "LeptonAtRest")     return fLeptonAtRest;
  if (processTypeName == "ChargeExchange")   return fChargeExchange;
  // if (processTypeName == "NuOscillation")    return fNuOscillation;
  // if (processTypeName == "NuElectron")       return fNuElectron;
  // if (processTypeName == "NuNucleus")        return fNuNucleus;
  if (processTypeName == "RadioactiveDecay") return fRadioactiveDecay;
  if (processTypeName == "EMDissociation")   return fEMDissociation;

  // TG4HadronicProcessType
  if (processTypeName == "ElectronNuclear")  return fElectronNuclear;
  if (processTypeName == "PositronNuclear")  return fPositronNuclear;
  if (processTypeName == "MuonNuclear")      return fMuonNuclear;
  if (processTypeName == "PhotoNuclear")     return fPhotoNuclear;

  TG4Globals::Warning(
    "TG4ComposedPhysicsList", "GetProcessSubType:",
    "Unknown process type name: " + TString(processTypeName.data()));
  errorCode = 1;
  return fHadronInelastic;
}

G4VProcess* GetProcess(G4ParticleDefinition* particle, G4int processSubType)
{
  // Find a process of given sub type;
  // print a warning if no process or more than one process exist

  auto processList = particle->GetProcessManager()->GetProcessList();
  std::vector<G4VProcess*> foundProcesses;
  for (std::size_t i = 0; i < processList->size(); ++i) {
    if ((*processList)[i]->GetProcessSubType() == processSubType) {
      foundProcesses.push_back((*processList)[i]);
    }
  }

  if (foundProcesses.empty()) {
    TString text;
    text += processSubType;
    TG4Globals::Warning(
      "TG4ComposedPhysicsList", "GetProcess:",
      "Unknown process sub type: " + text);
    return nullptr;
  }

  if (foundProcesses.size()>1) {
    TString text;
    text += processSubType;
    TG4Globals::Warning(
      "TG4ComposedPhysicsList", "GetProcess:",
      "More than one process of sub type: " + text + " exists.");
  }

  return foundProcesses[0];
}

} // namespace

const G4double TG4ComposedPhysicsList::fgkDefautLooperThresholdsLevel = 1;

//_____________________________________________________________________________
TG4ComposedPhysicsList::TG4ComposedPhysicsList()
  : G4VUserPhysicsList(),
    TG4Verbose("composedPhysicsList"),
    fMessenger(this),
    fPhysicsLists(),
    fIsProductionCutsTableEnergyRange(false),
    fProductionCutsTableEnergyMin(0.),
    fProductionCutsTableEnergyMax(0.),
    fGammaToMuonsCrossSectionFactor(-1.),
    fLooperThresholdsLevel(fgkDefautLooperThresholdsLevel)
{
  /// Default constructor

  if (VerboseLevel() > 1)
    G4cout << "TG4ComposedPhysicsList::TG4ComposedPhysicsList" << G4endl;

  SetVerboseLevel(TG4Verbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4ComposedPhysicsList::~TG4ComposedPhysicsList()
{
  /// Destructor

  for (G4int i = 0; i < G4int(fPhysicsLists.size()); i++)
    delete fPhysicsLists[i];
}

//
// private methods
//

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ApplyGammaToMuonsCrossSectionFactor()
{
  /// Apply gamma to muons cross section factor if defined by user
  /// if applicaple

  if (fGammaToMuonsCrossSectionFactor > 0.) {
    G4ParticleDefinition* gamma = G4Gamma::Gamma();
    G4ProcessManager* processManager = gamma->GetProcessManager();
    G4ProcessVector* processVector = processManager->GetProcessList();
    G4bool done = false;
    // get G4GammaConversionToMuons
    for (size_t i = 0; i < processVector->length(); i++) {
      G4GammaConversionToMuons* gammaToMuMu =
        dynamic_cast<G4GammaConversionToMuons*>((*processVector)[i]);
      if (gammaToMuMu) {
        if (VerboseLevel() > 0) {
          G4cout << "### Set gamma to muons cross section factor user value: "
                 << fGammaToMuonsCrossSectionFactor << G4endl;
        }
        gammaToMuMu->SetCrossSecFactor(fGammaToMuonsCrossSectionFactor);
        done = true;
        break;
      }
    }
    if (!done) {
      TG4Globals::Warning("TG4ComposedPhysicsList",
        "ApplyGammaToMuonsCrossSectionFactor",
        "Cannot set gamma to muons cross section factor, GammaToMuons must be "
        "activated first.");
    }
  }
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ApplyCrossSectionFactor(
  const G4String& particleName, const G4String& processDef, G4double factor,
  G4bool isProcessName)
{
  /// Apply the scale factor to a hadronic process cross section

  // get particle definition from G4ParticleTable
  auto particleDefinition =
    G4ParticleTable::GetParticleTable()->FindParticle(particleName);
  if (particleDefinition == nullptr) {
    TG4Globals::Warning("TG4ComposedPhysicsList", "ApplyCrossSectionFactor",
      "G4ParticleTable::FindParticle() for particle " +
      TString(particleName.data()) + " failed.");
    return;
  }

  G4VProcess* process = nullptr;
  if (isProcessName) {
    // get process by name
    process = particleDefinition->GetProcessManager()->GetProcess(processDef);
  }
  else {
    // get process by type
    G4int errorCode = 0;
    auto processSubType = GetProcessSubType(processDef, errorCode);
    if (errorCode > 0) {
      TG4Globals::Warning("TG4ComposedPhysicsList", "ApplyCrossSectionFactor",
        "GetProcessSubType for processTypeName " +
        TString(processDef.data()) + " failed.");
      return;
    }
    process = GetProcess(particleDefinition, processSubType);
       // Cannot use G4HadronicProcessStore::FindProcess
       // as we redefine some processes  sub-types with our codes
  }

  if (process == nullptr) {
    TString by  = (isProcessName) ? " by name" : " by type";
      TG4Globals::Warning("TG4ComposedPhysicsList", "ApplyCrossSectionFactor",
        "Get process " + TString(processDef.data()) + by + " failed.");
      return;
  }

  // check process type
  if (process->GetProcessType() != fHadronic) {
    TG4Globals::Warning("TG4ComposedPhysicsList", "ApplyCrossSectionFactor",
      "Process " + TString(processDef.data()) + " is not hadronic.");
    return;
  }

  // apply factor
  (static_cast<G4HadronicProcess*>(process))->MultiplyCrossSectionBy(factor);

  if (VerboseLevel() > 0) {
    G4cout
      << "### Applied cross section factor " << factor
      << " for " <<  particleDefinition->GetParticleName()
      << " process " << process->GetProcessName()
      << " process type: " << GetProcessTypeName(process->GetProcessSubType())
      << G4endl;
  }
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ApplyCrossSectionFactors()
{
  /// Apply the cross section factors registered in fCrossSectionFactors

  for (const auto& [particleName, processType, factor, isProcessName] :
      fCrossSectionFactors) {
    ApplyCrossSectionFactor(particleName, processType, factor, isProcessName);
  }
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetLooperThresholds()
{
  /// Apply the looper thresholds level
  auto plHelper = G4PhysicsListHelper::GetPhysicsListHelper();
  if (fLooperThresholdsLevel == 0) {
    if (VerboseLevel() > 0) {
      G4cout << "### Use low looper thresholds" << G4endl;
    }
    plHelper->UseLowLooperThresholds();
  }
  else if (fLooperThresholdsLevel == 1) {
    // Do nothing: Geant4 defaults
  }
  else if (fLooperThresholdsLevel == 2) {
    if (VerboseLevel() > 0) {
      G4cout << "### Use high looper thresholds" << G4endl;
    }
    plHelper->UseHighLooperThresholds();
  }
  else {
    TString message = "The level";
    message += fLooperThresholdsLevel;
    message += " is not supported (the value must be 0, 1 or 2.)";
    TG4Globals::Warning(
      "TG4ComposedPhysicsList", "SetPresetLooperThresholds", message);
  }
}

//
// public methods
//

//_____________________________________________________________________________
void TG4ComposedPhysicsList::AddPhysicsList(G4VUserPhysicsList* physicsList)
{
  /// Add physics list in the list

  fPhysicsLists.push_back(physicsList);
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ConstructParticle()
{
  /// Construct all particles.

  if (VerboseLevel() > 1)
    G4cout << "TG4ComposedPhysicsList::ConstructParticle" << G4endl;

  for (G4int i = 0; i < G4int(fPhysicsLists.size()); i++)
    fPhysicsLists[i]->ConstructParticle();

  if (VerboseLevel() > 1)
    G4cout << "TG4ComposedPhysicsList::ConstructParticle done" << G4endl;
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ConstructProcess()
{
  /// Construct all processes.

  if (VerboseLevel() > 1)
    G4cout << "TG4ComposedPhysicsList::ConstructProcess" << G4endl;

  // lock physics manager
  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  g3PhysicsManager->Lock();

  // set looper thresholds level
  SetLooperThresholds();

  for (G4int i = 0; i < G4int(fPhysicsLists.size()); i++) {
    fPhysicsLists[i]->ConstructProcess();
  }

  if (fGammaToMuonsCrossSectionFactor > 0.) {
    ApplyGammaToMuonsCrossSectionFactor();
  }

  if (!fCrossSectionFactors.empty()) {
    ApplyCrossSectionFactors();
  }

  if (VerboseLevel() > 1)
    if (fLooperThresholdsLevel != fgkDefautLooperThresholdsLevel) {
      // Print looper thresholds
      auto transportation = FindTransportation(G4Electron::Electron());
      if (transportation) {
        transportation->ReportLooperThresholds();
      }
    }

  G4cout << "TG4ComposedPhysicsList::ConstructProcess done" << G4endl;
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCuts()
{
  /// Set cut values

  if (VerboseLevel() > 1) G4cout << "TG4ComposedPhysicsList::SetCuts" << G4endl;

  if (fIsProductionCutsTableEnergyRange) {
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(
      fProductionCutsTableEnergyMin, fProductionCutsTableEnergyMax);
  }

  // Get default range cut values from physics manager
  G4double rangeCutGam = TG4PhysicsManager::Instance()->GetCutForGamma();
  G4double rangeCutEle = TG4PhysicsManager::Instance()->GetCutForElectron();
  G4double rangeCutPos = TG4PhysicsManager::Instance()->GetCutForPositron();
  G4double rangeCutPro = TG4PhysicsManager::Instance()->GetCutForProton();

  // Set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(rangeCutGam, "gamma");
  SetCutValue(rangeCutEle, "e-");
  SetCutValue(rangeCutPos, "e+");
  SetCutValue(rangeCutPro, "proton");

  // Set the default production cuts to all already created regions.
  // The cuts can be then further customized via VMC cuts or
  // via Geant4 UI commands called after G4RunManager initialization
  // (in VMC examples in g4config2.in macros)

  // Loop over regions
  G4RegionStore* regionStore = G4RegionStore::GetInstance();
  for (G4int i = 0; i < G4int(regionStore->size()); i++) {
    G4Region* region = (*regionStore)[i];

    // skip regions which already have production cuts defined
    if (region->GetProductionCuts()) continue;

    G4ProductionCuts* cuts = new G4ProductionCuts();
    cuts->SetProductionCut(rangeCutGam, 0);
    cuts->SetProductionCut(rangeCutEle, 1);
    cuts->SetProductionCut(rangeCutPos, 2);
    cuts->SetProductionCut(rangeCutPro, 3);
    region->SetProductionCuts(cuts);
  }

  if (VerboseLevel() > 0) DumpCutValuesTable();
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForGamma(G4double cut)
{
  /// Set cut value for gamma

  SetParticleCuts(cut, G4Gamma::Gamma());
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForElectron(G4double cut)
{
  /// Set cut value for electron

  SetParticleCuts(cut, G4Electron::Electron());
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForPositron(G4double cut)
{
  /// Set cut value for positron

  SetParticleCuts(cut, G4Positron::Positron());
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForProton(G4double cut)
{
  /// Set cut value for positron

  SetParticleCuts(cut, G4Proton::Proton());
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetProductionCutsTableEnergyRange(
  G4double min, G4double max)
{
  /// Set the production cuts table energy range

  fProductionCutsTableEnergyMin = min;
  fProductionCutsTableEnergyMax = max;
  fIsProductionCutsTableEnergyRange = true;
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCrossSectionFactor(
  const G4String& particleName, const G4String& processTypeOrName,
  G4double factor, G4bool isProcessName)
{
  /// Define a scale factor for a hadronic process defined
  /// by its name or its type name (depending on the 'isProcessName' setting)
  /// for the particle with the given name.

  fCrossSectionFactors.push_back(
    std::make_tuple(particleName, processTypeOrName, factor, isProcessName));
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::PrintAllProcesses() const
{
  /// Print all processes.

  G4cout << "Instantiated processes: " << G4endl;
  G4cout << "======================= " << G4endl;

  G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
  G4ProcessTable::G4ProcNameVector* processNameList =
    processTable->GetNameList();

  for (G4int i = 0; i < G4int(processNameList->size()); i++) {
    G4cout << "   " << (*processNameList)[i] << G4endl;
  }
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::DumpAllProcesses() const
{
  /// Dump all particles and their processes.

  G4cout << "Instantiated particles and processes: " << G4endl;
  G4cout << "===================================== " << G4endl;

  auto theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();
  while ((*theParticleIterator)()) {
    // print particle name
    G4cout << "Particle: " << theParticleIterator->value()->GetParticleName()
           << G4endl;

    // dump particle processes
    G4ProcessVector* processVector =
      theParticleIterator->value()->GetProcessManager()->GetProcessList();
    for (size_t i = 0; i < processVector->length(); i++)
      (*processVector)[i]->DumpInfo();

    G4cout << G4endl;
  }
}

//_____________________________________________________________________________
G4int TG4ComposedPhysicsList::VerboseLevel() const
{
  /// Return verbose level (via TG4Verbose)

  return TG4Verbose::VerboseLevel();
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::VerboseLevel(G4int level)
{
  /// Set the specified level to both TG4Verbose and
  /// G4VUserPhysicsList.
  /// The verbose level is also propagated to registered physics lists.

  TG4Verbose::VerboseLevel(level);
  SetVerboseLevel(level);

  for (G4int i = 0; i < G4int(fPhysicsLists.size()); i++) {
    TG4Verbose* verbose = dynamic_cast<TG4Verbose*>(fPhysicsLists[i]);
    if (verbose)
      verbose->VerboseLevel(level);
    else
      fPhysicsLists[i]->SetVerboseLevel(level);
  }
}
