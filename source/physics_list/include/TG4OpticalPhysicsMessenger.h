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

class TG4OpticalPhysics;

class G4UIcmdWithAnInteger;

/// \ingroup physics_list
/// \brief Messenger class that defines commands for the optical physics
///
/// Implements commands:
/// - /mcPhysics/setCerenkovMaxPhotons [maxNofPhotons]
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
    
    /// setCerenkovMaxPhotons command   
    G4UIcmdWithAnInteger*  fSetCerenkovMaxPhotonsCmd;                                               
};    

#endif //TG4_OPTICAL_PHYSICS_MESSENGER_H
