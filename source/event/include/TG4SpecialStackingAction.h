#ifndef TG4_STACKING_ACTION_H
#define TG4_STACKING_ACTION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SpecialStackingAction.h
/// \brief Definition of the TG4SpecialStackingAction class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"

#include <G4UserStackingAction.hh>
#include <globals.hh>

class G4Track;
class G4TrackStack;

/// \ingroup event
/// \brief Defines a special stacking mechanism
///
/// Class that defines a special stacking mechanism,
/// with which all secondaries produced by a primary particle
/// and its daughters are tracked before starting a new primary
/// (activated by default).
/// The class is also used for skipping neutrina
/// (not activated by default).
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SpecialStackingActionMessenger.h"

class TG4SpecialStackingAction : public G4UserStackingAction,
                                 public TG4Verbose
{
  public:
    TG4SpecialStackingAction();
    virtual ~TG4SpecialStackingAction();

    // methods
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track);
    void NewStage();
    void PrepareNewEvent();

    // set method
    void SetSkipNeutrino(G4bool value);
    void SetWaitPrimary(G4bool value);

    // get method
    G4bool GetSkipNeutrino() const;
    G4bool GetWaitPrimary() const;

  private:
    /// Not implemented
    TG4SpecialStackingAction(const TG4SpecialStackingAction& right);
    /// Not implemented
    TG4SpecialStackingAction& operator=(const TG4SpecialStackingAction& right);

    // data members
    TG4SpecialStackingActionMessenger  fMessenger; ///< messenger
    /// Stage number
    G4int   fStage;
    /// Option to skip tracking of all neutrina
    G4bool  fSkipNeutrino;
    /// Option to let the next primary wait until all secondaries of previous primary are tracked
    G4bool  fWaitPrimary;
};

// inline functions

/// Set the option for skipping neutrino
inline void TG4SpecialStackingAction::SetSkipNeutrino(G4bool value)
{ fSkipNeutrino = value; }

/// Set the option to let the next primary wait until all secondaries of previous primary are tracked
inline void TG4SpecialStackingAction::SetWaitPrimary(G4bool value)
{ fWaitPrimary = value; }

/// Return the option for skipping neutrino
inline G4bool TG4SpecialStackingAction::GetSkipNeutrino() const
{ return fSkipNeutrino; }

/// Return the option for skipping neutrino
inline G4bool TG4SpecialStackingAction::GetWaitPrimary() const
{ return fWaitPrimary; }

#endif //TG4_STACKING_ACTION_H

