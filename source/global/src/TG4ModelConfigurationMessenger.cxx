//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ModelConfigurationMessenger.cxx
/// \brief Implementation of the TG4ModelConfigurationMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ModelConfigurationMessenger.h"
#include "TG4ModelConfigurationManager.h"

#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>

#include <locale>

//
// utility methods
//

namespace
{

//______________________________________________________________________________
G4String GetDirectoryName(const G4String& baseName)
{
  std::string name(baseName);
  // convert first letter to lower case
  std::locale loc;
  name.replace(0, 1, 1, std::tolower(name[0], loc));
  return "/mcPhysics/" + name + "/";
}

} // namespace

//
// ctors, dtor
//

//______________________________________________________________________________
TG4ModelConfigurationMessenger::TG4ModelConfigurationMessenger(
  TG4ModelConfigurationManager* manager, const G4String& availableModels)
  : G4UImessenger(),
    fModelConfigurationManager(manager),
    fSelectedModel(),
    fDirectory(0),
    fSetModelCmd(0),
    fSetParticlesCmd(0),
    fSetExcludedParticlesCmd(0),
    fSetRegionsCmd(0)
{
  /// Standard constructor

  G4String physicsName = fModelConfigurationManager->GetName();

  // Create directory with a name according to modelConfiguration name
  G4String dirName = GetDirectoryName(physicsName);
  fDirectory = new G4UIdirectory(dirName);
  G4String guidance = physicsName + " physics commands.";
  fDirectory->SetGuidance(guidance);

  // setModel command
  G4String commandName = dirName + "setModel";
  fSetModelCmd = new G4UIcmdWithAString(commandName, this);
  guidance = "Define an extra " + physicsName;
  fSetModelCmd->SetGuidance(guidance);
  fSetModelCmd->SetParameterName("Model", false);
  if (availableModels.size()) fSetModelCmd->SetCandidates(availableModels);
  fSetModelCmd->AvailableForStates(G4State_PreInit);

  // setEmModel command = the same as setModel
  // deprecated - kept for backward compatibility
  commandName = dirName + "setEmModel";
  fSetEmModelCmd = new G4UIcmdWithAString(commandName, this);
  guidance = "The same as setModel, kept for backward compatibility.";
  fSetEmModelCmd->SetGuidance(guidance);
  fSetEmModelCmd->SetParameterName("EmModel", false);
  if (availableModels.size()) fSetEmModelCmd->SetCandidates(availableModels);
  fSetEmModelCmd->AvailableForStates(G4State_PreInit);

  // setParticles command
  commandName = dirName + "setParticles";
  fSetParticlesCmd = new G4UIcmdWithAString(commandName, this);
  guidance = "Set particles for the selected extra " + physicsName + "\n" +
             "(all = select all particles which the model is applicable.)";
  fSetParticlesCmd->SetGuidance(guidance.c_str());
  fSetParticlesCmd->SetParameterName("Particles", false);
  fSetParticlesCmd->AvailableForStates(G4State_PreInit);

  // setExcludedParticles command
  commandName = dirName + "setExcludedParticles";
  fSetExcludedParticlesCmd = new G4UIcmdWithAString(commandName, this);
  guidance = "Set to be excluded from the selected extra " + physicsName + "\n" +
             "if 'all' was selected previously ";
  fSetExcludedParticlesCmd->SetGuidance(guidance.c_str());
  fSetExcludedParticlesCmd->SetParameterName("Particles", false);
  fSetExcludedParticlesCmd->AvailableForStates(G4State_PreInit);

  // setRegions command
  commandName = dirName + "setRegions";
  fSetRegionsCmd = new G4UIcmdWithAString(commandName, this);
  guidance = "Set tracking media names (regions) for the selected extra " +
             physicsName + "\n" +
             "("
             " = the model will be applied to the default world region.";
  fSetRegionsCmd->SetGuidance(guidance);
  fSetRegionsCmd->SetParameterName("Regions", false);
  fSetRegionsCmd->AvailableForStates(G4State_PreInit);

  // setOneRegion command
  commandName = dirName + "setOneRegion";
  fSetOneRegionCmd = new G4UIcmdWithAString(commandName, this);
  guidance = "Set one tracking medium name (regions) for the selected extra " +
             physicsName + "\n" +
             "("
             " = the model will be applied to the default world region.";
  fSetOneRegionCmd->SetGuidance(guidance);
  fSetOneRegionCmd->SetParameterName("Region", false);
  fSetOneRegionCmd->AvailableForStates(G4State_PreInit);
}

//______________________________________________________________________________
TG4ModelConfigurationMessenger::~TG4ModelConfigurationMessenger()
{
  /// Destructor
  delete fDirectory;
  delete fSetModelCmd;
  delete fSetEmModelCmd;
  delete fSetParticlesCmd;
  delete fSetExcludedParticlesCmd;
  delete fSetRegionsCmd;
  delete fSetOneRegionCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4ModelConfigurationMessenger::SetNewValue(
  G4UIcommand* command, G4String newValue)
{
  /// Apply command to the associated object.

  if (command == fSetModelCmd || command == fSetEmModelCmd) {
    fSelectedModel = newValue;
    fModelConfigurationManager->SetModel(fSelectedModel);
  }
  else if (command == fSetParticlesCmd) {
    fModelConfigurationManager->SetModelParticles(fSelectedModel, newValue);
  }
  else if (command == fSetExcludedParticlesCmd) {
    fModelConfigurationManager->SetModelExcludedParticles(fSelectedModel, newValue);
  }
  else if (command == fSetRegionsCmd) {
    fModelConfigurationManager->SetModelRegions(fSelectedModel, newValue);
  }
  else if (command == fSetOneRegionCmd) {
    fModelConfigurationManager->SetOneModelRegion(fSelectedModel, newValue);
  }
}
