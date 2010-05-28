// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4G3ControlVector.cxx
/// \brief Implementation of the TG4G3ControlVector class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4G3ControlVector.h"
#include "TG4G3CutVector.h"
#include "TG4ProcessControlMap.h"
#include "TG4G3Defaults.h"
#include "TG4Globals.h"

#include <G4VProcess.hh>

#if __GNUC__ >= 3
#include <sstream>
#else
#include <strstream>
#endif

#include <math.h>

TG4StringVector TG4G3ControlVector::fgControlNameVector;

//_____________________________________________________________________________
TG4G3ControlVector::TG4G3ControlVector()
  : fControlVector()
{
/// Default constructor

  // initialize fControlVector 
  for (G4int i=0; i<=kNoG3Controls; i++) 
    fControlVector.push_back(kUnsetControlValue); 
  
  // fill name vector
  if (fgControlNameVector.size() == 0) FillControlNameVector(); 
}

//_____________________________________________________________________________
TG4G3ControlVector::TG4G3ControlVector(const TG4G3ControlVector& right)
  :  fControlVector(right.fControlVector.size())
{
/// Copy constructor

  // copy stuff
  *this = right;  
}

//_____________________________________________________________________________
TG4G3ControlVector::~TG4G3ControlVector() 
{
/// Destructor
}

//
// operators
//

//_____________________________________________________________________________
TG4G3ControlVector& TG4G3ControlVector::operator=(
                                          const TG4G3ControlVector& right)
{
/// Assignment operator

  // check assignement to self
  if (this == &right) return *this;

  // initialize fControlVector 
  for (G4int i=0; i<=kNoG3Controls; i++) 
    fControlVector[i] = right.fControlVector[i];
  
  return *this;   
}  

//_____________________________________________________________________________
TG4G3ControlValue TG4G3ControlVector::operator[](G4int index) const
{
/// Element access operator 

  if (index <= kNoG3Controls)
    return fControlVector[index];
  else {
    TG4Globals::Exception(
      "TG4G3ControlVector", "operator[]", "Index out of the vector scope");
    return kUnsetControlValue;  
  }    
}  

//
// private methods
//

//_____________________________________________________________________________
void TG4G3ControlVector::FillControlNameVector() 
{
/// Define fControlNameVector.

  fgControlNameVector.push_back("PAIR");
  fgControlNameVector.push_back("COMP");
  fgControlNameVector.push_back("PHOT");
  fgControlNameVector.push_back("PFIS");
  fgControlNameVector.push_back("DRAY");
  fgControlNameVector.push_back("ANNI");
  fgControlNameVector.push_back("BREM");
  fgControlNameVector.push_back("HADR");
  fgControlNameVector.push_back("MUNU");
  fgControlNameVector.push_back("DCAY");
  fgControlNameVector.push_back("LOSS");
  fgControlNameVector.push_back("MULS");
  fgControlNameVector.push_back("CKOV");
  fgControlNameVector.push_back("RAYL");
  fgControlNameVector.push_back("LABS");
  fgControlNameVector.push_back("SYNC");
  fgControlNameVector.push_back("NONE");
}

//
// public methods
//

//_____________________________________________________________________________
TG4G3Control TG4G3ControlVector::GetControl(const G4String& controlName)
{
/// Retrieve corresponding TG4G3Control constant from the controlName.

  if      (controlName == fgControlNameVector[kPAIR]) return kPAIR;
  else if (controlName == fgControlNameVector[kCOMP]) return kCOMP;
  else if (controlName == fgControlNameVector[kPHOT]) return kPHOT;
  else if (controlName == fgControlNameVector[kPFIS]) return kPFIS;
  else if (controlName == fgControlNameVector[kDRAY]) return kDRAY;
  else if (controlName == fgControlNameVector[kANNI]) return kANNI;
  else if (controlName == fgControlNameVector[kBREM]) return kBREM;
  else if (controlName == fgControlNameVector[kHADR]) return kHADR;
  else if (controlName == fgControlNameVector[kMUNU]) return kMUNU;
  else if (controlName == fgControlNameVector[kDCAY]) return kDCAY;
  else if (controlName == fgControlNameVector[kLOSS]) return kLOSS;
  else if (controlName == fgControlNameVector[kMULS]) return kMULS;
  else if (controlName == fgControlNameVector[kCKOV]) return kCKOV;
  else if (controlName == fgControlNameVector[kRAYL]) return kRAYL;
  else if (controlName == fgControlNameVector[kLABS]) return kLABS;
  else if (controlName == fgControlNameVector[kSYNC]) return kSYNC;
  else return kNoG3Controls;
}

//_____________________________________________________________________________
const G4String& TG4G3ControlVector::GetControlName(TG4G3Control control)
{
/// Return name of a specified cut.

  // fill name vector
  if (fgControlNameVector.size() == 0) 
    TG4G3ControlVector::FillControlNameVector(); 

  return fgControlNameVector[control];
}  

//_____________________________________________________________________________
TG4G3ControlValue TG4G3ControlVector::GetControlValue(G4int value,
                                                      TG4G3Control control)
{
/// Conversion G4int -> G3ControlValue,
/// special treatment for LOSS values 3,4,5.

  switch (value) {
    case kInActivate: 
      return kInActivate;
      ;;
    case kActivate:
      return kActivate;
      ;;
    case kActivate2:
      return kActivate2;
      ;;
    case 3: case 4: case 5:
      if (control == kLOSS) 
        return kActivate;
      else
        return kUnsetControlValue;
      ;;                        
  }    
  return kUnsetControlValue;
}    

//_____________________________________________________________________________
TG4G3ControlValue TG4G3ControlVector::GetControlValue(G4double value, 
                                                      TG4G3Control control)
{
/// Conversion G4double -> G3ControlValue

  return TG4G3ControlVector::GetControlValue((G4int)value, control);
}    


//_____________________________________________________________________________
G4bool TG4G3ControlVector::SetControl(TG4G3Control control, 
                                      TG4G3ControlValue controlValue,
                                      TG4G3CutVector& cuts)
{
/// Set the controlValue for the specified process control.
/// Modify cuts if necessary.
/// Return true if the control value was set.

  if (control == kDRAY) {
    if (controlValue == kActivate &&
        GetControlValue(kLOSS) == kActivate2) {
      TG4Globals::Warning(
        "TG4Limits", "SetG3Control", "Cannot set DRAY=1 when LOSS=2.");    
      return false;
    }
    else 
      cuts.SetDeltaRaysOn(true);
  }               

  if (control == kLOSS && controlValue == kActivate2) {
    SetControl(kDRAY, kInActivate, cuts);
    cuts.SetDeltaRaysOn(false);  
  }        

  fControlVector[control] = controlValue;
  return true;
}

//_____________________________________________________________________________
void TG4G3ControlVector::SetG3Defaults()
{
/// Set G3 default values for all controls.

  for (G4int i=0; i<=kNoG3Controls; i++) 
    fControlVector[i] = TG4G3Defaults::Instance()->ControlValue(i);
}

//_____________________________________________________________________________
G4bool TG4G3ControlVector::Update(const TG4G3ControlVector& vector)
{
/// Unset value of DRAY (this information was passed to cut vector.)
/// Reset value of LOSS (the special controls process operates only with
/// activate/inactivate options.)
/// Return true if some value was modified.

  G4bool result = false;

  if (fControlVector[kDRAY] != kUnsetControlValue ) {
      fControlVector[kDRAY] = kUnsetControlValue;
       result = true;
  }
  
  // if both kLOSS values will have the same effect
  // unset this control

  TG4G3ControlValue passed  = vector[kLOSS];
  TG4G3ControlValue current = fControlVector[kLOSS];

  if (passed  == kActivate2) passed = kActivate;
  if (current == kActivate2) current = kActivate;
           // there is no need to distinguish 
           // kActivate, kActivate2 after Init phase

  if (current == passed) current = kUnsetControlValue;
           // if both kLOSS values will have the same effect
           // unset this control

  if (current != fControlVector[kLOSS]) {
     fControlVector[kLOSS] = current;
     result = true;
  }
  return result;     
}

//_____________________________________________________________________________
G4String TG4G3ControlVector::Format() const
{
/// Format the output into a string.

#if __GNUC__ >= 3
  std::ostringstream tmpStream;
#else  
  std::strstream tmpStream;
#endif

  tmpStream << "  G3 control vector:" << G4endl; 
  for (G4int i=0; i<kNoG3Controls; i++) 
    //if (i != kDRAY) {
      tmpStream << "    " << fgControlNameVector[i] 
                << " control value: " << fControlVector[i] << G4endl; 
    //}             
    
  return tmpStream.str();  
}           

//_____________________________________________________________________________
void TG4G3ControlVector::Print() const
{
/// Print the controls.

  G4cout << Format();             
}           

//_____________________________________________________________________________
TG4G3ControlValue 
TG4G3ControlVector::GetControlValue(G4VProcess* process) const 
{
/// Return the control value for the particle associated with
/// the specified process.

  TG4G3Control control 
    = TG4ProcessControlMap::Instance()->GetControl(process);
    
  return fControlVector[control];
}

//_____________________________________________________________________________
TG4G3ControlValue 
TG4G3ControlVector::GetControlValue(TG4G3Control control) const 
{
/// Return the control value for the particle associated with
/// the specified process.

  return fControlVector[control];
}

//_____________________________________________________________________________
G4bool TG4G3ControlVector::IsControl() const
{
/// Return true if any of controls is set.

  for (G4int i=0; i<kNoG3Controls; i++) 
    if (fControlVector[i] != kUnsetControlValue) return true;
    
  return false;  
}  
