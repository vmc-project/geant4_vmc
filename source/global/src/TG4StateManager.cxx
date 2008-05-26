// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4StateManager.cxx
/// \brief Implementation of the TG4StateManager class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4StateManager.h"
#include "TG4Globals.h"

TG4StateManager* TG4StateManager::fgInstance = 0;

//_____________________________________________________________________________
G4String TG4StateManager::GetStateName(TG4ApplicationState state)
{
/// Return string for given application state

  switch ( state ) {
    case kPreInit:              return "PreInit";
    case kConstructGeometry:    return "ConstructGeometry";
    case kConstructOpGeometry:  return "ConstructOpGeometry";
    case kMisalignGeometry:     return "MisalignGeometry";
    case kInitGeometry:         return "InitGeometry";
    case kAddParticles:         return "AddParticles";
    case kAddIons:              return "AddIons";
    case kInEvent:              return "InEvent";
    case kNotInApplication:   
    default:                    return "NotInApplication";
  }
  return "Undefined";
}  


//_____________________________________________________________________________
TG4StateManager::TG4StateManager() 
  : TG4Verbose("stateManager"),
    fCurrentState(kNotInApplication),
    fPreviousState(kNotInApplication)
  
{
/// Standard constructor

  if ( fgInstance ) {
    TG4Globals::Exception(
      "TG4StateManager", "TG4StateManager:",
      "Cannot create two instances of singleton.");
  }
  
  fgInstance = this;
}

//_____________________________________________________________________________
TG4StateManager::~TG4StateManager() 
{
/// Destructor

  fgInstance = 0;
}

//_____________________________________________________________________________
void TG4StateManager::SetNewState(TG4ApplicationState state) 
{
/// Set application state

  if ( VerboseLevel() > 1 ) {
    G4cout << "**** TG4StateManager::SetNewState: " 
           << GetStateName(state) << "****" << G4endl;
  }         
  
  fPreviousState = fCurrentState;
  fCurrentState = state;
}  
  


