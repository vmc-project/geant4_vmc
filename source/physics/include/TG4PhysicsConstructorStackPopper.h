// $Id: TG4PhysicsConstructorStackPopper.h,v 1.4 2006/12/12 16:21:16 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorStackPopper
/// \brief Constructor of stack popper special process
///
/// Author: I. Hrivnacova

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

