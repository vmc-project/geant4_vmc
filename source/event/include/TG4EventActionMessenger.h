#ifndef TG4_EVENT_ACTION_MESSENGER_H
#define TG4_EVENT_ACTION_MESSENGER_H 

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4EventActionMessenger.h
/// \brief Definition of the TG4EventActionMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UImessenger.hh>

class TG4EventAction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

/// \ingroup event
/// \brief Messenger class that defines commands for TG4EventAction.
///
/// Implements command
/// - /mcEvent/drawTracks [NONE, CHARGED, ALL]
/// - /mcEvent/printMemory [true|false]
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4EventActionMessenger: public G4UImessenger
{
  public:
    TG4EventActionMessenger(TG4EventAction* eventAction);
    virtual ~TG4EventActionMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    /// Not implemented
    TG4EventActionMessenger();
    /// Not implemented
    TG4EventActionMessenger(const TG4EventActionMessenger& right);
    /// Not implemented
    TG4EventActionMessenger& operator=(
                            const TG4EventActionMessenger& right);

    // data members
    TG4EventAction*        fEventAction;    ///< associated class
    G4UIdirectory*         fEventDirectory; ///< command directory
    G4UIcmdWithAString*    fDrawTracksCmd;  ///< command: drawTracks
    G4UIcmdWithABool*      fPrintMemoryCmd; ///< command: printMemory
};

#endif //TG4_EVENT_ACTION_MESSENGER_H
