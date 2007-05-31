// $Id: TG4StateManager.h,v 1.1 2007/02/06 11:06:34 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup geometry
/// \class TG4StateManager
/// \brief The manager class for application state
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_STATE_MANAGER_H
#define TG4_STATE_MANAGER_H

#include "TG4ApplicationState.h"
#include "TG4Verbose.h"

#include "globals.hh"

class TG4StateManager;

class TG4StateManager : public TG4Verbose
{
  public:
    TG4StateManager();
    virtual ~TG4StateManager();

    // static access method
    static TG4StateManager* Instance();
    static G4String GetStateName(TG4ApplicationState state);

    // methods
    void SetNewState(TG4ApplicationState state);
    TG4ApplicationState GetCurrentState() const;
    TG4ApplicationState GetPreviousState() const;
     
  private:
    TG4StateManager(const TG4StateManager& right);
    TG4StateManager& operator=(const TG4StateManager& right);

    // static data members
    static TG4StateManager* fgInstance;

    // data members
    TG4ApplicationState  fCurrentState;  
    TG4ApplicationState  fPreviousState; 
};

// inline methods

inline TG4StateManager* TG4StateManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}

inline  TG4ApplicationState TG4StateManager::GetCurrentState() const {
  /// Return application state
  return fCurrentState;
}  

inline  TG4ApplicationState TG4StateManager::GetPreviousState() const {
  /// Return application state
  return fPreviousState;
}  

#endif //TG4_STATE_MANAGER_H

