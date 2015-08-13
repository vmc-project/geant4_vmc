#ifndef TG4_EM_MODEL_PHYSICS_MESSENGER_H
#define TG4_EM_MODEL_PHYSICS_MESSENGER_H 

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4EmModelPhysicsMessenger.h
/// \brief Definition of the TG4EmModelPhysicsMessenger class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4EmModelPhysics;

class G4UIdirectory;
class G4UIcmdWithAString;

/// \ingroup physics_list
/// \brief Messenger class that defines commands for the optical physics
///
/// Implements commands:
/// - /mcPhysics/emModel/setEmModel modelName
/// - /mcPhysics/emModel/setParticles particleName1 particleName2 ...
/// - /mcPhysics/emModel/setRegions regionName1 regionName2 ...
///
/// \author I. Hrivnacova; IPN Orsay

class TG4EmModelPhysicsMessenger: public G4UImessenger
{
  public:
    TG4EmModelPhysicsMessenger(TG4EmModelPhysics* EmModelPhysics); 
    virtual ~TG4EmModelPhysicsMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    /// Not implemented
    TG4EmModelPhysicsMessenger();  
    /// Not implemented
    TG4EmModelPhysicsMessenger(const TG4EmModelPhysicsMessenger& right);
    /// Not implemented
    TG4EmModelPhysicsMessenger& operator=(const TG4EmModelPhysicsMessenger& right);

    //
    // data members
    
    /// associated class
    TG4EmModelPhysics*     fEmModelPhysics;
    
    /// current model name
    G4String               fSelectedEmModel;

    /// command directory
    G4UIdirectory*         fDirectory; 

    /// setElossModel command
    G4UIcmdWithAString*    fSetEmModelCmd;

    /// setParticles command
    G4UIcmdWithAString*    fSetParticlesCmd;

    /// setRegions command
    G4UIcmdWithAString*    fSetRegionsCmd;
};    

#endif //TG4_OPTICAL_PHYSICS_MESSENGER_H
