#ifndef TG4_STATE_MANAGER_H
#define TG4_STATE_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4StateManager.h
/// \brief Definition of the TG4StateManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ApplicationState.h"
#include "TG4Verbose.h"

#include "globals.hh"

class TG4StateManager;

/// \ingroup geometry
/// \brief The manager class for application state
///
/// \author I. Hrivnacova; IPN, Orsay

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
    /// Not implemented
    TG4StateManager(const TG4StateManager& right);
    /// Not implemented
    TG4StateManager& operator=(const TG4StateManager& right);

    // static data members
    static G4ThreadLocal TG4StateManager* fgInstance; ///< this instance

    // data members
    TG4ApplicationState  fCurrentState; ///< the current application state
    TG4ApplicationState  fPreviousState;///< the previous application state
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

