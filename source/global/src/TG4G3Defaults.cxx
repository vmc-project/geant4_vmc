// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4G3Defaults.cxx
/// \brief Implementation of the TG4G3Defaults class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4G3Defaults.h"
#include "TG4Globals.h"

#include <math.h>

// static data members

TG4G3Defaults* TG4G3Defaults::fgInstance = 0;

//_____________________________________________________________________________
TG4G3Defaults::TG4G3Defaults() 
  : fCutVector(),
    fControlVector()
{
/// Default constructor

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4G3Defaults", "TG4G3Defaults", 
      "Cannot create two instances of singleton.");
  }
      
  fgInstance = this;  

  // fill cut vector with default values

  fCutVector.SetCut(kCUTGAM, 0.001 * GeV);
  fCutVector.SetCut(kCUTELE, 0.001 * GeV);
  fCutVector.SetCut(kCUTNEU, 0.01 * GeV);
  fCutVector.SetCut(kCUTHAD, 0.01 * GeV);
  fCutVector.SetCut(kCUTMUO, 0.01 * GeV);
  fCutVector.SetCut(kBCUTE, fCutVector[kCUTGAM]);
  fCutVector.SetCut(kBCUTM, fCutVector[kCUTGAM]);
  fCutVector.SetCut(kDCUTE,  10. * TeV);
  fCutVector.SetCut(kDCUTM,  10. * TeV);
  fCutVector.SetCut(kPPCUTM, 0.01 * GeV);

  // fill control vector with default values
  fControlVector.SetControl(kPAIR, kActivate, fCutVector);   // 1
  fControlVector.SetControl(kCOMP, kActivate, fCutVector);   // 1
  fControlVector.SetControl(kPHOT, kActivate, fCutVector);   // 1
  fControlVector.SetControl(kPFIS, kInActivate, fCutVector); // 0
  fControlVector.SetControl(kDRAY, kInActivate, fCutVector); // 0
  fControlVector.SetControl(kANNI, kActivate, fCutVector);   // 1
  fControlVector.SetControl(kBREM, kActivate, fCutVector);   // 1
  fControlVector.SetControl(kHADR, kActivate, fCutVector);   // 1
  fControlVector.SetControl(kMUNU, kInActivate, fCutVector); // 0
  fControlVector.SetControl(kDCAY, kActivate, fCutVector);   // 1
  fControlVector.SetControl(kLOSS, kActivate2, fCutVector);  // 2
  fControlVector.SetControl(kMULS, kActivate, fCutVector);   // 1
  fControlVector.SetControl(kCKOV, kInActivate, fCutVector); // 0
  fControlVector.SetControl(kRAYL, kInActivate, fCutVector); // 0
  fControlVector.SetControl(kLABS, kInActivate, fCutVector); // 0
  fControlVector.SetControl(kSYNC, kInActivate, fCutVector); // 0
}
  
//_____________________________________________________________________________
TG4G3Defaults::~TG4G3Defaults() 
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
G4double TG4G3Defaults::CutValue(G4int g3Cut) const
{
/// Return the G3 default value for the specified cut.

  return fCutVector[g3Cut];
}          

//_____________________________________________________________________________
TG4G3ControlValue TG4G3Defaults::ControlValue(G4int control) const
{
/// Return the G3 default value for the specified control.

  return fControlVector[control];
}          

//_____________________________________________________________________________
G4bool TG4G3Defaults::IsDefaultCut(TG4G3Cut cut, G4double value) const
{
/// Test if the parameter value is equal to the G3 default value.

  if (std::abs(value*GeV - CutValue(cut)) > TG4G3CutVector::Tolerance()) 
    return false;
  else  
    return true;
}

//_____________________________________________________________________________
G4bool TG4G3Defaults::IsDefaultControl(TG4G3Control control,
                                       TG4G3ControlValue value) const
{
/// Test if the parameter value is equal to the G3 default value.

  if (value == ControlValue(control)) 
    return true;
  else  
    return false;
}
