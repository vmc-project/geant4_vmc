// $Id: TG4EventAction.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: event
//
// Author: I. Hrivnacova
//
// Class TG4EventAction
// --------------------
// Class that defines actions at the beginning and the end of event.

#ifndef TG4_EVENT_ACTION_H
#define TG4_EVENT_ACTION_H 

#include "TG4Verbose.h"
#include "TG4EventActionMessenger.h"

#include <TStopwatch.h>

#include <G4UserEventAction.hh>
#include <globals.hh>

class G4Event;

class TG4EventAction : public G4UserEventAction,
                       public TG4Verbose
{
  public:
    TG4EventAction();
    // --> protected
    // TG4EventAction(const TG4EventAction& right);
    virtual ~TG4EventAction();
    
    // methods
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    
    // set methods
    void SetDrawFlag(G4String drawFlag);
    
    // get methods
    G4String GetDrawFlag() const;
    
  protected:
    TG4EventAction(const TG4EventAction& right);

    // operators
    TG4EventAction& operator=(const TG4EventAction& right);

  private:
    // methods 
    void DisplayEvent(const G4Event* event) const;
  
    // data members
    TG4EventActionMessenger   fMessenger; //messenger
    TStopwatch                fTimer;     //timer
    G4String                  fDrawFlag;  //control drawing of the event
};

// inline methods

inline void TG4EventAction::SetDrawFlag(G4String drawFlag)
{ fDrawFlag = drawFlag; }

inline G4String TG4EventAction::GetDrawFlag() const
{ return fDrawFlag; }

#endif //TG4_EVENT_ACTION_H

    
