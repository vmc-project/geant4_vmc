// $Id: TG4EventActionMessenger.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup event
//
/// \class TG4EventActionMessenger
/// 
/// Messenger class that defines commands for TG4EventAction.
///
/// Author: I. Hrivnacova

#ifndef TG4_EVENT_ACTION_MESSENGER_H
#define TG4_EVENT_ACTION_MESSENGER_H 

#include <G4UImessenger.hh>

class TG4EventAction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class TG4EventActionMessenger: public G4UImessenger
{
  public:
    TG4EventActionMessenger(TG4EventAction* eventAction);
    // --> protected
    // TG4EventActionMessenger();
    // TG4EventActionMessenger(const TG4EventActionMessenger& right);
    virtual ~TG4EventActionMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  protected:
    TG4EventActionMessenger();
    TG4EventActionMessenger(const TG4EventActionMessenger& right);

    // operators
    TG4EventActionMessenger& operator=(
                            const TG4EventActionMessenger& right);

  private:
    // data members
    TG4EventAction*        fEventAction;    //associated class
    G4UIdirectory*         fEventDirectory; //command directory
    G4UIcmdWithAString*    fDrawTracksCmd;  //command: drawTracks
};

#endif //TG4_EVENT_ACTION_MESSENGER_H
