// $Id: TG4SteppingActionMessenger.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup event
//
/// \class TG4SteppingActionMessenger
/// 
/// Messenger class that defines commands for TG4SteppingAction.
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
    // --> protected
    // TG4SteppingActionMessenger();
    // TG4SteppingActionMessenger(const TG4SteppingActionMessenger& right);
    virtual ~TG4SteppingActionMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  protected:
    TG4SteppingActionMessenger();
    TG4SteppingActionMessenger(const TG4SteppingActionMessenger& right);

    // operators
    TG4SteppingActionMessenger& operator=(
                            const TG4SteppingActionMessenger& right);

  private:
    // data members
    TG4SteppingAction*     fSteppingAction; //associated class  
    G4UIcmdWithAnInteger*  fLoopVerboseCmd; //command: loopVerbose
    G4UIcmdWithAnInteger*  fMaxNofStepsCmd; //command: maxNofSteps
};

#endif //TG4_STEPPING_ACTION_MESSENGER_H
