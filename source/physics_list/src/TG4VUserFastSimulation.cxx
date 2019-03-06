//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VUserFastSimulation.cxx
/// \brief Implementation of the TG4VUserFastSimulation class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VUserFastSimulation.h"
#include "TG4GeometryManager.h"
#include "TG4ModelConfigurationManager.h"
#include "TG4ModelConfiguration.h"
#include "TG4Globals.h"

#include <G4VFastSimulationModel.hh>

//_____________________________________________________________________________
TG4VUserFastSimulation::TG4VUserFastSimulation()
 :// fModelNames(),
   fFastModelsManager(0)
{
/// Default constructor

  fFastModelsManager
    = TG4GeometryManager::Instance()->GetFastModelsManager();
}

//_____________________________________________________________________________
TG4VUserFastSimulation::~TG4VUserFastSimulation()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4VUserFastSimulation::SetModel(const G4String& modelName)
{
  fFastModelsManager->SetModel(modelName);
}

//_____________________________________________________________________________
void TG4VUserFastSimulation::SetModelParticles(const G4String& modelName,
                                               const G4String& particles)
{
  fFastModelsManager->SetModelParticles(modelName, particles);
}

//_____________________________________________________________________________
void TG4VUserFastSimulation::SetModelRegions(const G4String& modelName,
                                             const G4String& regions)
{
  fFastModelsManager->SetModelRegions(modelName, regions);
}

//_____________________________________________________________________________
void TG4VUserFastSimulation::Register(G4VFastSimulationModel* fastSimulationModel)
{
/// Register the user fast simulation model

  // check that model configuration exists
  TG4ModelConfiguration* modelConfiguration
    = fFastModelsManager->GetModelConfiguration(fastSimulationModel->GetName());
  if ( ! modelConfiguration ) {
    TString text = "The fast simulation model ";
    text += fastSimulationModel->GetName().data();
    text += " configuration was not found.";
    TG4Globals::Warning(
      "TG4VUserFastSimulation", "Register",
      text + TG4Globals::Endl()
      + TString("The model configuration has to set firts.") + TG4Globals::Endl()
      + TString("/mcPhysics/fastSimulation/setModel command or SetModel() function."));
  	return;
  }

  modelConfiguration->SetFastSimulationModel(fastSimulationModel);
}
