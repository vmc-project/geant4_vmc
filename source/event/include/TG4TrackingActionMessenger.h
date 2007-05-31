// $Id: TG4TrackingActionMessenger.h,v 1.5 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup event
/// \class TG4TrackingActionMessenger
/// \brief Messenger class that defines commands for TG4TrackingAction.
///
/// Implements command
/// - /mcTracking/newVerbose [level]
/// - /mcTracking/newVerboseTrack [trackID]
/// - /mcTracking/saveSecondaries [true|false]
/// 
/// \author I. Hrivnacova; IPN, Orsay
 
#ifndef TG4_TRACKING_ACTION_MESSENGER_H
#define TG4_TRACKING_ACTION_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4TrackingAction;

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class TG4TrackingActionMessenger: public G4UImessenger
{
  public:
    TG4TrackingActionMessenger(TG4TrackingAction* trackingAction);
    virtual ~TG4TrackingActionMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4TrackingActionMessenger();
    TG4TrackingActionMessenger(const TG4TrackingActionMessenger& right);
    TG4TrackingActionMessenger& operator=(
                               const TG4TrackingActionMessenger& right);

    // data members
    TG4TrackingAction*     fTrackingAction;    //associated class 
    G4UIdirectory*         fTrackingDirectory; //command directory
    G4UIcmdWithAnInteger*  fNewVerboseCmd;     //command: newVerbose
    G4UIcmdWithAnInteger*  fNewVerboseTrackCmd;//command: newVerboseTrack
    G4UIcmdWithABool*      fSaveSecondariesCmd;//command: saveSecondaries
};

#endif //TG4_TRACKING_ACTION_MESSENGER_H
