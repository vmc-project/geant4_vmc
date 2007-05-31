// $Id: TG4SteppingActionMessenger.h,v 1.5 2007/05/22 12:26:42 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup event
/// \class TG4SteppingActionMessenger
/// \brief Messenger class that defines commands for TG4SteppingAction.
///
/// Implements commands:
/// - /mcTracking/loopVerbose [level]
/// - /mcTracking/maxNofSteps [nofSteps]
/// - /mcTracking/saveSecondariesInStep [true|false]
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_STEPPING_ACTION_MESSENGER_H
#define TG4_STEPPING_ACTION_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4SteppingAction;

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class TG4SteppingActionMessenger: public G4UImessenger
{
  public:
    TG4SteppingActionMessenger(TG4SteppingAction* steppingAction);
    virtual ~TG4SteppingActionMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4SteppingActionMessenger();
    TG4SteppingActionMessenger(const TG4SteppingActionMessenger& right);
    TG4SteppingActionMessenger& operator=(
                               const TG4SteppingActionMessenger& right);

    // data members
    TG4SteppingAction*     fSteppingAction; //associated class  
    G4UIcmdWithAnInteger*  fLoopVerboseCmd; //command: loopVerbose
    G4UIcmdWithAnInteger*  fMaxNofStepsCmd; //command: maxNofSteps
    G4UIcmdWithABool*      fSaveSecondariesCmd;//command: saveSecondaries
};

#endif //TG4_STEPPING_ACTION_MESSENGER_H
