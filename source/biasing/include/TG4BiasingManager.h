#ifndef TG4_BIASING_MANAGER_H
#define TG4_BIASING_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2019 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4BiasingManager.h
/// \brief Definition of the TG4BiasingManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4ModelConfigurationManager.h"

/// \ingroup physics_list
/// \brief The biasing manager.
///
/// This class uses the TG4ModelConfigurationManager functionality for
/// creating the UI commands for configuring the selection of the media
/// and particles to which biasing will be applied.
/// The manager does not contribute to creating regions, as the biasing
/// operator is attached directly to logical volumes.

/// \author I. Hrivnacova; IPN Orsay

class TG4BiasingManager : public TG4ModelConfigurationManager
{
 public:
  typedef std::vector<TG4ModelConfiguration*> ModelConfigurationVector;

 public:
  TG4BiasingManager(
    const G4String& name, const G4String& availableModels = "");
  virtual ~TG4BiasingManager();

  // methods
  void CreateBiasingOperator();

 private:
  /// Not implemented
  TG4BiasingManager(const TG4BiasingManager& right);
  /// Not implemented
  TG4BiasingManager& operator=(
    const TG4BiasingManager& right);
};


#endif // TG4_BIASING_MANAGER_H
