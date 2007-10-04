// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4OpticalPhysicsMessenger
/// \brief Messenger class that defines commands for the optical physics
///
/// Implements commands:
/// - /mcPhysics/setCerenkovMaxPhotons [maxNofPhotons]
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_OPTICAL_PHYSICS_MESSENGER_H
#define TG4_OPTICAL_PHYSICS_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4OpticalPhysics;

class G4UIcmdWithAnInteger;

class TG4OpticalPhysicsMessenger: public G4UImessenger
{
  public:
    TG4OpticalPhysicsMessenger(TG4OpticalPhysics* opticalPhysics); 
    virtual ~TG4OpticalPhysicsMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4OpticalPhysicsMessenger();  
    TG4OpticalPhysicsMessenger(const TG4OpticalPhysicsMessenger& right);
    TG4OpticalPhysicsMessenger& operator=(const TG4OpticalPhysicsMessenger& right);

    // data members
    TG4OpticalPhysics*     fOpticalPhysics;  //associated class
    G4UIcmdWithAnInteger*  fSetCerenkovMaxPhotonsCmd; 
                                             //setCerenkovMaxPhotons command                                                  
};    

#endif //TG4_OPTICAL_PHYSICS_MESSENGER_H
