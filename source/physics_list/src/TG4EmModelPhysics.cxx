//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4EmModelPhysics.cxx
/// \brief Implementation of the TG4EmModelPhysics class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4EmModelPhysics.h"
#include "TG4EmModelConfiguration.h"
#include "TG4SpecialUrbanMscModel.h"
#include "TG4GeometryServices.h"
#include "TG4PhysicsManager.h"
#include "TG4MediumMap.h"
#include "TG4Medium.h"
#include "TG4Limits.h"
#include "TG4G3ControlVector.h"
#include "TG4Globals.h"

#include <TVirtualMCDecayer.h>
#include <TVirtualMC.h>

#include <G4TransportationManager.hh>
#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4PAIModel.hh>
#include <G4PAIPhotModel.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4RegionStore.hh>
#include <G4AnalysisUtilities.hh>

//
// static methods
//

//_____________________________________________________________________________
TG4EmModel TG4EmModelPhysics::GetEmModel(const G4String& modelName)
{
/// Return the model tope for given model name

  if ( modelName == GetEmModelName(kPAIModel) ) {       
    return kPAIModel;
  }  
  else if ( modelName == GetEmModelName(kPAIPhotonModel) ) {
    return kPAIPhotonModel;
  }  
  else if ( modelName == GetEmModelName(kSpecialUrbanMscModel) ) {
    return kSpecialUrbanMscModel;
  }
  else if ( modelName == GetEmModelName(kNoEmModel) ) {      
    return kNoEmModel; 
  }  
  else {
    TG4Globals::Exception(
      "TG4EmModelPhysics", "GetEmModel", 
      TString(modelName.data()) + " unknown model name.");
     return kNoEmModel;
  }
}       

//_____________________________________________________________________________
G4String  TG4EmModelPhysics::GetEmModelName(G4int modelType)
{
/// Return model name for given model type

  switch ( modelType ) {
    case kPAIModel:             return "PAI";
    case kPAIPhotonModel:       return "PAIPhoton";
    case kSpecialUrbanMscModel: return "SpecialUrbanMsc";
    case kNoEmModel:            return "";
    default:
      TG4Globals::Exception(
        "TG4EmModelPhysics", "GetEmModelName", 
        TString("Unknown model type ") + TString(modelType));
       return kNoEmModel;
  }
}    
    
//
// ctors, dtor
//

//_____________________________________________________________________________
TG4EmModelPhysics::TG4EmModelPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fMessenger(this),
    fEmConfigurator(),
    fEmModels()
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4EmModelPhysics::TG4EmModelPhysics(G4int theVerboseLevel,
                                     const G4String& name)
  : TG4VPhysicsConstructor(name, theVerboseLevel),
    fMessenger(this),
    fEmConfigurator(),
    fEmModels()
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4EmModelPhysics::~TG4EmModelPhysics() 
{
/// Destructor

  EmModelConfigurationVector::iterator it;
  for ( it = fEmModels.begin(); it != fEmModels.end(); it++ ) {
    delete *it;
  }
}

//
// private methods
//

//_____________________________________________________________________________
TG4EmModelConfiguration* TG4EmModelPhysics::GetEmModelConfiguration(
                            const G4String& modelName,
                            const G4String& functionName) const
{
/// Return model configuration via specified model name;
/// print warning if functionName is specified and configuration is not found

  EmModelConfigurationVector::const_iterator it;
  for ( it = fEmModels.begin(); it != fEmModels.end(); it++ ) {
    if ( (*it)->GetModelName() == modelName ) return *it;
  }

  if ( functionName.size() ) {
    TString text = "EM model ";
    text +=  modelName.data();
    text += " does not exist";
    TG4Globals::Warning(
      "TG4EmModelPhysics", functionName, text);
  }

  return 0;
}

//_____________________________________________________________________________
void TG4EmModelPhysics::CreateRegions()
{
/// Create regions for all media with special EM models
  
  if ( VerboseLevel() > 1 ) {
    G4cout << "TG4EmModelPhysics::CreateRegions" << G4endl;
  }

  // Get world default region 
  G4LogicalVolume* worldLV 
    = G4TransportationManager::GetTransportationManager()
        ->GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume();
        // We cannot access world volume via geometry services as 
        // the it is available here only after initialization

  // Get default range cut values from physics manager
  G4double rangeCutGam 
    = TG4PhysicsManager::Instance()->GetCutForGamma();
  G4double rangeCutEle 
    = TG4PhysicsManager::Instance()->GetCutForElectron();
  G4double rangeCutPos 
    = TG4PhysicsManager::Instance()->GetCutForPositron();

  // Loop over logical volumes
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    G4bool isWorld = ( lv == worldLV ) ;
    
    if ( isWorld ) continue;

    TG4Medium* medium
      = TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(lv, false);

    if ( ! medium ) continue;

    G4String mediumName = medium->GetName();
    G4String materialName = lv->GetMaterial()->GetName();

    //G4cout << "Processing volume " << lv->GetName()
    //      << ", medium " << mediumName
    //      << ", material " << materialName << G4endl;

    // Skip volumes with media which are not in the map  
    G4bool isEmModelRegion = false;
    EmModelConfigurationVector::const_iterator it;
    for ( it = fEmModels.begin(); it != fEmModels.end(); it++ ) {
      G4String regions = (*it)->GetRegions();
      if ( regions.find(mediumName) != std::string::npos ) isEmModelRegion = true;
    }

    if ( ! isEmModelRegion ) continue;

    // If region already exists, only add the logical volume
    // and continue the loop   
    G4Region* region 
      = G4RegionStore::GetInstance()->GetRegion(materialName, false );
    //G4cout << "Got region " << regionName << " " << region << G4endl;
    
    if ( ! region ) {
      region = new G4Region(materialName);
      G4ProductionCuts* dcuts = new G4ProductionCuts();
      dcuts->SetProductionCut(rangeCutGam, 0);
      dcuts->SetProductionCut(rangeCutEle, 1);
      dcuts->SetProductionCut(rangeCutPos, 2);
      region->SetProductionCuts(dcuts);
      //G4cout << "Created region " << regionName << G4endl;
    }   
    
    region->AddRootLogicalVolume(lv);
  }  
}    

//_____________________________________________________________________________
void TG4EmModelPhysics::AddModel(
                           TG4EmModel emModel,
                           const G4ParticleDefinition* particle, 
                           const G4String& regions)
{ 
/// Add selected EM model to given particle, process and region.
/// If regionName is not set, the model is set to the world region.

  // Get process name
  G4ProcessVector* processVector
    = particle->GetProcessManager()->GetProcessList();
  for (G4int i=0; i<processVector->length(); i++) {
    G4String processName;
    G4String currentProcessName = (*processVector)[i]->GetProcessName();

    // PAI applied to ionisation
    if ( currentProcessName.contains("Ioni") && ( emModel == kPAIModel || emModel == kPAIPhotonModel ) ) {
      processName = currentProcessName;
    }

    // UrbanMsc applied to msc
    if ( currentProcessName.contains("msc") && ( emModel == kSpecialUrbanMscModel ) ) {
      processName = currentProcessName;
    }

    if ( ! processName.size() ) continue;

    // CreateEM model
    //
    G4VEmModel* g4EmModel = 0;
    G4VEmFluctuationModel* g4FluctModel = 0;
    if ( emModel == kPAIModel ) {
      // PAI
      G4PAIModel* pai = new G4PAIModel(particle, "PAIModel");
      if ( verboseLevel > 1 ) {
        G4cout << "New G4PAIModel" << G4endl;
      }
      g4EmModel = pai;
      g4FluctModel = pai;
    }
    else if ( emModel == kPAIPhotonModel ) {
      // PAIPhoton
      if ( verboseLevel > 1 ) {
        G4cout << "New G4PAIPhotModel" << G4endl;
      }
      G4PAIPhotModel* paiPhot = new G4PAIPhotModel(particle, "PAIPhotModel");
      g4EmModel = paiPhot;
      g4FluctModel = paiPhot;
    }
    else if ( emModel == kSpecialUrbanMscModel ) {
      // SpecialUrbanMsc
      if ( verboseLevel > 1 ) {
        G4cout << "New TG4SpecialUrbanMscModel" << G4endl;
      }
      g4EmModel = new TG4SpecialUrbanMscModel();
      g4FluctModel = 0;
    }

    // Get regions
    std::vector<G4String> regionVector;
    if ( regions.size() ) {
      // use analysis utility to tokenize regions
      G4Analysis::Tokenize(regions, regionVector);
    }
    else {
      // Get world default region
      G4LogicalVolume* worldLV
        = TG4GeometryServices::Instance()->GetWorld()->GetLogicalVolume();
      regionVector.push_back(worldLV->GetRegion()->GetName());
    }

    for (G4int j=0; j<G4int(regionVector.size()); ++j) {

      // Get material name (=region name)
      TG4Medium* medium
        = TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(regionVector[j]);
      G4String regionName
        = medium->GetMaterial()->GetName();

      if ( VerboseLevel() > 1 ) {
        G4cout << "Adding EM model: " << GetEmModelName(emModel)
               << " to particle: " << particle->GetParticleName()
               << " process: " << processName
               << " medium: " << regionVector[j]
               << " region: " << regionName
               << G4endl;
      }

      fEmConfigurator.SetExtraEmModel(
                        particle->GetParticleName(), processName, g4EmModel,
                        regionName, 0.0, DBL_MAX, g4FluctModel);
    }
  }
}                               

//_____________________________________________________________________________
void TG4EmModelPhysics::AddModels()
{
/// Loop over all particles and their processes and check if
/// the process is present in the map

  if ( VerboseLevel() > 1 ) {
    G4cout << "TG4EmModelPhysics::AddModels" << G4endl;
  }  

  EmModelConfigurationVector::const_iterator it;
  for ( it = fEmModels.begin(); it != fEmModels.end(); it++ ) {

    // Get model configuration
    TG4EmModel emModel = GetEmModel((*it)->GetModelName());
    G4String particles = (*it)->GetParticles();
    G4String regions = (*it)->GetRegions();
    
    // Add selected models
    aParticleIterator->reset();
    while ((*aParticleIterator)()) {
      G4ParticleDefinition* particle = aParticleIterator->value();
      G4String particleName = particle->GetParticleName();
      
      // skip particles which are not in selection
      if ( particles != "all" &&
           particles.find(particle->GetParticleName()) == std::string::npos ) {
           continue;
      }     

      // skip also monopole (exoerimental)
      if ( particle->GetParticleName() == "monopole" ) {
        G4cout << "TG4EmModelPhysics::AddModels - skipping monopole" << G4endl;
      }
                 
      AddModel(emModel, particle, regions);
    }
  }
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4EmModelPhysics::ConstructParticle()
{
/// Instantiate particles - nothing to be done here
}

//_____________________________________________________________________________
void TG4EmModelPhysics::ConstructProcess()
{
/// Loop over all particles and their processes and check if
/// the process is present in the map

  // Do nothing if no models were set  
  if ( fEmModels.size() == 0 ) return;

  // Create regions
  CreateRegions();

  // Add user selected models to G4 EM configurator
  AddModels();
  
  // Let G4 EM configurator to add all previously declared models to corresponding 
  // processes
  fEmConfigurator.AddModels();
    
  if ( VerboseLevel() > 0 ) {
    G4cout << "### Selected EmModels added to EM processes" << G4endl;
  }    
}

//_____________________________________________________________________________
void TG4EmModelPhysics::SetEmModel(const G4String& modelName)
{
/// Set an extra EM model with the given name.

  if ( ! GetEmModelConfiguration(modelName) ) {
    fEmModels.push_back(new TG4EmModelConfiguration(modelName));
  }
  else {
    TString text = "Cannot create EM model ";
    text +=  modelName.data();
    text += " twice.";
    TG4Globals::Warning(
      "TG4EmModelPhysics", "CreateEmModel", text);
  }
}

//_____________________________________________________________________________
void TG4EmModelPhysics::SetEmModelParticles(const G4String& modelName,
                                            const G4String& particles)
{
/// Set particles for the extra EM model for given medium.

  TG4EmModelConfiguration* emModelConfiguration
    = GetEmModelConfiguration(modelName, "SetEmModelParticles");

  if ( ! emModelConfiguration ) return;

  emModelConfiguration->SetParticles(particles);
}

//_____________________________________________________________________________
void TG4EmModelPhysics::SetEmModelRegions(const G4String& modelName,
                                          const G4String& regions)
{
/// Set regions for the extra EM model for given medium.

  TG4EmModelConfiguration* emModelConfiguration
    = GetEmModelConfiguration(modelName, "SetEmModelRegions");

  if ( ! emModelConfiguration ) return;

  emModelConfiguration->SetRegions(regions);
}


