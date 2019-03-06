//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4G3PhysicsManager.cxx
/// \brief Implementation of the TG4G3PhysicsManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4G3PhysicsManager.h"
#include "TG4G3CutVector.h"
#include "TG4G3ControlVector.h"
#include "TG4G3Defaults.h"
#include "TG4G3Units.h"

#include <G4ParticleDefinition.hh>
#include <G4VProcess.hh>
#include <G4UImessenger.hh>
#include <G4ProcessTable.hh>

TG4G3PhysicsManager* TG4G3PhysicsManager::fgInstance = 0;

//_____________________________________________________________________________
TG4G3PhysicsManager::TG4G3PhysicsManager()
  : fCutVector(0),
    fControlVector(0),
    fIsCutVector(0),
    fIsControlVector(0),
    fG3Defaults(),
    fLock(false)
{
/// Default constructor

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4G3PhysicsManager", "TG4G3PhysicsManager",
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;

  // create vector of kinetic energy cut values
  fCutVector = new TG4G3CutVector();

  // create vector of control process control values
  fControlVector = new TG4G3ControlVector();

  // initialize fIsCutVector
  fIsCutVector = new TG4boolVector();
  G4int i;
  for (i=0; i<kNofParticlesWSP; i++) fIsCutVector->push_back(false);

  // initialize fIsControlVector
  fIsControlVector = new TG4boolVector;
  for (i=0; i<kNofParticlesWSP; i++) fIsControlVector->push_back(false);
}

//_____________________________________________________________________________
TG4G3PhysicsManager::~TG4G3PhysicsManager()
{
/// Destructor

  delete fCutVector;
  delete fControlVector;
  delete fIsCutVector;
  delete fIsControlVector;

  fgInstance = 0;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4G3PhysicsManager::SetCut(TG4G3Cut cut, G4double cutValue)
{
/// Set kinetic energy cut (in a G3-like way).

  // Do nothing if cutValue is not valid
  if ( ! TG4G3CutVector::CheckCutValue(cut, cutValue) ) return;

  fCutVector->SetCut(cut, cutValue);
  SwitchIsCutVector(cut);
}

//_____________________________________________________________________________
void TG4G3PhysicsManager::SetProcess(TG4G3Control control,
                                     TG4G3ControlValue controlValue)
{
/// Set control process control (in a G3-like way).

  if (control == kDRAY || control == kG3LOSS) {
      SwitchIsCutVector(kDCUTE);
      SwitchIsCutVector(kDCUTM);
  }

  fControlVector->SetControl(control, controlValue, *fCutVector);
}


//_____________________________________________________________________________
void TG4G3PhysicsManager::SwitchIsCutVector(TG4G3Cut cut)
{
/// Update the vector of booleans (fIsCutVector) for the specified cut.

  switch (cut) {
    case kCUTGAM:
           (*fIsCutVector)[kGamma] = true;
           break;
    case kBCUTE:
           (*fIsCutVector)[kGamma] = true;
           break;
    case kBCUTM:
           (*fIsCutVector)[kGamma] = true;
           break;
    case kCUTELE:
           (*fIsCutVector)[kElectron] = true;
           break;
    case kDCUTE:
           (*fIsCutVector)[kElectron] = true;
           break;
    case kDCUTM:
           (*fIsCutVector)[kElectron] = true;
           break;
    case kPPCUTM:
           (*fIsCutVector)[kElectron] = true;
           (*fIsCutVector)[kEplus] = true;
           break;
    case kCUTNEU:
           (*fIsCutVector)[kNeutralHadron] = true;
           break;
    case kCUTHAD:
           (*fIsCutVector)[kChargedHadron] = true;
           break;
    case kCUTMUO:
           (*fIsCutVector)[kMuon] = true;
           break;
    default:
           break;
  }
}

//_____________________________________________________________________________
void TG4G3PhysicsManager::SwitchIsControlVector(TG4G3Control control)
{
/// Update the vector of booleans (fIsControlVector) for the specified control.

  switch (control) {
    case kPAIR:
           // gamma
           (*fIsControlVector)[kGamma] = true;
           break;
    case kCOMP:
           // gamma
           (*fIsControlVector)[kGamma] = true;
           break;
    case kPHOT:
           // gamma
           (*fIsControlVector)[kGamma] = true;
           break;
    case kPFIS:
           // gamma
           (*fIsControlVector)[kGamma] = true;
           break;
    case kDRAY:
           // all charged particles
           (*fIsControlVector)[kElectron] = true;
           (*fIsControlVector)[kEplus] = true;
           (*fIsControlVector)[kChargedHadron] = true;
           (*fIsControlVector)[kMuon] = true;
           break;
    case kANNI:
           // e+ only
           (*fIsControlVector)[kEplus] = true;
           break;
    case kBREM:
           // e-/e+, muons
           (*fIsControlVector)[kElectron] = true;
           (*fIsControlVector)[kEplus] = true;
           (*fIsControlVector)[kMuon] = true;
           break;
    case kHADR:
           // hadrons
           (*fIsControlVector)[kNeutralHadron] = true;
           (*fIsControlVector)[kChargedHadron] = true;
           break;
    case kMUNU:
           // muons
           (*fIsControlVector)[kMuon] = true;
           break;
    case kDCAY:
           // any
           (*fIsControlVector)[kAny] = true;
           break;
    case kG3LOSS:
           // all charged particles
           (*fIsControlVector)[kElectron] = true;
           (*fIsControlVector)[kEplus] = true;
           (*fIsControlVector)[kChargedHadron] = true;
           (*fIsControlVector)[kMuon] = true;
           break;
    case kMULS:
           // all charged particles
           (*fIsControlVector)[kElectron] = true;
           (*fIsControlVector)[kEplus] = true;
           (*fIsControlVector)[kChargedHadron] = true;
           (*fIsControlVector)[kMuon] = true;
           break;
    default:
          break;
  }
}

//
// public methods
//

//_____________________________________________________________________________
void TG4G3PhysicsManager::CheckLock()
{
/// Give exception in case the physics manager is locked.
/// Prevent from modifying physics setup after the physics manager is locked.

  if (fLock) {
    TG4Globals::Exception(
      "TG4PhysicsManager", "CheckLock",
      "It is too late to change physics setup." + TG4Globals::Endl() +
      "PhysicsManager has been already locked.");
  }
}

//_____________________________________________________________________________
G4bool TG4G3PhysicsManager::CheckCutWithTheVector(G4String name,
                                 G4double value, TG4G3Cut& cut)
{
/// Retrieve corresponding TG4G3Cut from the name and
/// in case the value is different from the value in cutVector
/// and is valid, set true the value of the fIsCutVector element
/// corresponding to this cut and return true;
/// return false otherwise.

  // convert cut name -> TG4G3Cut
  cut = TG4G3CutVector::GetCut(name);

  // add units
  if ( cut == kTOFMAX ) value *= TG4G3Units::Time();
  else                  value *= TG4G3Units::Energy();

  // check cut value validity
  if ( ! TG4G3CutVector::CheckCutValue(cut, value) ) return false;

  // set switch vector element only if the value
  // is different from the value in cutVector and is valid
  if ( cut != kNoG3Cuts ) {
    //G4cout << "Comparing: "
    //       << value << "  "
    //       << (*fCutVector)[cut] << "  "
    //       << std::abs(value - (*fCutVector)[cut]) << "  > "
    //       << TG4G3CutVector::Tolerance() << G4endl;

    if ( std::abs(value - (*fCutVector)[cut]) > TG4G3CutVector::Tolerance() ) {
      SwitchIsCutVector(cut);
      return true;
    }
    else return false;
  }
  return false;
}

//_____________________________________________________________________________
G4bool TG4G3PhysicsManager::CheckControlWithTheVector(G4String name,
                                 G4double value, TG4G3Control& control,
                                 TG4G3ControlValue& controlValue)
{
/// Retrieve corresponding TG4G3Control from the name and
/// in case the value is different from the value in controlVector
/// set true the value of the fIsControlVector element
/// corresponding to this control and return true;
/// return false otherwise.

  // convert control name -> TG4G3Control
  control = TG4G3ControlVector::GetControl(name);

  // convert double value -> TG4G3ControlValue
  controlValue = TG4G3ControlVector::GetControlValue(value, control);

  // set switch vector element only if the value
  // is different from the value in controlVector
  if (control !=kNoG3Controls) {
    if (controlValue != (*fControlVector)[control]) {
      SwitchIsControlVector(control);
      return true;
    }
    else return false;
  }
  return false;
}

//_____________________________________________________________________________
void TG4G3PhysicsManager::SetG3DefaultCuts()
{
/// Set G3 default values of kinetic energy cuts.

  CheckLock();
  fCutVector->SetG3Defaults();

  for (G4int i=0; i<kNofParticlesWSP; i++) (*fIsCutVector)[i] = true;
}

//_____________________________________________________________________________
void TG4G3PhysicsManager::SetG3DefaultControls()
{
/// Set G3 default values of control process controls.

  CheckLock();
  fControlVector->SetG3Defaults();

  for (G4int i=0; i<kNofParticlesWSP; i++) (*fIsControlVector)[i] = true;
}

//_____________________________________________________________________________
G4bool TG4G3PhysicsManager::IsSpecialCuts() const
{
/// Return true if any special cut value is set.


  for (G4int i=0; i<kNofParticlesWSP; i++)
  {  if ((*fIsCutVector)[i]) return true; }

  return false;
}

//_____________________________________________________________________________
G4bool TG4G3PhysicsManager::IsSpecialControls() const
{
/// Return true if any special control value is set
/// (per tracking medium) and is different from its
/// global value

  for (G4int i=0; i<kNofParticlesWSP; i++)
  {  if ((*fIsControlVector)[i]) return true; }

  return false;
}

//_____________________________________________________________________________
G4bool TG4G3PhysicsManager::IsGlobalSpecialControls() const
{
/// Return true if any global special control value is set

  for (G4int i=0; i<kNoG3Controls; i++)
  {  if ((*fControlVector)[i] != kUnsetControlValue ) return true; }

  return false;
}

//_____________________________________________________________________________
TG4G3ParticleWSP TG4G3PhysicsManager::GetG3ParticleWSP(
                                      G4ParticleDefinition* particle) const
{
/// Return TG4G3ParticleWSP code for the specified particle.
/// (See TG4G3ParticleWSP.h, too.)

  G4String name = particle->GetParticleName();
  G4String pType = particle->GetParticleType();

  if (name == "gamma") {
    return kGamma;
  }
  else if (name == "e-") {
    return kElectron;
  }
  else if (name == "e+") {
    return kEplus;
  }
  else if (( pType == "baryon" || pType == "meson" || pType == "nucleus" )) {
    if (particle->GetPDGCharge() == 0) {
      return kNeutralHadron;
    }
    else
      return kChargedHadron;
  }
  else if ( name == "mu-" || name == "mu+" ) {
    return kMuon;
  }
  else {
    return kNofParticlesWSP;
  }
}

//_____________________________________________________________________________
G4String TG4G3PhysicsManager::GetG3ParticleWSPName(G4int particleWSP) const
{
/// Return the name of the particle specified by TG4G3ParticleWSP constant.
/// (See TG4G3ParticleWSP.h, too.)

  switch (particleWSP) {
    case kGamma:
      return "Gamma";
      break;
    case kElectron:
      return "Electron";
      break;
    case kEplus:
      return "Eplus";
      break;
    case kNeutralHadron:
      return "NeutralHadron";
      break;
    case kChargedHadron:
      return "ChargedHadron";
      break;
    case kMuon:
      return "Muon";
      break;
    case kAny:
      return "Any";
      break;
    case kNofParticlesWSP:
      return "NoSP";
      break;
    default:
      TG4Globals::Exception(
        "TG4G3PhysicsManager", "GetG3ParticleWSPName", "Wrong particleWSP.");
      return "";
  }
}

