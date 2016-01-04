//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GflashFastSimulation.cxx
/// \brief Implementation of the TG4GflashFastSimulation class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4GflashFastSimulation.h"
#include "TG4GflashFastSimulationMessenger.h"
#include "TG4Globals.h"

#include <G4RegionStore.hh>
#include <G4Material.hh>
#include <GFlashHomoShowerParameterisation.hh>
#include <GFlashShowerModel.hh>
#include <GFlashHitMaker.hh>
#include <GFlashParticleBounds.hh>

#include <Riostream.h>

using namespace std;

//_____________________________________________________________________________
TG4GflashFastSimulation::TG4GflashFastSimulation() 
  : TG4VUserFastSimulation(),
    fMessenger(0),
    fMaterialName(),
    fGflashShowerModel(0)
{
/// Standard constructor

  // create the model in contsructor 
  // to make available its messenger commands
  fGflashShowerModel = new GFlashShowerModel("GflashShowerModel");
              // region will be set via the model configuration

  fMessenger = new TG4GflashFastSimulationMessenger(this);
}

//_____________________________________________________________________________
TG4GflashFastSimulation::~TG4GflashFastSimulation()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
void  TG4GflashFastSimulation::Construct()
{
/// This function must be overriden in user class and users should create
/// the simulation models and register them to VMC framework

  if ( ! fMaterialName.size() ) {
    TG4Globals::Warning(
      "TG4GflashFastSimulation", "Construct", 
      "The material for Gflash parameterisation is not defined.");
    return;
  }

  // Get material from G4MaterialTable
  G4Material*  material = G4Material::GetMaterial(fMaterialName);
  if ( ! material ) {
    TString text = "The material ";
    text += fMaterialName.data();
    text += " was not found.";
    TG4Globals::Warning(
      "TG4GflashFastSimulation", "Construct", text);
    return;
  }

  // Initializing shower model
  //
  G4cout << "Configuring shower parameterization model" << G4endl;

  fGflashShowerModel->SetFlagParamType(1);

  GFlashHomoShowerParameterisation* parameterisation =
    new GFlashHomoShowerParameterisation(material);
  fGflashShowerModel->SetParameterisation(*parameterisation);

  // Energy cuts to kill particles:
  GFlashParticleBounds* particleBounds = new GFlashParticleBounds();
  fGflashShowerModel->SetParticleBounds(*particleBounds);

  // Makes the EnergieSpots
  GFlashHitMaker* hitMaker = new GFlashHitMaker();
  fGflashShowerModel->SetHitMaker(*hitMaker);

  // Register model in VMC frameworks
  Register(fGflashShowerModel);

  G4cout<<"end configuring shower parameterization."<<G4endl;
  //
  // end Initializing shower model
}
