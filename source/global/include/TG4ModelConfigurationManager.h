#ifndef TG4_MODEL_CONFIGURATION_MANAGER_H
#define TG4_MODEL_CONFIGURATION_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ModelConfigurationManager.h
/// \brief Definition of the TG4ModelConfigurationManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"

#include <globals.hh>

#include <vector>

class TG4ModelConfiguration;
class TG4ModelConfigurationMessenger;

/// \ingroup physics_list
/// \brief The model configuration vector with suitable setters and a messenger.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ModelConfigurationManager : public TG4Verbose
{
 public:
  typedef std::vector<TG4ModelConfiguration*> ModelConfigurationVector;

 public:
  TG4ModelConfigurationManager(
    const G4String& name, const G4String& availableModels = "");
  virtual ~TG4ModelConfigurationManager();

  // methods
  void CreateRegions();

  // set methods
  void SetModel(const G4String& modelName);
  void SetModelParticles(const G4String& modelName, const G4String& particles);
  void SetModelRegions(const G4String& modelName, const G4String& regions);

  // get methods
  G4String GetName() const;
  G4String GetAvailableModels() const;
  TG4ModelConfiguration* GetModelConfiguration(
    const G4String& modelName, G4bool warn = true) const;
  const ModelConfigurationVector& GetVector() const;

 protected:
  // methods
  void SetRegionsNames();

 private:
  /// Not implemented
  TG4ModelConfigurationManager(const TG4ModelConfigurationManager& right);
  /// Not implemented
  TG4ModelConfigurationManager& operator=(
    const TG4ModelConfigurationManager& right);

  // data members

  /// Messenger
  TG4ModelConfigurationMessenger* fMessenger; ///< messenger

  /// The associated physics builder name
  G4String fName;

  /// List of available models
  G4String fAvailableModels;

  /// Vector of registered model configurations
  ModelConfigurationVector fVector;

  /// Info whether regions were constructed
  G4bool fCreateRegionsDone;
};

// inline functions

inline G4String TG4ModelConfigurationManager::GetName() const
{
  /// Return the associated physics builder name
  return fName;
}

inline G4String TG4ModelConfigurationManager::GetAvailableModels() const
{
  /// Return the list of available models
  return fAvailableModels;
}

inline const TG4ModelConfigurationManager::ModelConfigurationVector&
TG4ModelConfigurationManager::GetVector() const
{
  /// Return the vector of registered model configurations
  return fVector;
}

#endif // TG4_MODEL_CONFIGURATION_MANAGER_H
