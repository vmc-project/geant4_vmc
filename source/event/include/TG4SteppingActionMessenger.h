#ifndef TG4_STEPPING_ACTION_MESSENGER_H
#define TG4_STEPPING_ACTION_MESSENGER_H 

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SteppingActionMessenger.h
/// \brief Definition of the TG4SteppingActionMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4SteppingAction;

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

/// \ingroup event
/// \brief Messenger class that defines commands for TG4SteppingAction.
///
/// Implements commands:
/// - /mcTracking/loopVerbose [level]
/// - /mcTracking/maxNofSteps [nofSteps]
/// - /mcTracking/saveSecondariesInStep [true|false]
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4SteppingActionMessenger: public G4UImessenger
{
  public:
    TG4SteppingActionMessenger(TG4SteppingAction* steppingAction);
    virtual ~TG4SteppingActionMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    /// Not implemented
    TG4SteppingActionMessenger();
    /// Not implemented
    TG4SteppingActionMessenger(const TG4SteppingActionMessenger& right);
    /// Not implemented
    TG4SteppingActionMessenger& operator=(
                               const TG4SteppingActionMessenger& right);

    // data members
    TG4SteppingAction*     fSteppingAction;    ///< associated class  
    G4UIcmdWithAnInteger*  fLoopVerboseCmd;    ///< command: loopVerbose
    G4UIcmdWithAnInteger*  fMaxNofStepsCmd;    ///< command: maxNofSteps
    G4UIcmdWithABool*      fSaveSecondariesCmd;///< command: saveSecondaries
};

#endif //TG4_STEPPING_ACTION_MESSENGER_H
