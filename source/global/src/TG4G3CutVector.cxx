//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4G3CutVector.cxx
/// \brief Implementation of the TG4G3CutVector class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4G3CutVector.h"
#include "TG4G3Defaults.h"
#include "TG4Globals.h"

#include "CLHEP/Units/PhysicalConstants.h"

#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>

#if __GNUC__ >= 3
#include <sstream>
#else
#include <strstream>
#endif

const G4double TG4G3CutVector::fgkDCUTEOff = 10. * TeV;
const G4double TG4G3CutVector::fgkDCUTMOff = 10. * TeV;
const G4double TG4G3CutVector::fgkTolerance = 1. * keV;
// for cut in time this value represents 1e-03s
TG4StringVector TG4G3CutVector::fgCutNameVector;

//
// static methods
//

//_____________________________________________________________________________
TG4G3Cut TG4G3CutVector::GetCut(const G4String& cutName)
{
  /// Retrieve corresponding TG4G3Cut constant from the cutName.

  if (cutName == fgCutNameVector[kCUTGAM])
    return kCUTGAM;
  else if (cutName == fgCutNameVector[kCUTELE])
    return kCUTELE;
  else if (cutName == fgCutNameVector[kCUTNEU])
    return kCUTNEU;
  else if (cutName == fgCutNameVector[kCUTHAD])
    return kCUTHAD;
  else if (cutName == fgCutNameVector[kCUTMUO])
    return kCUTMUO;
  else if (cutName == fgCutNameVector[kBCUTE])
    return kBCUTE;
  else if (cutName == fgCutNameVector[kBCUTM])
    return kBCUTM;
  else if (cutName == fgCutNameVector[kDCUTE])
    return kDCUTE;
  else if (cutName == fgCutNameVector[kDCUTM])
    return kDCUTM;
  else if (cutName == fgCutNameVector[kPPCUTM])
    return kPPCUTM;
  else if (cutName == fgCutNameVector[kTOFMAX])
    return kTOFMAX;
  else
    return kNoG3Cuts;
}

//_____________________________________________________________________________
G4bool TG4G3CutVector::CheckCutValue(TG4G3Cut cut, G4double value)
{
  /// Check if the cut value is valid for given cut;
  /// if not return false.
  /// Currently only the PPCUTM value is tested if it is > 2 * mass of electron.

  // G4cout << "TG4G3CutVector::CheckCutValue: "
  //       << value / MeV << " MeV  "
  //       << 2.*CLHEP::electron_mass_c2 / MeV  << " MeV" << G4endl;

  if (cut == kPPCUTM &&
      value < (2. * CLHEP::electron_mass_c2 - TG4G3CutVector::Tolerance())) {
    // TString message = "PPCUTM cut value ";
    // message += value;
    // message += " MeV is lower than 2*e_mass.";
    // TG4Globals::Warning(
    //  "TG4G3CutVector", "CheckCutValue",
    //  message + TG4Globals::Endl() +
    //  TString("The cut will be ignored."));
    // The cut value is often set to 1 MeV, which makes no difference
    // in Geant4, but in Geant3 it overrides the default, 10 MeV
    return false;
  }

  return true;
}

//_____________________________________________________________________________
const G4String& TG4G3CutVector::GetCutName(TG4G3Cut cut)
{
  /// Return name of a specified cut.

  // fill name vector
  if (fgCutNameVector.size() == 0) TG4G3CutVector::FillCutNameVector();

  return fgCutNameVector[cut];
}

//
// ctors, dtors
//

//_____________________________________________________________________________
TG4G3CutVector::TG4G3CutVector()
  : fCutVector(),
    fDeltaRaysOn(true),
    fIsBCUTE(false),
    fIsBCUTM(false),
    fIsDCUTE(false),
    fIsDCUTM(false)
{
  /// Default constructor

  // fill name vector
  if (fgCutNameVector.size() == 0) FillCutNameVector();

  // initialize fCutVector
  for (G4int i = 0; i < kTOFMAX; i++) fCutVector.push_back(0.);
  fCutVector.push_back(DBL_MAX);
}

//_____________________________________________________________________________
TG4G3CutVector::TG4G3CutVector(const TG4G3CutVector& right)
  : fCutVector(right.fCutVector.size()), fDeltaRaysOn(right.fDeltaRaysOn)
{
  /// Copy constructor

  // copy stuff
  *this = right;
}

//_____________________________________________________________________________
TG4G3CutVector::~TG4G3CutVector()
{
  /// Destructor
}

//
// operators
//

//_____________________________________________________________________________
TG4G3CutVector& TG4G3CutVector::operator=(const TG4G3CutVector& right)
{
  /// Assignment operator

  // check assignement to self
  if (this == &right) return *this;

  // copy fCutVector
  for (G4int i = 0; i < kNoG3Cuts; i++) fCutVector[i] = right.fCutVector[i];

  fDeltaRaysOn = right.fDeltaRaysOn;
  fIsBCUTE = right.fIsBCUTE;
  fIsBCUTM = right.fIsBCUTM;
  fIsDCUTE = right.fIsDCUTE;
  fIsDCUTM = right.fIsDCUTM;

  return *this;
}

//_____________________________________________________________________________
G4double TG4G3CutVector::operator[](G4int index) const
{
  /// Element access operator

  if (index < kNoG3Cuts)
    return fCutVector[index];
  else {
    TG4Globals::Exception(
      "TG4G3CutVector", "operator[]", "Index out of the vector scope");
    return 0.;
  }
}

//
// private methods
//

//_____________________________________________________________________________
void TG4G3CutVector::FillCutNameVector()
{
  /// Define fCutNameVector.

  fgCutNameVector.push_back("CUTGAM");
  fgCutNameVector.push_back("CUTELE");
  fgCutNameVector.push_back("CUTNEU");
  fgCutNameVector.push_back("CUTHAD");
  fgCutNameVector.push_back("CUTMUO");
  fgCutNameVector.push_back("BCUTE");
  fgCutNameVector.push_back("BCUTM");
  fgCutNameVector.push_back("DCUTE");
  fgCutNameVector.push_back("DCUTM");
  fgCutNameVector.push_back("PPCUTM");
  fgCutNameVector.push_back("TOFMAX");
  fgCutNameVector.push_back("NONE");
}

//
// public methods
//

//_____________________________________________________________________________
void TG4G3CutVector::SetCut(TG4G3Cut cut, G4double cutValue)
{
  /// Set the cutValue for the specified cut.

  if (cut >= kNoG3Cuts) {
    TG4Globals::Exception("TG4G3CutVector", "SetG3Cut", "Inconsistent cut.");
  }

  fCutVector[cut] = cutValue;

  // Update the selected flags
  if (cut == kBCUTE) {
    fIsBCUTE = true;
  }
  else if (cut == kBCUTM) {
    fIsBCUTM = true;
  }
  else if (cut == kDCUTE) {
    fIsDCUTE = true;
  }
  else if (cut == kDCUTM) {
    fIsDCUTM = true;
  }

  // Set the CUTELE value also to DCUTE, DCUTM unless they were already set
  if (cut == kCUTELE) {
    if (! fIsBCUTE) {
      fCutVector[kDCUTE] = cutValue;
    }
    if (! fIsBCUTM) {
      fCutVector[kDCUTM] = cutValue;
    }
  }

  // set the CUTGAM value also to BCUTE, BCUTM unless they were already set
  if (cut == kCUTGAM) {
    if (! fIsBCUTE) {
      fCutVector[kBCUTE] = cutValue;
    }
    if (! fIsBCUTM) {
      fCutVector[kBCUTM] = cutValue;
    }
  }
}

//_____________________________________________________________________________
void TG4G3CutVector::SetG3Defaults()
{
  /// Set G3 default values for all cuts.

  for (G4int i = 0; i < kNoG3Cuts; i++)
    fCutVector[i] = TG4G3Defaults::Instance()->CutValue(i);
}

//_____________________________________________________________________________
G4String TG4G3CutVector::Format() const
{
  /// Format the output into a string.

#if __GNUC__ >= 3
  std::ostringstream tmpStream;
#else
  std::strstream tmpStream;
#endif

  tmpStream << "  G3 cut vector:" << G4endl;
  if (fDeltaRaysOn)
    tmpStream << "    Delta rays On" << G4endl;
  else
    tmpStream << "    Delta rays Off" << G4endl;

  // energy cuts
  for (G4int i = 0; i < kTOFMAX; i++) {

    tmpStream << "    " << fgCutNameVector[i] << " cut value (MeV): ";

    if (i == kDCUTE && !fDeltaRaysOn)
      tmpStream << fgkDCUTEOff / MeV << G4endl;
    else if (i == kDCUTM && !fDeltaRaysOn)
      tmpStream << fgkDCUTMOff / MeV << G4endl;
    else
      tmpStream << fCutVector[i] / MeV << G4endl;
  }

  // time cut
  tmpStream << "    " << fgCutNameVector[kTOFMAX]
            << " cut value (s):   " << fCutVector[kTOFMAX] / s << G4endl;

  return tmpStream.str();
}

//_____________________________________________________________________________
void TG4G3CutVector::Print() const
{
  /// Print the cuts.

  G4cout << Format();
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForGamma(const G4Track& track) const
{
  /// Return the cut value for gamma:
  /// - BCUTE   - if gamma is produced by e- Bremstrahlung
  /// - BCUTM   - if gamma is produced by muon or hadron Bremstrahlung
  /// - CUTGAM  - in all other cases
  ///
  /// Cut is not applied for "opticalphoton" which is treated in G4 as a
  /// particle different from "gamma".

  // Special treatment of bremstrahlung threshold:
  // apply the BCUT*  in the first step only
  if (track.GetCurrentStepNumber() == 1) {

    const G4VProcess* kpCreatorProcess = track.GetCreatorProcess();
    G4String processName = "";
    if (kpCreatorProcess) processName = kpCreatorProcess->GetProcessName();

    if (processName == "eBrem") {
      return fCutVector[kBCUTE];
    }
    else if (processName == "muBrems" || processName == "hBrems") {
      return fCutVector[kBCUTM];
    }
    else {
      return fCutVector[kCUTGAM];
    }
  }
  else {
    return fCutVector[kCUTGAM];
  }
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForElectron(const G4Track& track) const
{
  /// Return the cut value for e-:
  /// - DCUTE - if e- is produced by e- ionisation and DRAY process is selected
  ///           (default)
  /// - DCUTM - if e- is produced by muon ionisation and DRAY process is
  /// selected
  ///           (default)
  /// - 10 TeV - if e- is produced by ionisation and DRAY process is
  ///            switched off
  /// - CUTELE - in all other cases.
  /// The delta ray cuts are applied only in the track first step.

  // Special treatment of delta rays threashold:
  // apply the DCUT* in the first step only
  if (track.GetCurrentStepNumber() == 1) {

    const G4VProcess* kpCreatorProcess = track.GetCreatorProcess();
    G4String processName = "";
    if (kpCreatorProcess) processName = kpCreatorProcess->GetProcessName();

    if (processName == "eIoni") {
      // delta rays by e-, e+
      if (fDeltaRaysOn)
        return fCutVector[kDCUTE];
      else
        return fgkDCUTEOff;
    }
    else if (processName == "muIoni") {
      // delta rays by mu
      if (fDeltaRaysOn)
        return fCutVector[kDCUTM];
      else
        return fgkDCUTMOff;
    }
    else if (processName == "hIoni" || processName == "ionIoni") {
      // delta rays by other particles
      if (fDeltaRaysOn)
        return fCutVector[kCUTELE];
      else
        return fgkDCUTMOff;
    }
    else {
      return fCutVector[kCUTELE];
    }
  }
  else {
    return fCutVector[kCUTELE];
  }
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForChargedHadron(
  const G4Track& /*track*/) const
{
  /// Return the cut value for charged hadron (CUTHAD).

  return fCutVector[kCUTHAD];
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForNeutralHadron(
  const G4Track& /*track*/) const
{
  /// Return the cut value for neutral hadron (CUTNEU).

  return fCutVector[kCUTNEU];
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForMuon(const G4Track& /*track*/) const
{
  /// Return the cut value for muon (CUTMUO)

  return fCutVector[kCUTMUO];
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEtotPair() const
{
  /// Return the cut value for the pair production by muons (PPCUTM).

  return fCutVector[kPPCUTM];
}

//_____________________________________________________________________________
G4bool TG4G3CutVector::IsCut() const
{
  /// Return true if any of cuts is set.

  for (G4int i = 0; i < kNoG3Cuts; i++)
    if (fCutVector[i] > fgkTolerance) return true;

  return false;
}
