// $Id: TG4EventAction.h,v 1.5 2006/04/12 10:38:21 brun Exp $
/// \ingroup event
//
/// \class TG4EventAction
/// \brief Actions at the beginning and the end of event.
///
/// Author: I. Hrivnacova

#ifndef TG4_EVENT_ACTION_H
#define TG4_EVENT_ACTION_H 

#include "TG4Verbose.h"
#include "TG4EventActionMessenger.h"

#include <TStopwatch.h>

#include <G4UserEventAction.hh>
#include <globals.hh>

class G4Event;
class G4PrimaryVertex;
class G4PrimaryParticle;

class TG4EventAction : public G4UserEventAction,
                       public TG4Verbose
{
  public:
    TG4EventAction();
    virtual ~TG4EventAction();
    
    // methods
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    
    // set methods
    void SetDrawFlag(G4String drawFlag);
    
    // get methods
    G4String GetDrawFlag() const;
    
  private:
    TG4EventAction(const TG4EventAction& right);
    TG4EventAction& operator=(const TG4EventAction& right);

    // methods 
    void DisplayEvent(const G4Event* event) const;
    void PrimaryToStack(const G4PrimaryVertex* vertex,
                        const G4PrimaryParticle* particle) const;
  
    // data members
    TG4EventActionMessenger   fMessenger; //messenger
    TStopwatch                fTimer;     //timer
    G4String                  fDrawFlag;  //control drawing of the event
};

// inline methods

inline void TG4EventAction::SetDrawFlag(G4String drawFlag) { 
  /// Set control for drawing event: "CHARGED" (default), "ALL"
  fDrawFlag = drawFlag; 
}

inline G4String TG4EventAction::GetDrawFlag() const {   
  /// Return control for drawing event
  return fDrawFlag;
}

#endif //TG4_EVENT_ACTION_H

    
