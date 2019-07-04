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
/// with which particles are popped from the stack in "families".
/// The primary particles are first reclassified into
/// the postpone stack, a new primary is transfered to the urgent
/// stack only after all secondaries of its precedent primary
/// are tracked.                                                             \n
/// The secondary particles are classified into the waiting stack,
/// which is transfered to the urgent stack when the urgent stack
/// is empty.                                                                \n
/// This ensures that all daughters of any particle are tracked
/// subsequently and get successive track IDs:                               \n
/// n, n+1, n+2, n+3, ..., n+m
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SpecialStackingActionMessenger.h"

class TG4SpecialStackingAction : public G4UserStackingAction, public TG4Verbose
{
 public:
  TG4SpecialStackingAction();
  virtual ~TG4SpecialStackingAction();

  // methods
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track);
  void NewStage();
  void PrepareNewEvent();

  // set method
  void SetSkipNeutrino(G4bool skipNeutrino);

  // get method
  G4bool GetSkipNeutrino() const;

 private:
  /// Not implemented
  TG4SpecialStackingAction(const TG4SpecialStackingAction& right);
  /// Not implemented
  TG4SpecialStackingAction& operator=(const TG4SpecialStackingAction& right);

  // data members
  TG4SpecialStackingActionMessenger fMessenger; ///< messenger
  G4int fStage;                                 ///< stage number
  G4bool fSkipNeutrino; ///< option to skip tracking of neutrino
};

// inline functions

/// Set the option for skipping neutrino
inline void TG4SpecialStackingAction::SetSkipNeutrino(G4bool skipNeutrino)
{
  fSkipNeutrino = skipNeutrino;
}

/// Return the option for skipping neutrino
inline G4bool TG4SpecialStackingAction::GetSkipNeutrino() const
{
  return fSkipNeutrino;
}

#endif // TG4_STACKING_ACTION_H
