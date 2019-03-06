#ifndef TG4_MODEL_CONFIGURATION_MESSENGER_H
#define TG4_MODEL_CONFIGURATION_MESSENGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ModelConfigurationMessenger.h
/// \brief Definition of the TG4ModelConfigurationMessenger class
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4ModelConfigurationManager;

class G4UIdirectory;
class G4UIcmdWithAString;

/// \ingroup physics_list
/// \brief Messenger class that defines commands for the special physica
/// models
///
/// Implements commands:
/// - /mcPhysics/physicsName/setModel modelName
/// - /mcPhysics/physicsName/setParticles particleName1 particleName2 ...
/// - /mcPhysics/physicsName/setRegions regionName1 regionName2 ...
/// - /mcPhysics/physicsName/setEmModel modelName  (deprecated)
/// where physicName = fastSimulation, emModel
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ModelConfigurationMessenger: public G4UImessenger
{
  public:
    TG4ModelConfigurationMessenger(TG4ModelConfigurationManager* manager,
                                   const G4String& availableModels);
    virtual ~TG4ModelConfigurationMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String string);

  private:
    /// Not implemented
    TG4ModelConfigurationMessenger();
    /// Not implemented
    TG4ModelConfigurationMessenger(const TG4ModelConfigurationMessenger& right);
    /// Not implemented
    TG4ModelConfigurationMessenger& operator=(const TG4ModelConfigurationMessenger& right);

    //
    // data members

    /// associated class
    TG4ModelConfigurationManager*  fModelConfigurationManager;

    /// current model name
    G4String               fSelectedModel;

    /// command directory
    G4UIdirectory*         fDirectory;

    /// setModel command
    G4UIcmdWithAString*    fSetModelCmd;

    /// setEmModel command (deprecated)
    G4UIcmdWithAString*    fSetEmModelCmd;

    /// setParticles command
    G4UIcmdWithAString*    fSetParticlesCmd;

    /// setRegions command
    G4UIcmdWithAString*    fSetRegionsCmd;
};

#endif //TG4_MODEL_CONFIGURATIONS_MESSENGER_H
