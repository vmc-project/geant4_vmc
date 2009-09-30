#ifndef TG4_OPTICAL_PHYSICS_MESSENGER_H
#define TG4_OPTICAL_PHYSICS_MESSENGER_H 

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4OpticalPhysicsMessenger.h
/// \brief Definition of the TG4OpticalPhysicsMessenger class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

#include "TG4OpticalProcess.h"

class TG4OpticalPhysics;

class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

/// \ingroup physics_list
/// \brief Messenger class that defines commands for the optical physics
///
/// Implements commands:
/// - /mcPhysics/selectOpProcess  processName
/// - /mcPhysics/setOpProcessActivation  true|false
/// - /mcPhysics/setOpProcessVerbose  verboseLevel
/// - /mcPhysics/setCerenkovMaxPhotons maxNofPhotons
/// - /mcPhysics/setCerenkovMaxBetaChange maxBetaChange
/// - /mcPhysics/setScintillationYieldFactor yieldFactor
/// - /mcPhysics/setOpticalSurfaceModel glisur|unified
/// - /mcPhysics/setTrackSecondariesFirst true|false
///
/// \author I. Hrivnacova; IPN Orsay

class TG4OpticalPhysicsMessenger: public G4UImessenger
{
  public:
    TG4OpticalPhysicsMessenger(TG4OpticalPhysics* opticalPhysics); 
    virtual ~TG4OpticalPhysicsMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    /// Not implemented
    TG4OpticalPhysicsMessenger();  
    /// Not implemented
    TG4OpticalPhysicsMessenger(const TG4OpticalPhysicsMessenger& right);
    /// Not implemented
    TG4OpticalPhysicsMessenger& operator=(const TG4OpticalPhysicsMessenger& right);

    //
    // data members
    
    /// associated class
    TG4OpticalPhysics*     fOpticalPhysics;
    
    /// selected optical process
    TG4OpticalProcess      fSelectedProcess;
    
    /// selectOpProcess command   
    G4UIcmdWithAString*    fSelectOpProcessCmd;                                               

    /// setCerenkovActivation command   
    G4UIcmdWithABool*      fSetOpProcessActivationCmd;                                               

    /// setCerenkovVerbose command   
    G4UIcmdWithAnInteger*  fSetOpProcessVerboseCmd;                                               

    /// setCerenkovMaxPhotons command   
    G4UIcmdWithAnInteger*  fSetCerenkovMaxPhotonsCmd;                                               

    /// setCerenkovMaxBetaChange command   
    G4UIcmdWithADouble*    fSetCerenkovMaxBetaChangeCmd;                                               

    /// setScintillationYieldFactor command   
    G4UIcmdWithADouble*    fSetScintillationYieldFactorCmd;                                               

    /// setOpticalSurfaceModel command   
    G4UIcmdWithAString*    fSetOpticalSurfaceModelCmd;                                               

    /// setTrackSecondariesFirst command   
    G4UIcmdWithABool*      fSetTrackSecondariesFirstCmd;                                               
};    

#endif //TG4_OPTICAL_PHYSICS_MESSENGER_H
