#ifndef TG4_STEPPING_ACTION_H
#define TG4_STEPPING_ACTION_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SteppingAction.h
/// \brief Definition of the TG4SteppingAction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SteppingActionMessenger.h"

#include <G4UserSteppingAction.hh>

#include <globals.hh>

class TG4SpecialControlsV2;

class G4Track;
class G4Step;

/// \ingroup event
/// \brief Actions at each step
/// 
/// This class ensures the additional call to the sensitive detector
/// when track crosses a geometrical boundary.
/// It also enables to define a maximum number of steps
/// and takes care of stopping of a track when this number
/// is reached.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4SteppingAction : public G4UserSteppingAction 
{
  enum { 
    kMaxNofSteps = 30000,
    kMaxNofLoopSteps = 5
  };

  public:
    TG4SteppingAction();
    virtual ~TG4SteppingAction();
   
    // static access method
    static TG4SteppingAction* Instance();

    // methods
    virtual void SteppingAction(const G4Step* step);
                  // the following method should not
                  // be overwritten in a derived class
    virtual void UserSteppingAction(const G4Step* step);

    // set methods
    void SetLoopVerboseLevel(G4int level);
    void SetMaxNofSteps(G4int number);
    void SetSaveSecondaries(G4bool saveSecondaries);
    void SetSpecialControls(TG4SpecialControlsV2* specialControls);

    // get methods
    G4int GetLoopVerboseLevel() const;
    G4int GetMaxNofSteps() const;
    G4bool GetSaveSecondaries() const;

  protected:
    // methods
    void PrintTrackInfo(const G4Track* track) const;

  private:
    /// Not implemented
    TG4SteppingAction(const TG4SteppingAction& right);
    /// Not implemented
    TG4SteppingAction& operator=(const TG4SteppingAction& right);

    // static data members
    static TG4SteppingAction*   fgInstance; ///< this instance

    //
    // data members
    
    /// messenger    
    TG4SteppingActionMessenger  fMessenger;
    
    /// the special controls manager
    TG4SpecialControlsV2*  fSpecialControls;

    /// max number of allowed steps
    G4int  fMaxNofSteps;
    
    /// standard tracking verbose level
    G4int  fStandardVerboseLevel;
    
    /// tracking verbose level for looping particles
    G4int  fLoopVerboseLevel;
    
    /// counter of step in looping    
    G4int  fLoopStepCounter;
    
    /// control of saving secondaries
    G4bool fSaveSecondaries;
};

// inline methods

inline TG4SteppingAction* TG4SteppingAction::Instance() { 
  /// Return this instance
  return fgInstance; 
}

inline void TG4SteppingAction::SteppingAction(const G4Step* /*step*/) {
  /// Dummy stepping action that can be overriden
  /// in a user defined class
}  

inline void TG4SteppingAction::SetLoopVerboseLevel(G4int level) { 
  /// Set loop verbose level (applied when a track is looping) 
  fLoopVerboseLevel = level; 
}

inline void TG4SteppingAction::SetMaxNofSteps(G4int number) { 
  /// Set maximum number of steps allowed 
  fMaxNofSteps = number; 
}

inline void TG4SteppingAction::SetSpecialControls(TG4SpecialControlsV2* specialControls)  { 
  /// Set special controls manager
  fSpecialControls = specialControls; 
}

inline G4int TG4SteppingAction::GetMaxNofSteps() const { 
  /// Get maximum number of steps allowed 
  return fMaxNofSteps; 
}

inline G4int TG4SteppingAction::GetLoopVerboseLevel() const { 
  /// Get loop verbose level (applied when a track is looping) 
  return fLoopVerboseLevel; 
}

inline G4bool TG4SteppingAction::GetSaveSecondaries() const { 
  /// Return control for saving secondaries in the VMC stack
  return fSaveSecondaries; 
}

#endif //TG4_STEPPING_ACTION_H
