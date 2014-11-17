#ifndef TG4_TRACKING_ACTION_MESSENGER_H
#define TG4_TRACKING_ACTION_MESSENGER_H 

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4TrackingActionMessenger.h
/// \brief Definition of the TG4TrackingActionMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4TrackingAction;

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

/// \ingroup event
/// \brief Messenger class that defines commands for TG4TrackingAction.
///
/// Implements command
/// - /mcTracking/newVerbose [level]
/// - /mcTracking/newVerboseTrack [trackID]
/// - /mcTracking/saveSecondaries [DoNotSave|SaveInPreTrack|SaveInStep]
/// - /mcTracking/saveDynamicCharge [true|false]
/// 
/// \author I. Hrivnacova; IPN, Orsay
 
class TG4TrackingActionMessenger: public G4UImessenger
{
  public:
    TG4TrackingActionMessenger(TG4TrackingAction* trackingAction);
    virtual ~TG4TrackingActionMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    /// Not implemented
    TG4TrackingActionMessenger();
    /// Not implemented
    TG4TrackingActionMessenger(const TG4TrackingActionMessenger& right);
    /// Not implemented
    TG4TrackingActionMessenger& operator=(
                               const TG4TrackingActionMessenger& right);

    // data members
    TG4TrackingAction*     fTrackingAction;    ///< associated class 
    G4UIdirectory*         fTrackingDirectory; ///< command directory
    G4UIcmdWithAnInteger*  fNewVerboseCmd;     ///< command: newVerbose
    G4UIcmdWithAnInteger*  fNewVerboseTrackCmd;///< command: newVerboseTrack
    G4UIcmdWithAString*    fSaveSecondariesCmd;///< command: saveSecondaries
    G4UIcmdWithABool*      fSaveDynamicChargeCmd; ///< command: saveDynamicCharge
};

#endif //TG4_TRACKING_ACTION_MESSENGER_H
