#ifndef TG4_SPECIAL_CONTROLS_H
#define TG4_SPECIAL_CONTROLS_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SpecialControls.h
/// \brief Definition of the TG4SpecialControls class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"
#include "TG4Globals.h"
#include "TG4G3Control.h"

#include <G4VProcess.hh>
#include <G4ProcessVector.hh>
#include <globals.hh>
#include <vector>

/// \ingroup physics
/// \brief Special process that applies the G3 process controls.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4SpecialControls : public G4VProcess,
                           public TG4Verbose 
{
  /// The action which are passed from PostStepGetPIL to PostStepDoIt
  enum Switch { 
    kSwitch,  ///< switch the process activation
    kReswitch,///< switch the process activation back 
    kUnswitch ///< do not switch the process activation
  };

  public:     
    TG4SpecialControls(const G4String& processName ="specialControl" );
    virtual ~TG4SpecialControls();

    // methods

    virtual G4double PostStepGetPhysicalInteractionLength(
                         const G4Track& track, G4double previousStepSize, 
                         G4ForceCondition* condition);

    virtual G4VParticleChange* PostStepDoIt(const G4Track& track, 
                                   const G4Step& step);

                     /// Not implemented
    virtual G4double AlongStepGetPhysicalInteractionLength(
                         const G4Track& /*track*/, G4double /*previousStepSize*/,
                         G4double /*currentMinimumStep*/, G4double& /*proposedSafety*/,
                         G4GPILSelection* /*selection*/)
                         { return -1.0; }

                     /// Not implemented
    virtual G4VParticleChange* AlongStepDoIt(
                         const G4Track& /*track*/, const G4Step& /*step*/)
                         { return 0; }

                     /// Not implemented
    virtual G4double AtRestGetPhysicalInteractionLength(
                         const G4Track& /*track*/, G4ForceCondition* /*condition*/)
                         { return -1.0; }

                     /// Not implemented
    virtual G4VParticleChange* AtRestDoIt(
                         const G4Track& /*track*/, const G4Step& /*step*/)
                         { return 0; }

  private:  
    /// Not implemented
    TG4SpecialControls(const TG4SpecialControls& right);
    /// Not implemented
    TG4SpecialControls& operator = (const TG4SpecialControls& right);

    // methods
    virtual void Reset();                             
    
    // data members

    /// Directive passed from PostStepGetPIL to PostStepDoIt
    Switch fSwitchControls; 

    /// Vector of the processes the activation of which is changed by this process
    G4ProcessVector  fSwitchedProcesses; 

    /// Vector for storing the current values of the processes activation 
    TG4boolVector  fSwitchedControls;

    /// The last processed track ID
    G4int fLastTrackID;
};

#endif //TG4_SPECIAL_CONTROLS_H

