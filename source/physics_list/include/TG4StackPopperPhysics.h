// $Id: TG4StackPopperPhysics.h,v 1.2 2007/05/31 10:24:32 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4StackPopperPhysics
/// \brief The builder for stack popper process
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_STACK_POPPER_PHYSICS_H
#define TG4_STACK_POPPER_PHYSICS_H

#include "TG4VPhysicsConstructor.h"
#include "TG4StackPopperMessenger.h"

#include <globals.hh>

class TG4StackPopper;

class TG4StackPopperPhysics : public TG4VPhysicsConstructor
{
  public:
    TG4StackPopperPhysics(const G4String& name = "StackPopper");
    TG4StackPopperPhysics(G4int verboseLevel,
                          const G4String& name = "StackPopper");
    virtual ~TG4StackPopperPhysics();
    
    // set methods
    void SetSelection(const G4String& selection);

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    TG4StackPopperPhysics(const TG4StackPopperPhysics& right);
    TG4StackPopperPhysics& operator=(const TG4StackPopperPhysics& right);
    
    // data members
    TG4StackPopperMessenger  fMessenger;  // messenger
    TG4StackPopper*  fStackPopperProcess; // stack popper process
    G4String         fSelection;          // particles selection

};

// inline functions

inline void TG4StackPopperPhysics::SetSelection(const G4String& selection) {
  /// Set particles selection
  fSelection = selection;
}  

#endif //TG4_STACK_POPPER_PHYSICS_H

