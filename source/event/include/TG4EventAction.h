#ifndef TG4_EVENT_ACTION_H
#define TG4_EVENT_ACTION_H 

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4EventAction.h
/// \brief Definition of the TG4EventAction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"
#include "TG4EventActionMessenger.h"

#include <TStopwatch.h>

#include <G4UserEventAction.hh>
#include <globals.hh>

class G4Event;

/// \ingroup event
/// \brief Actions at the beginning and the end of event.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4EventAction : public G4UserEventAction,
                       public TG4Verbose
{
  public:
    TG4EventAction();
    virtual ~TG4EventAction();
    
    // methods
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    
    // set methods
    void SetPrintMemory(G4bool printMemory);
    void SetSaveRandomStatus(G4bool saveRandomStatus);
    
    // get methods
    G4bool  GetPrintMemory() const;
    G4bool  GetSaveRandomStatus() const;

  private:
    /// Not implemented
    TG4EventAction(const TG4EventAction& right);
    /// Not implemented
    TG4EventAction& operator=(const TG4EventAction& right);

    // data members
    TG4EventActionMessenger   fMessenger; ///< messenger
    TStopwatch  fTimer;          ///< timer
    G4bool      fPrintMemory;    ///< control for printing memory usage 

    /// control for saving random engine status for each event
    G4bool      fSaveRandomStatus; 
};

// inline methods

inline void TG4EventAction::SetPrintMemory(G4bool printMemory){ 
  /// Set option for printing memory usage
  fPrintMemory = printMemory; 
}

inline G4bool TG4EventAction::GetPrintMemory() const {   
  /// Return the option for printing memory usage
  return fPrintMemory;
}

inline G4bool TG4EventAction::GetSaveRandomStatus() const {
  /// Return the option for printing memory usage
  return fSaveRandomStatus;
}

inline void TG4EventAction::SetSaveRandomStatus(G4bool saveRandomStatus) {
  /// Set option for saving random engine status for each event
  fSaveRandomStatus = saveRandomStatus;
}

#endif //TG4_EVENT_ACTION_H

    
