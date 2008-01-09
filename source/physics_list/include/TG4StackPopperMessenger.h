#ifndef TG4_STACK_POPPER_MESSENGER_H
#define TG4_STACK_POPPER_MESSENGER_H 

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4StackPopperMessenger.h
/// \brief Definition of the TG4StackPopperMessenger class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4StackPopperPhysics;

class G4UIcmdWithAString;

/// \ingroup physics_list
/// \brief Messenger class that defines commands for the stack popper
///        special process
///
/// Implements commands:
/// - /mcPhysics/setStackPopperSelection [particleName1 particleName2 ...]
///
/// \author I. Hrivnacova; IPN Orsay

class TG4StackPopperMessenger: public G4UImessenger
{
  public:
    TG4StackPopperMessenger(TG4StackPopperPhysics* stackPopperPhysics); 
    virtual ~TG4StackPopperMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    /// Not implemented
    TG4StackPopperMessenger();  
    /// Not implemented
    TG4StackPopperMessenger(const TG4StackPopperMessenger& right);
    /// Not implemented
    TG4StackPopperMessenger& operator=(const TG4StackPopperMessenger& right);

    //
    // data members
    
    /// associated class
    TG4StackPopperPhysics* fStackPopperPhysics;
    
    /// setStackPopperSelection command
    G4UIcmdWithAString*    fSetSelectionCmd;
};    

#endif //TG4_STACK_POPPER_MESSENGER_H
