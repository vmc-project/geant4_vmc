// $Id: TG4PhysicsListOptions.cxx,v 1.2 2007/05/10 14:44:53 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4PhysicsListOptions
// ---------------------------
// See the class description in the header file.

#include "TG4PhysicsListOptions.h"

//_____________________________________________________________________________
TG4PhysicsListOptions::TG4PhysicsListOptions()
  : fSetEMPhysics(true),
    fSetMuonPhysics(true),
    fSetHadronPhysics(false),
    fSetOpticalPhysics(false),
    fSetSpecialCutsPhysics(false),
    fSetSpecialControlsPhysics(false),
    fSetStepLimiterPhysics(true),
    fSetStackPopperPhysics(false)
{
//
}

//_____________________________________________________________________________
TG4PhysicsListOptions::TG4PhysicsListOptions(const TG4PhysicsListOptions& right)
  : fSetEMPhysics(right.fSetEMPhysics),
    fSetMuonPhysics(right.fSetMuonPhysics),
    fSetHadronPhysics(right.fSetHadronPhysics),
    fSetOpticalPhysics(right.fSetOpticalPhysics),
    fSetSpecialCutsPhysics(right.fSetSpecialCutsPhysics),
    fSetSpecialControlsPhysics(right.fSetSpecialControlsPhysics),
    fSetStepLimiterPhysics(right.fSetStepLimiterPhysics),
    fSetStackPopperPhysics(right.fSetStackPopperPhysics)
{
//
}

//_____________________________________________________________________________
TG4PhysicsListOptions::~TG4PhysicsListOptions() {
//
  //delete fExtDecayer;
       // fExtDecayer is deleted in G4Decay destructor
}

//
// operators
//

//_____________________________________________________________________________
TG4PhysicsListOptions& 
TG4PhysicsListOptions::operator=(const TG4PhysicsListOptions &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  fSetEMPhysics = right.fSetEMPhysics;
  fSetMuonPhysics = right.fSetMuonPhysics;
  fSetHadronPhysics = right.fSetHadronPhysics;
  fSetOpticalPhysics = right.fSetOpticalPhysics;
  fSetSpecialCutsPhysics = right.fSetSpecialCutsPhysics;
  fSetSpecialControlsPhysics = right.fSetSpecialControlsPhysics;
  fSetStackPopperPhysics = right.fSetStackPopperPhysics;

  return *this;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4PhysicsListOptions::SetEMPhysics(Bool_t value) 
{ 
  /// Swith on/off EM physics
  fSetEMPhysics = value; 
}

//_____________________________________________________________________________
void TG4PhysicsListOptions::SetMuonPhysics(Bool_t value) 
{ 
  /// Switch on/off muon physics
  fSetMuonPhysics = value; 
}

//_____________________________________________________________________________
void TG4PhysicsListOptions::SetHadronPhysics(Bool_t value) 
{ 
  /// Switch on/off hadron physics
  fSetHadronPhysics = value; 
}

//_____________________________________________________________________________
void TG4PhysicsListOptions::SetOpticalPhysics(Bool_t value) 
{ 
  /// Switch on/off optical physics
  fSetOpticalPhysics = value; 
}

//_____________________________________________________________________________
void TG4PhysicsListOptions::SetSpecialCutsPhysics(Bool_t value) 
{ 
  /// Switch on/off special cuts = cuts in energy defined via VMC
  fSetSpecialCutsPhysics = value; 
}

//_____________________________________________________________________________
void TG4PhysicsListOptions::SetSpecialControlsPhysics(Bool_t value) 
{ 
  /// Switch on/off special controls = process controls defined via VMC
  fSetSpecialControlsPhysics = value; 
}

//_____________________________________________________________________________
void TG4PhysicsListOptions::SetStepLimiterPhysics(Bool_t value) 
{ 
  /// Switch on/off step limiter
  fSetStepLimiterPhysics = value; 
}

//_____________________________________________________________________________
void TG4PhysicsListOptions::SetStackPopperPhysics(Bool_t value) 
{ 
  /// Switch on/off a process which pops from a stack user defined
  /// tracks and put them in tracking
  fSetStackPopperPhysics = value; 
}

//_____________________________________________________________________________
Bool_t TG4PhysicsListOptions::GetEMPhysics() const 
{
  /// Return EM physics control value
  return fSetEMPhysics;
}  

//_____________________________________________________________________________
Bool_t TG4PhysicsListOptions::GetMuonPhysics() const  
{
  /// Return Muon physics control value
  return fSetMuonPhysics;
}  

//_____________________________________________________________________________
Bool_t TG4PhysicsListOptions::GetHadronPhysics() const  
{
  /// Return Hadron physics control value
  return fSetHadronPhysics;
}  

//_____________________________________________________________________________
Bool_t TG4PhysicsListOptions::GetOpticalPhysics() const  
{
  /// Return Optical physics control value
  return fSetOpticalPhysics;
}  

//_____________________________________________________________________________
Bool_t TG4PhysicsListOptions::GetSpecialCutsPhysics() const  
{
  /// Return Special cuts physics control value
  return fSetSpecialCutsPhysics;
}  

//_____________________________________________________________________________
Bool_t TG4PhysicsListOptions::GetSpecialControlsPhysics() const  
{
  /// Return Special controls physics control value
  return fSetSpecialControlsPhysics;
}  

//_____________________________________________________________________________
Bool_t TG4PhysicsListOptions::GetStepLimiterPhysics() const  
{
  /// Return Step limiter physics control value
  return fSetStepLimiterPhysics;
}  

//_____________________________________________________________________________
Bool_t TG4PhysicsListOptions::GetStackPopperPhysics() const  
{
  /// Return Stack popper physics control value
  return fSetStackPopperPhysics;
}  
