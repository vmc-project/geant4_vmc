// $Id: TG4EventActionMessenger.h,v 1.4 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup event
/// \class TG4EventActionMessenger
/// \brief Messenger class that defines commands for TG4EventAction.
///
/// Implements command
/// - /mcEvent/drawTracks [NONE, CHARGED, ALL]
///
/// \author I. Hrivnacova; IPN, Orsay

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
    virtual ~TG4EventActionMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4EventActionMessenger();
    TG4EventActionMessenger(const TG4EventActionMessenger& right);
    TG4EventActionMessenger& operator=(
                            const TG4EventActionMessenger& right);

    // data members
    TG4EventAction*        fEventAction;    //associated class
    G4UIdirectory*         fEventDirectory; //command directory
    G4UIcmdWithAString*    fDrawTracksCmd;  //command: drawTracks
};

#endif //TG4_EVENT_ACTION_MESSENGER_H
