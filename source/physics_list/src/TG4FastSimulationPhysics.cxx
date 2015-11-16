//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4FastSimulationPhysics.cxx
/// \brief Implementation of the TG4FastSimulationPhysics class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4FastSimulationPhysics.h"
#include "TG4VUserFastSimulation.h"
#include "TG4GeometryManager.h"
#include "TG4ModelConfigurationManager.h"
#include "TG4ModelConfiguration.h"
#include "TG4Globals.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4FastSimulationManagerProcess.hh>
#include <G4RegionStore.hh>
#include <G4Region.hh>
#include <G4AnalysisUtilities.hh>

//_____________________________________________________________________________
G4ThreadLocal
TG4FastSimulationPhysics::ProcessMap* TG4FastSimulationPhysics::fgProcessMap = 0;

//_____________________________________________________________________________
TG4FastSimulationPhysics::TG4FastSimulationPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fUserFastSimulation(0)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4FastSimulationPhysics::TG4FastSimulationPhysics(G4int theVerboseLevel,
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name, theVerboseLevel),
    fUserFastSimulation(0)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4FastSimulationPhysics::~TG4FastSimulationPhysics()
{
/// Destructor

  if ( fgProcessMap ) {
    ProcessMap::iterator it;
    for ( it=fgProcessMap->begin(); it != fgProcessMap->end(); it++) {
      delete it->second;
    }
    delete fgProcessMap;
    fgProcessMap = 0;
  }
}

//
// private methods
//

//_____________________________________________________________________________
void TG4FastSimulationPhysics::UpdateRegions(
                                 const std::vector<TG4ModelConfiguration*>& models)
{

  if ( VerboseLevel() > 1 ) {
    G4cout << "TG4FastSimulationPhysics::UpdateRegions" << G4endl;
  }        

  // Process fast models configurations
  std::vector<TG4ModelConfiguration*>::const_iterator it;
  for ( it = models.begin(); it != models.end(); it++ ) {

    // Get model configuration
    G4String modelName = (*it)->GetModelName();
    G4String regions = (*it)->GetRegions();
    G4VFastSimulationModel* fastSimulationModel = (*it)->GetFastSimulationModel();

    if ( VerboseLevel() > 1 ) {
      G4cout << "Adding fast simulation model " << modelName
             << " object " << fastSimulationModel
             << " to regions " << regions << G4endl;
    }

    if ( ! fastSimulationModel ) {
      TString text = "The fast simulation model ";
      text += modelName.data();
      text += " was not found.";
      TG4Globals::Warning(
        "TG4FastSimulationPhysics", "UpdateRegions", text);
      continue;
    }

    if ( ! regions.size() ) {
      TString text = "The fast simulation model ";
      text += modelName.data();
      text += " has no associated region.";
      TG4Globals::Warning(
        "TG4FastSimulationPhysics", "UpdateRegions", text);
      continue;
    }

    std::vector<G4String> regionVector;
    // use analysis utility to tokenize regions
    G4Analysis::Tokenize(regions, regionVector);

    for (G4int j=0; j<G4int(regionVector.size()); ++j) {

      // Get region
      G4Region* region
        = G4RegionStore::GetInstance()->GetRegion(regionVector[j]);

      if ( ! region ) {
        TString text = "The region ";
        text += regionVector[j].data();
        text += " was not found.";
        TG4Globals::Warning(
          "TG4FastSimulationPhysics", "UpdateRegions", text);
        continue;
      }

      // Retrieve fast simulation manager ou create one if needed.
      G4FastSimulationManager* fastSimulationManager
        = region->GetFastSimulationManager();
      if ( ! fastSimulationManager ) {
        if ( VerboseLevel() > 1 ) {
          G4cout << "creating G4FastSimulationManager for the region" << G4endl;
        }
        // TO DO: CHECK THIS
        G4bool isUnique = false; 
        fastSimulationManager = new G4FastSimulationManager(region, isUnique);
      }
      fastSimulationManager->AddFastSimulationModel(fastSimulationModel);
    }
  }
}

//_____________________________________________________________________________
G4FastSimulationManagerProcess* 
TG4FastSimulationPhysics::GetOrCreateFastSimulationProcess(const G4String& modelName)
{
/// Get thread-local fast simulation process and create one if it does
/// not yet exist.

  // Create fast simulation process map if it does not yet exist
  if ( ! fgProcessMap ) {
    fgProcessMap = new ProcessMap();
  }

  // Create one thread-local fast simulation process per model
  G4FastSimulationManagerProcess*  fastSimulationProcess = 0;
  ProcessMap::iterator it
    = fgProcessMap->find(modelName);
  if ( it != fgProcessMap->end() ) {
    fastSimulationProcess = it->second;
  } else {
    //fastSimulationProcess = new G4FastSimulationManagerProcess(modelName);
    fastSimulationProcess = new G4FastSimulationManagerProcess();
    (*fgProcessMap)[modelName] = fastSimulationProcess;
    G4cout << "... created fastSimulationProcess" << G4endl;
  }

  return fastSimulationProcess;
}

//_____________________________________________________________________________
void TG4FastSimulationPhysics::AddFastSimulationProcess(
                                 const std::vector<TG4ModelConfiguration*>& models)
{
/// Loop over all particles and their processes and check if
/// the process is present in the map

  if ( VerboseLevel() > 1 ) {
    G4cout << "TG4FastSimulationPhysics::AddFastSimulationProcess" << G4endl;
  }  

  // Process fast models configurations    
  std::vector<TG4ModelConfiguration*>::const_iterator it;
  for ( it = models.begin(); it != models.end(); it++ ) {

    // Get model name
    G4String modelName = (*it)->GetModelName();
    G4String particles = (*it)->GetParticles();


    // Get or create fast simulation process
    G4FastSimulationManagerProcess* fastSimulationProcess 
      = GetOrCreateFastSimulationProcess(modelName);
 
    // Add fast simulation process to selected particles 
    aParticleIterator->reset();
    while ((*aParticleIterator)()) {
      G4ParticleDefinition* particle = aParticleIterator->value();
      G4String particleName = particle->GetParticleName();
      
      // skip particles which are not in selection
      if ( particles != "all" &&
           ( ! (*it)->HasParticle(particleName) ) ) {
           continue;
      }

      if ( VerboseLevel() > 2 ) {
        G4cout << "Adding model " << modelName
               << " to particle " <<  particle->GetParticleName() << G4endl;
      }

      // Set the process to the particle process manager     
      particle->GetProcessManager()->AddDiscreteProcess(fastSimulationProcess);
    }
  }
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4FastSimulationPhysics::ConstructParticle()
{
/// Instantiate particles - nothing to be done here
}

//_____________________________________________________________________________
void TG4FastSimulationPhysics::ConstructProcess()
{
  if ( VerboseLevel() > 1 ) {
    G4cout << "TG4FastSimulationPhysics::ConstructProcess " << G4endl;
  }

  // Get model configurations vector from geometry manager
  TG4ModelConfigurationManager* fastModelsManager 
    = TG4GeometryManager::Instance()->GetFastModelsManager();

  const std::vector<TG4ModelConfiguration*>& models 
    = fastModelsManager->GetVector();


  // Do nothing if no models were set  
  if ( models.size() == 0 ) {
    if ( VerboseLevel() > 1 ) {
      G4cout << "No fast simulation models are defined." << G4endl;
    }
    return;
  }

  // Construct user models
  if ( fUserFastSimulation ) {
    fUserFastSimulation->Construct();
  }

  // Update regions
  UpdateRegions(models);

  // Add fast simulation process to particles
  AddFastSimulationProcess(models);

  if ( VerboseLevel() > 0 ) {
    G4cout << "### Fast simulation models added to physics processes" << G4endl;
  }
}
