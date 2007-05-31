// $Id: TG4PhysicsConstructorStackPopper.h,v 1.1 2007/05/10 14:44:53 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsConstructorStackPopper
/// \brief Constructor of stack popper special process
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PHYSICS_CONSTRUCTOR_STACK_POPPER_H
#define TG4_PHYSICS_CONSTRUCTOR_STACK_POPPER_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class TG4StackPopper;

class TG4PhysicsConstructorStackPopper: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorStackPopper(const G4String& name = "StackPopper");
    TG4PhysicsConstructorStackPopper(G4int verboseLevel,
                                     const G4String& name = "StackPopper");
    virtual ~TG4PhysicsConstructorStackPopper();
    
    // set methods
    void SetSelection(const G4String& selection);

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
    TG4StackPopper* fStackPopperProcess; // stack popper process
    G4String        fSelection;          // particles selection

  private:
    TG4PhysicsConstructorStackPopper(const TG4PhysicsConstructorStackPopper& right);
    TG4PhysicsConstructorStackPopper& operator=(const TG4PhysicsConstructorStackPopper& right);  
};

// inline functions

inline void TG4PhysicsConstructorStackPopper::SetSelection(const G4String& selection) {
  /// Set particles selection
  fSelection = selection;
}  

#endif //TG4_PHYSICS_CONSTRUCTOR_STACK_POPPERS_H

