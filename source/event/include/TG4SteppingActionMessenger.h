// $Id: TG4SteppingActionMessenger.h,v 1.3 2005/09/01 10:04:32 brun Exp $
/// \ingroup event
//
/// \class TG4SteppingActionMessenger
/// \brief Messenger class that defines commands for TG4SteppingAction.
///
/// Author: I. Hrivnacova

#ifndef TG4_STEPPING_ACTION_MESSENGER_H
#define TG4_STEPPING_ACTION_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4SteppingAction;

class G4UIdirectory;
class G4UIcmdWithAnInteger;

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
};

#endif //TG4_STEPPING_ACTION_MESSENGER_H
