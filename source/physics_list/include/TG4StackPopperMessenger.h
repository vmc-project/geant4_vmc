// $Id: TG4StackPopperMessenger.h,v 1.1 2007/08/09 13:43:34 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4StackPopperMessenger
/// \brief Messenger class that defines commands for the stack popper
///        special process
///
/// Implements commands:
/// - /mcPhysics/setStackPopperSelection [particleName1 particleName2 ...]
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_STACK_POPPER_MESSENGER_H
#define TG4_STACK_POPPER_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4StackPopperPhysics;

class G4UIcmdWithAString;

class TG4StackPopperMessenger: public G4UImessenger
{
  public:
    TG4StackPopperMessenger(TG4StackPopperPhysics* stackPopperPhysics); 
    virtual ~TG4StackPopperMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4StackPopperMessenger();  
    TG4StackPopperMessenger(const TG4StackPopperMessenger& right);
    TG4StackPopperMessenger& operator=(const TG4StackPopperMessenger& right);

    // data members
    TG4StackPopperPhysics* fStackPopperPhysics;//associated class
    G4UIcmdWithAString*    fSetSelectionCmd;   //setStackPopperSelection command
};    

#endif //TG4_STACK_POPPER_MESSENGER_H
