//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4PhysicsManager.cxx
/// \brief Implementation of the TG4PhysicsManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4PhysicsManager.h"
#include "TG4G3Control.h"
#include "TG4G3Cut.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3Units.h"
#include "TG4GeometryServices.h"
#include "TG4Limits.h"
#include "TG4Medium.h"
#include "TG4MediumMap.h"
#include "TG4ParticlesManager.h"
#include "TG4ProcessMap.h"
#include "TG4SpecialPhysicsList.h"
#include "TG4StateManager.h"

#include <G4OpBoundaryProcess.hh>
#include <G4OpProcessSubType.hh>
#include <G4OpticalPhoton.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessTable.hh>
#include <G4TransportationProcessType.hh>
#include <G4VProcess.hh>
#include <G4VUserPhysicsList.hh>
#include <G4Version.hh>

#if G4VERSION_NUMBER == 1100
// Temporary work-around for bug in Cerenkov
#include <G4Cerenkov.hh>
#include <G4Electron.hh>
#include <G4OpticalParameters.hh>
#endif

#include <TDatabasePDG.h>
#include <TVirtualMCApplication.h>

// Moved after Root includes to avoid shadowed variables
// generated from short units names
#include <G4SystemOfUnits.hh>

TG4PhysicsManager* TG4PhysicsManager::fgInstance = 0;
const G4double TG4PhysicsManager::fgkDefautCut = 1. * mm;
TG4ProcessMap* TG4PhysicsManager::fgProcessMap = 0;

//_____________________________________________________________________________
TG4PhysicsManager::TG4PhysicsManager()
  : TG4Verbose("physicsManager"),
    fParticlesManager(0),
    fG3PhysicsManager(0),
    fNotImplParNames(),
    fCutForGamma(fgkDefautCut),
    fCutForElectron(fgkDefautCut),
    fCutForPositron(fgkDefautCut),
    fCutForProton(fgkDefautCut),
    fOpBoundaryProcess(0)
{
  /// Default constructor

  if (fgInstance) {
    TG4Globals::Exception("TG4PhysicsManager", "TG4PhysicsManager",
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;

  G4bool isMaster = !G4Threading::IsWorkerThread();
  if (isMaster) {
    fgProcessMap = new TG4ProcessMap();
  }

  // create particles manager
  fParticlesManager = new TG4ParticlesManager();

  // create G3 physics manager
  fG3PhysicsManager = new TG4G3PhysicsManager();

  // fill process name map
  // FillProcessMap();
}

//_____________________________________________________________________________
TG4PhysicsManager::~TG4PhysicsManager()
{
  /// Destructor

  fgInstance = 0;
  G4bool isMaster = !G4Threading::IsWorkerThread();
  if (isMaster) {
    delete fgProcessMap;
    fgProcessMap = 0;
  }
  delete fParticlesManager;
  delete fG3PhysicsManager;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4PhysicsManager::GstparCut(G4int itmed, TG4G3Cut par, G4double parval)
{
  /// Set special tracking medium parameter.
  /// It is applied to all logical volumes that use the specified
  /// tracking medium.

  // get medium from the map
  TG4Medium* medium =
    TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(itmed);
  if (!medium) {
    // TG4GeometryServices::Instance()->GetMediumMap()->Print();
    TString text = "mediumId=";
    text += itmed;
    TG4Globals::Warning(
      "TG4PhysicsManager", "GstparCut", "Medium with " + text + " not found.");
    return;
  }

  // get/create user limits
  TG4Limits* limits = TG4GeometryServices::Instance()->GetLimits(
    medium->GetLimits(), *fG3PhysicsManager->GetCutVector(),
    *fG3PhysicsManager->GetControlVector());

  if (!limits) {
    limits = new TG4Limits(*fG3PhysicsManager->GetCutVector(),
      *fG3PhysicsManager->GetControlVector());
    if (VerboseLevel() > 1) {
      G4cout << "TG4PhysicsManager::GstparCut: new TG4Limits() for medium "
             << itmed << " has been created." << G4endl;
    }
  }

  // set new limits object to medium
  medium->SetLimits(limits);

  // add units
  if (par == kTOFMAX)
    parval *= TG4G3Units::Time();
  else
    parval *= TG4G3Units::Energy();

  // set parameter
  limits->SetG3Cut(par, parval);
}

//_____________________________________________________________________________
void TG4PhysicsManager::GstparControl(
  G4int itmed, TG4G3Control par, TG4G3ControlValue parval)
{
  /// Set special tracking medium parameter.
  /// It is applied to all logical volumes that use the specified
  /// tracking medium.

  // get medium from the map
  TG4Medium* medium =
    TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(itmed);
  if (!medium) {
    // TG4GeometryServices::Instance()->GetMediumMap()->Print();
    TString text = "mediumId=";
    text += itmed;
    TG4Globals::Warning("TG4PhysicsManager", "GstparControl",
      "Medium with " + text + " not found.");
    return;
  }

  // get/create user limits
  TG4Limits* limits = TG4GeometryServices::Instance()->GetLimits(
    medium->GetLimits(), *fG3PhysicsManager->GetCutVector(),
    *fG3PhysicsManager->GetControlVector());

  if (!limits) {
    limits = new TG4Limits(*fG3PhysicsManager->GetCutVector(),
      *fG3PhysicsManager->GetControlVector());

    if (VerboseLevel() > 1) {
      G4cout << "TG4PhysicsManager::GstparControl: new TG4Limits() for medium"
             << itmed << " has been created." << G4endl;
    }
  }

  // set new limits object to medium
  medium->SetLimits(limits);

  // set parameter
  limits->SetG3Control(par, parval);
}

//_____________________________________________________________________________
G4ParticleDefinition* TG4PhysicsManager::GetParticleDefinition(
  G4int pdgEncoding) const
{
  /// Return G4 particle definition for given PDG encoding.

  G4ParticleDefinition* particleDefinition =
    G4ParticleTable::GetParticleTable()->FindParticle(pdgEncoding);

  if (!particleDefinition) {
    TString text = "PDG=";
    text += pdgEncoding;
    TG4Globals::Warning("TG4ParticlesManager", "GetParticleDefinition",
      "Particle with " + text + " not found.");
  }

  return particleDefinition;
}

//_____________________________________________________________________________
G4VProcess* TG4PhysicsManager::GetProcess(
  G4ProcessManager* processManager, G4int subType) const
{
  /// Find the process in the particle process manager which name contains
  /// subName

  auto processVector = processManager->GetProcessList();
  for (size_t i = 0; i < processVector->length(); ++i) {
    if ((*processVector)[i]->GetProcessSubType() == subType) {
      return (*processVector)[i];
    }
  }
  return nullptr;
}

//_____________________________________________________________________________
void TG4PhysicsManager::SetProcessActivation(
  G4ProcessManager* processManager, G4int processId, G4bool activation)
{
  /// Set process activation for the given process.

  G4String strActivation = "Activate   ";
  if (!activation) strActivation = "Inactivate ";

  if (TG4Verbose::VerboseLevel() > 1) {
    G4cout << strActivation << " process "
           << (*processManager->GetProcessList())[processId]->GetProcessName()
           << " for " << processManager->GetParticleType()->GetParticleName()
           << G4endl;
  }

  processManager->SetProcessActivation(processId, activation);
}

//_____________________________________________________________________________
void TG4PhysicsManager::SetSpecialControlsActivation()
{
  /// (In)Activate built processes according
  /// to the setup in TG4G3PhysicsManager::fControlVector.

  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();

  TG4G3ControlVector* controlVector = g3PhysicsManager->GetControlVector();
  if (!controlVector) {
    TG4Globals::Exception("TG4PhysicsManager", "SetSpecialControlsActivation",
      "Vectors of processes controls is not set.");
    return;
  }

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  for (G4int i = 0; i < G4int(particleTable->size()); ++i) {

    G4ParticleDefinition* particle = particleTable->GetParticle(i);
    if (!particle) continue;

    G4ProcessManager* processManager = particle->GetProcessManager();
    if (!processManager) continue;

    G4ProcessVector* processVector = processManager->GetProcessList();

    // activate or inactivate processes according to
    // global setting in the control vector in G3 physics manager
    //
    for (size_t j = 0; j < processVector->length(); j++) {

      TG4G3ControlValue control =
        controlVector->GetControlValue((*processVector)[j]);
      G4bool activation = processManager->GetProcessActivation(j);

      if (control != kUnsetControlValue) {
        if (!TG4Globals::Compare(activation, control)) {

          // set new process activation
          G4bool activate;
          if (control == kInActivate)
            activate = false;
          else
            activate = true;

          SetProcessActivation(processManager, j, activate);
        }
      }
    }
  }
}

//_____________________________________________________________________________
void TG4PhysicsManager::SetSpecialCutsActivation()
{
  /// (In)Activate built special cut processes according
  /// to the setup in TG4G3PhysicsManager

  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  TG4boolVector* isCutVector = g3PhysicsManager->GetIsCutVector();

  if (!isCutVector) {
    TG4Globals::Exception("TG4PhysicsManager", "SetSpecialCutsActivation",
      "Vector of isCut booleans is not set.");
    return;
  }

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  for (G4int i = 0; i < G4int(particleTable->size()); ++i) {

    G4ParticleDefinition* particle = particleTable->GetParticle(i);
    if (!particle) continue;

    TG4G3ParticleWSP particleWSP = g3PhysicsManager->GetG3ParticleWSP(particle);
    if ((particleWSP != kNofParticlesWSP) && (particleWSP != kEplus) &&
        (particleWSP != kAny)) {
      // special process is activated in case
      // cutVector (vector of kinetic energy cuts) is set
      // or the special cut is set by TG4Limits

      G4ProcessManager* processManager = particle->GetProcessManager();

      // get the special cut process (if it was instantiated)
      // G4VProcess* process = GetProcess(processManager, "specialCut");
      G4VProcess* process = GetProcess(processManager, USER_SPECIAL_CUTS);
      if (process) {
        processManager->SetProcessActivation(
          process, (*isCutVector)[particleWSP]);
      }
      else {
        TG4Globals::Warning("TG4PhysicsManager", "SetSpecialCutsActivation",
          "The special cut process for " +
            TString(particle->GetParticleName()) + " is not defined");
      }
    }
  }
}

//
// public methods
//

//_____________________________________________________________________________
void TG4PhysicsManager::Gstpar(Int_t itmed, const char* param, Float_t parval)
{
  /// Passe the tracking medium parameter to TG4Limits.
  /// The tracking medium parameter is set only in case
  /// its value is different from the "global" physics setup.
  /// When any cut/control parameter is set in limits
  /// the physics manager is locked and the physics setup
  /// cannot be changed. \n Applying this TG4Limits to particles and physical
  /// processes is still in development.
  ///
  ///  Geant3 desription: \n
  ///  ================== \n To change the value of cut  or mechanism "CHPAR" to
  ///  a new value PARVAL  for tracking medium ITMED The  data   structure JTMED
  ///  contains  the   standard  tracking parameters (CUTS and flags to control
  ///  the physics processes)  which are used  by default  for all  tracking
  ///  media.   It is  possible to redefine individually  with GSTPAR  any of
  ///  these parameters  for a given tracking medium.
  ///  - ITMED     tracking medium number
  ///  - CHPAR     is a character string (variable name)
  ///  - PARVAL    must be given as a floating point.

  if (VerboseLevel() > 1) {
    G4cout << "TG4PhysicsManager::Gstpar " << param << "  " << parval << G4endl;
  }

  G4String name = TG4GeometryServices::Instance()->CutName(param);
  TG4G3Cut cut;
  if (fG3PhysicsManager->CheckCutWithTheVector(name, parval, cut)) {
    GstparCut(itmed, cut, parval);
    fG3PhysicsManager->Lock();
  }
  else {
    TG4G3Control control;
    TG4G3ControlValue controlValue;
    if (fG3PhysicsManager->CheckControlWithTheVector(
          name, parval, control, controlValue)) {
      GstparControl(itmed, control, controlValue);
      fG3PhysicsManager->Lock();
    }
    else if (cut == kNoG3Cuts && control == kNoG3Controls &&
             fNotImplParNames.find(TString(param)) == fNotImplParNames.end()) {

      if (TString(param) == "STRA") {
        TString text = name;
        text += " parameter is supported via /mcPhysics/emModel commands.\n";
        text += "    Setting via Gstpar is ignored.";
        TG4Globals::Warning("TG4PhysicsManager", "Gstpar", text);
      }
      else {
        TG4Globals::Warning("TG4PhysicsManager", "Gstpar",
          TString(name) + " parameter is not yet implemented.");
      }

      fNotImplParNames.insert(TString(param));
    }
  }
}

//_____________________________________________________________________________
Bool_t TG4PhysicsManager::SetCut(const char* cutName, Float_t cutValue)
{
  /// Set the specified cut.

  fG3PhysicsManager->CheckLock();
  TG4G3Cut g3Cut = TG4G3CutVector::GetCut(cutName);

  if (g3Cut == kNoG3Cuts) {
    if (fNotImplParNames.find(TString(cutName)) == fNotImplParNames.end()) {
      TG4Globals::Warning("TG4PhysicsManager", "SetCut",
        "Parameter " + TString(cutName) + " is not implemented.");

      fNotImplParNames.insert(TString(cutName));
    }
    return false;
  }

  // add units
  if (g3Cut == kTOFMAX)
    cutValue *= TG4G3Units::Time();
  else
    cutValue *= TG4G3Units::Energy();

  fG3PhysicsManager->SetCut(g3Cut, cutValue);

  return true;
}

//_____________________________________________________________________________
Bool_t TG4PhysicsManager::SetProcess(const char* controlName, Int_t value)
{
  /// Set the specified process control.

  fG3PhysicsManager->CheckLock();
  TG4G3Control control = TG4G3ControlVector::GetControl(controlName);

  if (control != kNoG3Controls) {
    TG4G3ControlValue controlValue =
      TG4G3ControlVector::GetControlValue(value, control);
    fG3PhysicsManager->SetProcess(control, controlValue);

    return true;
  }
  else {
    if (fNotImplParNames.find(TString(controlName)) == fNotImplParNames.end()) {
      TG4Globals::Warning("TG4PhysicsManager", "SetProcess",
        "Parameter " + TString(controlName) + " is not implemented.");
      fNotImplParNames.insert(TString(controlName));
    }
    return false;
  }
}

//_____________________________________________________________________________
Bool_t TG4PhysicsManager::DefineParticle(Int_t pdg, const char* name,
  TMCParticleType mcType, Double_t mass, Double_t charge, Double_t lifetime,
  const TString& pType, Double_t width, Int_t iSpin, Int_t iParity,
  Int_t iConjugation, Int_t iIsospin, Int_t iIsospinZ, Int_t gParity,
  Int_t lepton, Int_t baryon, Bool_t stable, Bool_t shortlived,
  const TString& subType, Int_t antiEncoding, Double_t magMoment,
  Double_t excitation)
{
  /// Check if particle with specified pdg is available in Geant4;
  /// if not let particles manager to create a new user particle.

  // Check if particle is available in Geant4
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition = 0;
  if (pdg != 0)
    particleDefinition = particleTable->FindParticle(pdg);
  else {
    G4String particleName(name);
    if (particleName == "Rootino")
      particleDefinition = particleTable->FindParticle("geantino");
  }

  if (particleDefinition) {
    TString text = "Particle with PDG=";
    text += pdg;
    text += ", name=" + TString(name) + TG4Globals::Endl();
    text += "is defined in Geant4 as \"";
    text += particleDefinition->GetParticleName().c_str();
    text += "\"." + TG4Globals::Endl();
    text += "User definition will be ignored.";

    TG4Globals::Warning("TG4PhysicsManager", "DefineParticle", text);

    return false;
  }
  else {
    fParticlesManager->AddParticle(pdg, name, mcType, mass, charge, lifetime,
      pType, width, iSpin, iParity, iConjugation, iIsospin, iIsospinZ, gParity,
      lepton, baryon, stable, shortlived, subType, antiEncoding, magMoment,
      excitation);
    return true;
  }
}

//_____________________________________________________________________________
void TG4PhysicsManager::SetUserDecay(Int_t pdg)
{
  /// Force the decay of particle with given PDG to be done with user
  /// defined decay or external decayer

  fParticlesManager->SetUserDecay(pdg);
}

//_____________________________________________________________________________
Bool_t TG4PhysicsManager::SetDecayMode(
  Int_t pdg, Float_t bratio[6], Int_t mode[6][3])
{
  /// Set a user phase space decay for a particle

  fParticlesManager->SetDecayMode(pdg, bratio, mode);

  return true;
}

//_____________________________________________________________________________
Bool_t TG4PhysicsManager::DefineIon(const char* name, Int_t Z, Int_t A, Int_t Q,
  Double_t excEnergy, Double_t /*mass*/)
{
  /// Keep user defined ion properties in order to be able to use
  /// them later as a primary particle.

  // add unit
  excEnergy *= TG4G3Units::Energy();

  fParticlesManager->AddIon(name, Z, A, Q, excEnergy);

  return true;
}

//_____________________________________________________________________________
Float_t TG4PhysicsManager::Xsec(
  char* /*ch*/, Float_t /*p1*/, Int_t /*i1*/, Int_t /*i2*/)
{
  /// Not yet implemented -> give exception.

  TG4Globals::Exception("TG4PhysicsManager", "Xsec", "Not yet implemented.");

  return 0.;
}

//_____________________________________________________________________________
Int_t TG4PhysicsManager::IdFromPDG(Int_t pdgID) const
{
  /// G4 does not use the integer particle identifiers
  /// Id <-> PDG is identity.

  return pdgID;
}

//_____________________________________________________________________________
Int_t TG4PhysicsManager::PDGFromId(Int_t mcID) const
{
  /// G4 does not use integer particle identifiers
  /// Id <-> PDG is identity.

  return mcID;
}

//_____________________________________________________________________________
TString TG4PhysicsManager::ParticleName(Int_t pdg) const
{
  /// Return name of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);

  if (particle)
    return TString(particle->GetParticleName());
  else
    return TString();
}

//_____________________________________________________________________________
Double_t TG4PhysicsManager::ParticleMass(Int_t pdg) const
{
  /// Return mass of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);

  if (particle)
    return particle->GetPDGMass() * TG4G3Units::InverseEnergy();
  else
    return 0.;
}

//_____________________________________________________________________________
Double_t TG4PhysicsManager::ParticleCharge(Int_t pdg) const
{
  /// Return charge (in e units) of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);

  if (particle)
    return particle->GetPDGCharge() * TG4G3Units::InverseCharge();
  else
    return 0.;
}

//_____________________________________________________________________________
Double_t TG4PhysicsManager::ParticleLifeTime(Int_t pdg) const
{
  /// Return life time of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);

  if (particle)
    return particle->GetPDGLifeTime();
  else
    return 0.;
}

//_____________________________________________________________________________
TMCParticleType TG4PhysicsManager::ParticleMCType(Int_t pdg) const
{
  /// Return VMC type of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);

  if (particle) {
    TG4Globals::Warning(
      "TG4PhysicsManager", "ParticleMCType", "Not yet implemented.");
    return kPTUndefined;
  }
  else
    return kPTUndefined;
}

//_____________________________________________________________________________
void TG4PhysicsManager::DefineParticles()
{
  /// Add particles with standard PDG = 0
  /// and user defined particles to TDatabasePDG
  /// and maps them to G4 particles objects.

  fParticlesManager->DefineParticles();
  TG4StateManager::Instance()->SetNewState(kAddIons);
  TVirtualMCApplication::Instance()->AddIons();
  TG4StateManager::Instance()->SetNewState(kNotInApplication);
}

//_____________________________________________________________________________
void TG4PhysicsManager::SetProcessActivation()
{
  /// (In)Activate built processes according
  /// to the setup in TG4G3PhysicsManager::fControlVector.

  if (TG4SpecialPhysicsList::Instance() &&
      TG4G3PhysicsManager::Instance()->IsGlobalSpecialControls()) {

    SetSpecialControlsActivation();
  }

  if (TG4SpecialPhysicsList::Instance() &&
      TG4SpecialPhysicsList::Instance()->IsSpecialCuts()) {

    SetSpecialCutsActivation();
  }
}

//_____________________________________________________________________________
void TG4PhysicsManager::RetrieveOpBoundaryProcess()
{
  /// Retrieve the G4OpBoundaryProcess

  G4ParticleDefinition* photon = G4OpticalPhoton::Definition();
  G4ProcessManager* processManager = photon->GetProcessManager();

  G4int nofProcesses = processManager->GetProcessListLength();
  G4ProcessVector* processList = processManager->GetProcessList();
  for (G4int i = 0; i < nofProcesses; i++) {
    if ((*processList)[i]->GetProcessSubType() == fOpBoundary) {
      fOpBoundaryProcess =
        dynamic_cast<G4OpBoundaryProcess*>((*processList)[i]);
      break;
    }
  }
}

//_____________________________________________________________________________
TMCProcess TG4PhysicsManager::GetMCProcess(const G4VProcess* process)
{
  /// Return the TMCProcess code of the specified G4 process.

  if (!process) return kPNoProcess;

  return fgProcessMap->GetMCProcess(process);
}

//_____________________________________________________________________________
TMCProcess TG4PhysicsManager::GetOpBoundaryStatus()
{
  /// Return the TMCProcess code according to the OpBoundary process
  /// status.

  if (!fOpBoundaryProcess) {
    TG4Globals::Exception("TG4PhysicsManager", "GetOpBoundaryStatus",
      "OpBoundary process is not defined.");
    return kPNoProcess;
  }

  switch (fOpBoundaryProcess->GetStatus()) {
    // reflection
    case FresnelReflection:
    case TotalInternalReflection:
    case LambertianReflection:
    case LobeReflection:
    case SpikeReflection:
    case BackScattering:
    case PolishedLumirrorAirReflection:
    case PolishedLumirrorGlueReflection:
    case PolishedAirReflection:
    case PolishedTeflonAirReflection:
    case PolishedTiOAirReflection:
    case PolishedTyvekAirReflection:
    case PolishedVM2000AirReflection:
    case PolishedVM2000GlueReflection:
    case EtchedLumirrorAirReflection:
    case EtchedLumirrorGlueReflection:
    case EtchedAirReflection:
    case EtchedTeflonAirReflection:
    case EtchedTiOAirReflection:
    case EtchedTyvekAirReflection:
    case EtchedVM2000AirReflection:
    case EtchedVM2000GlueReflection:
    case GroundLumirrorAirReflection:
    case GroundLumirrorGlueReflection:
    case GroundAirReflection:
    case GroundTeflonAirReflection:
    case GroundTiOAirReflection:
    case GroundTyvekAirReflection:
    case GroundVM2000AirReflection:
    case GroundVM2000GlueReflection:
#if G4VERSION_NUMBER >= 1110
    case CoatedDielectricReflection:
#endif
      return kPLightReflection;
      ;
      ;

    // refraction
    case FresnelRefraction:
#if G4VERSION_NUMBER >= 1000
    case Dichroic:
#endif
#if G4VERSION_NUMBER >= 1010
    case Transmission:
#endif
#if G4VERSION_NUMBER >= 1110
    case CoatedDielectricRefraction:
#endif
#if G4VERSION_NUMBER >= 1110
    case CoatedDielectricFrustratedTransmission:
#endif
      return kPLightRefraction;
      ;
      ;

    // absorption
    case Absorption:
      return kPLightAbsorption;
      ;
      ;
    // detection
    case Detection:
      return kPLightDetection;
      ;
      ;

    case NotAtBoundary:
    case SameMaterial:
    case StepTooSmall:
    case NoRINDEX:
    case Undefined:
      return kPNoProcess;
      ;
      ;
  }

  // should not happen
  return kPNoProcess;
}

#if G4VERSION_NUMBER == 1100
//_____________________________________________________________________________
void TG4PhysicsManager::StoreCerenkovMaxBetaChangeValue()
{
  // Temporary work-around for bug in Cerenkov in Geant4 11.0
  fCerenkovMaxBetaChange =
    G4OpticalParameters::Instance()->GetCerenkovMaxBetaChange();
  G4cout << "Saved fCerenkovMaxBetaChange " << fCerenkovMaxBetaChange << G4endl;
}

//_____________________________________________________________________________
void TG4PhysicsManager::ApplyCerenkovMaxBetaChangeValue()
{
  // Temporary work-around for bug in Cerenkov in Geant4 11.0
  // Apply the initial (not corrupted) value to Cerenkov process, if defined

  if (fCerenkovMaxBetaChange == 0.) return;

  auto cerenkov =
    G4Electron::Definition()->GetProcessManager()->GetProcess("Cerenkov");
  if (cerenkov != nullptr) {
    auto maxBetaChangeAfter =
      G4OpticalParameters::Instance()->GetCerenkovMaxBetaChange();
    G4cout << "Applying correction to CerenkovMaxBetaChange "
           << "from " << maxBetaChangeAfter << " to " << fCerenkovMaxBetaChange
           << G4endl;
    static_cast<G4Cerenkov*>(cerenkov)->SetMaxBetaChangePerStep(
      fCerenkovMaxBetaChange);
    G4cout << "New value: "
           << static_cast<G4Cerenkov*>(cerenkov)->GetMaxBetaChangePerStep()
           << G4endl;
  }
}
#endif
