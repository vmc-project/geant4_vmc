// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: global
// Class TG4Limits
// ---------------
// See the class description in the header file.

#include "TG4Limits.h"

#include <globals.hh>

G4int  TG4Limits::fgCounter = 0;

//_____________________________________________________________________________
TG4Limits::TG4Limits(const TG4G3CutVector& cuts, 
                     const TG4G3ControlVector& controls)
  : G4UserLimits(),              
    // default values of G4UserLimits data members are set: 
    // fMaxStep (DBL_MAX), fMaxTrack(DBL_MAX),fMaxTime(DBL_MAX),
    // fMinEkine(0.), fMinRange(0.)
    fName(""),
    fIsCut(false),
    fIsControl(false), 
    fCutVector(cuts),
    fControlVector()
{
//
  Initialize(cuts, controls);
}

//_____________________________________________________________________________
TG4Limits::TG4Limits(const G4String& name, 
                     const TG4G3CutVector& cuts, 
                     const TG4G3ControlVector& controls)
  : G4UserLimits(),              
    // default values of G4UserLimits data members are set: 
    // fMaxStep (DBL_MAX), fMaxTrack(DBL_MAX),fMaxTime(DBL_MAX),
    // fMinEkine(0.), fMinRange(0.)
    fName(name),
    fIsCut(false),
    fIsControl(false),
    fCutVector(cuts),
    fControlVector()
{
//
  Initialize(cuts, controls);
}

//_____________________________________________________________________________
TG4Limits::TG4Limits(const G4UserLimits& g4Limits, 
                     const TG4G3CutVector& cuts, 
                     const TG4G3ControlVector& controls )
  : G4UserLimits(g4Limits),              
    fName(""),
    fIsCut(false),
    fIsControl(false),
    fCutVector(cuts),
    fControlVector()
{
//
  Initialize(cuts, controls);
}

//_____________________________________________________________________________
TG4Limits::TG4Limits()
  : G4UserLimits(),              
    // default values of G4UserLimits data members are set: 
    // fMaxStep (DBL_MAX), fMaxTrack(DBL_MAX),fMaxTime(DBL_MAX),
    // fMinEkine(0.), fMinRange(0.)
    fName(""),
    fIsCut(false),
    fIsControl(false) ,
    fCutVector(),
    fControlVector()
{
//
  ++fgCounter;
}

//_____________________________________________________________________________
TG4Limits::TG4Limits(const TG4Limits& right)
  : G4UserLimits(right), 
    fName(right.fName),
    fIsCut(right.fIsCut),
    fIsControl(right.fIsControl) ,
    fCutVector(right.fCutVector),
    fControlVector(right.fControlVector)
{
  ++fgCounter;
}  

//_____________________________________________________________________________
TG4Limits::~TG4Limits() {
//
}

//
// operators
//

//_____________________________________________________________________________
TG4Limits& TG4Limits::operator=(const TG4Limits& right)
{
  // check assignement to self
  if (this == &right) return *this;

  // base class assignement
  G4UserLimits::operator=(right);

  fName = right.fName;
  fIsCut = right.fIsCut;
  fIsControl = right.fIsControl;
  fCutVector  = right.fCutVector;
  fControlVector = right.fControlVector;

  return *this;  
}    
          
//
// private methods
//

//_____________________________________________________________________________
void TG4Limits::Initialize(const TG4G3CutVector& cuts, 
                           const TG4G3ControlVector& controls)
{                         
/// Initialization.

  fMaxTime = cuts[kTOFMAX];

  fControlVector.Update(controls);
       // only controls different from passed controls (default) are set
       
  fIsCut = fCutVector.IsCut();
  fIsControl = fControlVector.IsControl();     
  
  ++fgCounter;
}  
  

//
// public methods
//

//_____________________________________________________________________________
G4double TG4Limits::GetUserMinEkine(const G4Track& /*track*/)
{
/// Return the kinetic energy cut parameter.
/// !! The cuts values defined if fCutVector are applied
/// only via TG4SpecialCuts process.

  return fMinEkine;
}

//_____________________________________________________________________________
void TG4Limits::SetG3Cut(TG4G3Cut cut, G4double cutValue)
{
/// Set the cut value for the specified cut.

  fCutVector.SetCut(cut, cutValue);
  fIsCut = true;
  
  if (cut == kTOFMAX) fMaxTime = cutValue;
}

//_____________________________________________________________________________
void TG4Limits::SetG3Control(TG4G3Control control, 
                             TG4G3ControlValue controlValue)
{
/// Set the process control value for the specified control.

  G4bool result
    = fControlVector.SetControl(control, controlValue, fCutVector);

  if (result) fIsControl = true;
  
}

//_____________________________________________________________________________
void TG4Limits::SetG3DefaultCuts()
{
/// Set the G3 default cut values for all cuts.

  fCutVector.SetG3Defaults();
  fIsCut = true;  
}

//_____________________________________________________________________________
G4bool TG4Limits::Update(const TG4G3ControlVector& controls)
{                       
/// Update controls in a special way.
/// Return true if some control in fControlVector is after update
/// still set.

  //G4bool result = fCutVector.Update(cutVector);
  //if (result) fIsCut = true;

  fControlVector.Update(controls);
  fIsControl = fControlVector.IsControl();
  
  return fIsControl;
}

//_____________________________________________________________________________
void TG4Limits::SetG3DefaultControls()
{
/// Set the G3 default process control values for all flags.

  fControlVector.SetG3Defaults();
  fIsControl = true;
}

//_____________________________________________________________________________
void TG4Limits::Print() const
{
/// Print limits.

   G4cout << "\"" << fName << "\"  limits:"<< G4endl;
   G4cout << "  Max step length (mm):  " << fMaxStep/mm   << G4endl;
   G4cout << "  Max track length (mm): " << fMaxTrack/mm  << G4endl;
   G4cout << "  Max time (s)           " << fMaxTime/s    << G4endl;
   G4cout << "  Min kin. energy (MeV)  " << fMinEkine/MeV << G4endl;
   G4cout << "  Min range (mm):        " << fMinRange/mm  << G4endl;

   if (!fIsCut)     G4cout << "  No special cuts. "<< G4endl;
   if (!fIsControl) G4cout << "  No special controls. "<< G4endl;

   if (fIsCut)     fCutVector.Print();
   if (fIsControl) fControlVector.Print();
}

//_____________________________________________________________________________
G4double TG4Limits::GetMinEkineForGamma(const G4Track& track) const
{
/// Return the cut value for gamma.

  if (fIsCut)
    return fCutVector.GetMinEkineForGamma(track);
  else 
    return fMinEkine;
}

//_____________________________________________________________________________
G4double TG4Limits::GetMinEkineForElectron(const G4Track& track) const
{
/// Return the cut value for e-.

  if (fIsCut)
    return fCutVector.GetMinEkineForElectron(track);
  else 
    return fMinEkine;
}

//_____________________________________________________________________________
G4double TG4Limits::GetMinEkineForEplus(const G4Track& track) const
{
/// Return the cut value for e-.

  if (fIsCut)
    return fCutVector.GetMinEkineForEplus(track);
  else 
    return fMinEkine;
}

//_____________________________________________________________________________
G4double TG4Limits::GetMinEkineForChargedHadron(const G4Track& track) const
{
/// Return the cut value for charged hadron.

  if (fIsCut)
    return fCutVector.GetMinEkineForChargedHadron(track);
  else 
    return fMinEkine;
}

//_____________________________________________________________________________
G4double TG4Limits::GetMinEkineForNeutralHadron(const G4Track& track) const
{
/// Return the cut value for neutral hadron.

  if (fIsCut)
    return fCutVector.GetMinEkineForNeutralHadron(track);
  else 
    return fMinEkine;
}

//_____________________________________________________________________________
G4double TG4Limits::GetMinEkineForMuon(const G4Track& track) const
{
/// Return the cut value for neutral muon.

  if (fIsCut)
    return fCutVector.GetMinEkineForMuon(track);
  else 
    return fMinEkine;
}

//_____________________________________________________________________________
G4double TG4Limits::GetMinEkineForOther(const G4Track& track) const
{
/// Return the cut value for other particle.

  if (fIsCut)
    return fCutVector.GetMinEkineForOther(track);
  else 
    return fMinEkine;
}

//_____________________________________________________________________________
TG4G3ControlValue TG4Limits::GetControl(G4VProcess* process) const 
{
/// Return the flag value for the particle associated with
/// the specified process.

  if (fIsControl)
    return fControlVector.GetControlValue(process);
  else 
    return kUnsetControlValue;
}
