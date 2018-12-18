#ifndef TG4_SPECIAL_STACKING_ACTION_MESSENGER_H
#define TG4_SPECIAL_STACKING_ACTION_MESSENGER_H 

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SpecialStackingActionMessenger.h
/// \brief Definition of the TG4SpecialStackingActionMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4SpecialStackingAction;

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

/// \ingroup event
/// \brief Messenger class that defines commands for TG4StackingAction.
///
/// Implements command:
/// - /mcTracking/skipNeutrino [true|false]
/// - /mcTracking/waitPrimary [true|false]
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4SpecialStackingActionMessenger: public G4UImessenger
{
  public:
    TG4SpecialStackingActionMessenger(TG4SpecialStackingAction* stackingAction);
    virtual ~TG4SpecialStackingActionMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    /// Not implemented
    TG4SpecialStackingActionMessenger();
    /// Not implemented
    TG4SpecialStackingActionMessenger(
                               const TG4SpecialStackingActionMessenger& right);
    /// Not implemented
    TG4SpecialStackingActionMessenger& operator=(
                               const TG4SpecialStackingActionMessenger& right);

    // data members
    TG4SpecialStackingAction*  fStackingAction;  ///< associated class  
    G4UIcmdWithABool*          fSkipNeutrinoCmd; ///< command: skipNeutrino
    G4UIcmdWithABool*          fWaitPrimaryCmd;  ///< command: waitPrimary
};

#endif //TG4_SPECIAL_STACKING_ACTION_MESSENGER_H
