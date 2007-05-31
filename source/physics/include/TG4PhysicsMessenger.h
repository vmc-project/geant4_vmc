// $Id: TG4PhysicsMessenger.h,v 1.5 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsMessenger
/// \brief Messenger class that defines commands for the physics manager.
///
/// Implements commands:
/// - /mcPhysics/printProcessMCMap
/// - /mcPhysics/printProcessControlMap
/// - /mcPhysics/printVolumeLimits [volName]
/// - /mcPhysics/printGeneralCuts
/// - /mcPhysics/printGeneralControls
/// 
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PHYSICS_MESSENGER_H
#define TG4_PHYSICS_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4PhysicsManager;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class TG4PhysicsMessenger: public G4UImessenger
{
  public:
    TG4PhysicsMessenger(TG4PhysicsManager* physicsManager); 
    virtual ~TG4PhysicsMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4PhysicsMessenger();  
    TG4PhysicsMessenger(const TG4PhysicsMessenger& right);
    TG4PhysicsMessenger& operator=(const TG4PhysicsMessenger& right);

    // data members
    TG4PhysicsManager* fPhysicsManager;       //associated class
    G4UIdirectory*     fDirectory;            //command directory
    
    G4UIcmdWithoutParameter*  fPrintProcessMCMapCmd; //.
                                              //printProcessMCMap command
    G4UIcmdWithoutParameter*  fPrintProcessControlMapCmd; //.
                                              //printProcessControlsMap command
    G4UIcmdWithAString*       fPrintVolumeLimitsCmd; //.
                                              //printVolumeLimits command
    G4UIcmdWithoutParameter*  fPrintGeneralCutsCmd; //.
                                              //printGeneralCuts command
    G4UIcmdWithoutParameter*  fPrintGeneralControlsCmd; //.
                                              //printGeneralControls command
};

#endif //TG4_PHYSICS_MESSENGER_H
