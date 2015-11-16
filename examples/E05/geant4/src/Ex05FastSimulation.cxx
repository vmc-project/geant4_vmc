//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05FastSimulation.cxx
/// \brief Implementation of the Ex05FastSimulation class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex05FastSimulation.h"

#include <G4RegionStore.hh>
#include <G4Material.hh>
#include <GFlashHomoShowerParameterisation.hh>
#include <G4FastSimulationManager.hh>
#include <GFlashShowerModel.hh>
#include <GFlashHitMaker.hh>
#include <GFlashParticleBounds.hh>
#include <G4NistManager.hh>

#include <Riostream.h>

//_____________________________________________________________________________
Ex05FastSimulation::Ex05FastSimulation() 
  : TG4VUserFastSimulation()
{
/// Standard constructor
/*
  // Create fast simulation model configuration.
  // This will generate UI commands which can be used to set particles
  // and regions where the model will be applied
  SetModel("fastShowerModel");

  // In the following calls users can select the particles and regions
  // which the fast simulation model(s) will be applied to. 
  // The setting can be done also interactively via UI commands.
  SetModelParticles("fastShowerModel", "all");
  SetModelRegions("fastShowerModel", "AirB");
*/
}

//_____________________________________________________________________________
Ex05FastSimulation::~Ex05FastSimulation()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
void  Ex05FastSimulation::Construct()
{
/// This function must be overriden in user class and users should create
/// the simulation models and register them to VMC framework

  // Get PbWO4 material from G4MaterialTable
  G4Material*  material = G4Material::GetMaterial("PbWO4");
  if ( ! material ) {
    cerr << "Material PbWO4 is not defined." << endl;
    return;
  }

  // Initializing shower model
  //
  G4cout << "Creating shower parameterization models" << G4endl;

  // GFlashShowerModel* gflashShowerModel 
  //   = new GFlashShowerModel("fastShowerModel", region);
            // region is set via the model configuration
  GFlashShowerModel* gflashShowerModel 
    = new GFlashShowerModel("fastShowerModel");

  gflashShowerModel->SetFlagParamType(1);

  GFlashHomoShowerParameterisation* parameterisation =
    new GFlashHomoShowerParameterisation(material);
  gflashShowerModel->SetParameterisation(*parameterisation);

  // Energy cuts to kill particles:
  GFlashParticleBounds* particleBounds = new GFlashParticleBounds();
  gflashShowerModel->SetParticleBounds(*particleBounds);

  // Makes the EnergieSpots
  GFlashHitMaker* hitMaker = new GFlashHitMaker();
  gflashShowerModel->SetHitMaker(*hitMaker);

  // Register model in VMC frameworks
  Register(gflashShowerModel);

  G4cout<<"end shower parameterization."<<G4endl;
  //
  // end Initializing shower model
}   
