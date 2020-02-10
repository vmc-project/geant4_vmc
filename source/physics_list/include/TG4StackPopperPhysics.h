#ifndef TG4_STACK_POPPER_PHYSICS_H
#define TG4_STACK_POPPER_PHYSICS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4StackPopperPhysics.h
/// \brief Definition of the TG4StackPopperPhysics class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4StackPopperMessenger.h"
#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class TG4StackPopper;

/// \ingroup physics_list
/// \brief The builder for stack popper process
///
/// \author I. Hrivnacova; IPN Orsay

class TG4StackPopperPhysics : public TG4VPhysicsConstructor
{
 public:
  TG4StackPopperPhysics(const G4String& name = "StackPopper");
  TG4StackPopperPhysics(
    G4int theVerboseLevel, const G4String& name = "StackPopper");
  virtual ~TG4StackPopperPhysics();

  // set methods
  void SetSelection(const G4String& selection);

 protected:
  // methods
  // construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();

 private:
  /// Not implemented
  TG4StackPopperPhysics(const TG4StackPopperPhysics& right);
  /// Not implemented
  TG4StackPopperPhysics& operator=(const TG4StackPopperPhysics& right);

  // data members
  TG4StackPopperMessenger fMessenger;  ///< messenger
  TG4StackPopper* fStackPopperProcess; ///< stack popper process
  G4String fSelection;                 ///< particles selection
};

// inline functions

inline void TG4StackPopperPhysics::SetSelection(const G4String& selection)
{
  /// Set particles selection
  fSelection = selection;
}

#endif // TG4_STACK_POPPER_PHYSICS_H
