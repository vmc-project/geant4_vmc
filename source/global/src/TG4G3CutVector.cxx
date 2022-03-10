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

#include <G4EmProcessSubType.hh>
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
{
  /// Default constructor

  // fill name vector
  if (fgCutNameVector.size() == 0) FillCutNameVector();

  // initialize fCutVector
  for (G4int i = 0; i < kTOFMAX; i++) {
    fCutVector.push_back(0.);
  }
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
  for (G4int i = 0; i < kNoG3Cuts; i++) {
    fCutVector[i] = right.fCutVector[i];
  }

  fDeltaRaysOn = right.fDeltaRaysOn;
  fIsCut = right.fIsCut;

  // copy flag arrays
  for (G4int i = 0; i <= kDM; ++i) {
    fIsBDCut[i] = right.fIsBDCut[i];
    fApplyBDCut[i] = right.fApplyBDCut[i];
  }
;

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
  if (cutValue > 0.) {
    fIsCut = true;
  }

  // Update the selected flags
  if (cut == kBCUTE) {
    fIsBDCut[kBE] = true;
  }
  else if (cut == kBCUTM) {
    fIsBDCut[kBM] = true;
  }
  else if (cut == kDCUTE) {
    fIsBDCut[kDE] = true;
  }
  else if (cut == kDCUTM) {
    fIsBDCut[kDM] = true;
  }

  // set the CUTGAM value also to BCUTE, BCUTM unless they were already set
  if (cut == kCUTGAM) {
    if (!fIsBDCut[kBE]) {
      fCutVector[kBCUTE] = cutValue;
    }
    if (!fIsBDCut[kBM]) {
      fCutVector[kBCUTM] = cutValue;
    }
  }

  // Set the CUTELE value also to DCUTE, DCUTM unless they were already set
  if (cut == kCUTELE) {
    if (!fIsBDCut[kDE]) {
      fCutVector[kDCUTE] = cutValue;
    }
    if (!fIsBDCut[kDM]) {
      fCutVector[kDCUTM] = cutValue;
    }
  }

  // Notify whther the [B/D]CUT should be taken into account
  if ( fCutVector[kBCUTE] != fCutVector[kCUTGAM] ||
       fCutVector[kBCUTM] != fCutVector[kCUTGAM] ) {
    fApplyBDCut[kB] = true;
    if (fCutVector[kBCUTE] != fCutVector[kBCUTM]) {
      fApplyBDCut[kBEM] = true;
    }
  }

  if ( fCutVector[kDCUTE] != fCutVector[kCUTELE] ||
       fCutVector[kDCUTM] != fCutVector[kCUTELE]) {
    fApplyBDCut[kD] = true;
    if (fCutVector[kDCUTE] != fCutVector[kDCUTM]) {
      fApplyBDCut[kDEM] = true;
    }
  }
}

//_____________________________________________________________________________
void TG4G3CutVector::SetG3Defaults()
{
  /// Set G3 default values for all cuts.

  for (G4int i = 0; i < kNoG3Cuts; i++) {
    fCutVector[i] = TG4G3Defaults::Instance()->CutValue(i);
  }

  fIsCut = true;
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

  // Cut for Bremstrahlung not set
  // or creator process is not Bremstrahlung
  if (! fApplyBDCut[kB] ||
      track.GetCreatorProcess() == nullptr ||
      track.GetCreatorProcess()->GetProcessSubType() != fBremsstrahlung) {
    return fCutVector[kCUTGAM];
  }

  if (! fApplyBDCut[kBEM]) {
    // Bremstrahlung - BCUTE, BCUTM need not to be applied separately
    return fCutVector[kBCUTE];
  }
  else {
    // Bremstrahlung - BCUTE, BCUTM are different
    auto processName = track.GetCreatorProcess()->GetProcessName();
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

  // Cut for Delta e- is not set and Delta e- are not switched off or
  // creator process is not Ionisation
  if ((! fApplyBDCut[kD] && fDeltaRaysOn) ||
      track.GetCreatorProcess() != nullptr ||
      track.GetCreatorProcess()->GetProcessSubType() == fIonisation ) {
    return fCutVector[kCUTELE];
  }

  // Delta e- are switched off
  if (! fDeltaRaysOn) {
     return fgkDCUTEOff;
  }

  // Cut for Delta e- is not set
  if (! fApplyBDCut[kD]) {
    return fCutVector[kCUTELE];
  }

  if (! fApplyBDCut[kDEM]) {
    // Delta e- - DCUTE, DCUTM need not to be applied separately
    return fCutVector[kDCUTE];
  }
  else {
    // Delta e- - DCUTE, DCUTM are different
    auto processName = track.GetCreatorProcess()->GetProcessName();
    if (processName == "eIoni") {
      return fCutVector[kDCUTE];
    }
    else if (processName == "muIoni") {
      return fCutVector[kDCUTM];
    }
    else {
      return fCutVector[kCUTELE];
    }
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
