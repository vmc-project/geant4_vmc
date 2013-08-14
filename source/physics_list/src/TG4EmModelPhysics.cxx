// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4EmModelPhysics.cxx
/// \brief Implementation of the TG4EmModelPhysics class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4EmModelPhysics.h"
#include "TG4EmModelConfiguration.h"
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
#include <G4PAIPhotonModel.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4RegionStore.hh>
#include <G4Version.hh>

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
    case kPAIModel:        return "PAI";
    case kPAIPhotonModel:  return "PAIPhoton";
    case kNoEmModel:       return "";
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
    fModelMap()
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4EmModelPhysics::TG4EmModelPhysics(G4int verboseLevel,
                                     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fMessenger(this),
    fEmConfigurator(),
    fModelMap()
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4EmModelPhysics::~TG4EmModelPhysics() 
{
/// Destructor
}

//
// private methods
//

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
    
    if ( isWorld) continue;
    
    G4int mediumId 
      = TG4GeometryServices::Instance()->GetMediumId(lv);
    
    // Skip volumes with media which are not in the map  
    ModelToMediumIdMap::const_iterator it = fModelMap.find(mediumId);
    if ( it == fModelMap.end() ) continue;  
    
    // If region already exists, only add the logical volume
    // and continue the loop   
    G4Material* material = lv->GetMaterial();
    G4String regionName = material->GetName();
    G4Region* region 
      = G4RegionStore::GetInstance()->GetRegion(regionName, false );
    
    if ( ! region ) {
      region = new G4Region(regionName);
      G4ProductionCuts* dcuts = new G4ProductionCuts();
      dcuts->SetProductionCut(rangeCutGam, 0);
      dcuts->SetProductionCut(rangeCutEle, 1);
      dcuts->SetProductionCut(rangeCutPos, 2);
      region->SetProductionCuts(dcuts);
    }   
    
    region->AddRootLogicalVolume(lv);
  }  
}    

//_____________________________________________________________________________
void TG4EmModelPhysics::AddModel(
                           TG4EmModel elossModel,
                           TG4EmModel fluctModel,
                           const G4ParticleDefinition* particle, 
                           const G4String& processName,
                           const G4String& regionName)
{ 
/// Add selected EM model to given particle, process and region.
/// If regionName is not set, the model is set to the world region.

  G4String modelRegionName = regionName;
  if ( regionName == "" ) {
    // Get world default region 
    G4LogicalVolume* worldLV 
      = TG4GeometryServices::Instance()->GetWorld()->GetLogicalVolume();
    modelRegionName = worldLV->GetRegion()->GetName();
  }  

  G4VEmModel* g4ElossModel = 0;
  G4VEmFluctuationModel* g4FluctModel = 0;
  
  if ( elossModel == kPAIModel ||  fluctModel == kPAIModel ) {
    // PAI
    G4PAIModel* pai = new G4PAIModel(particle, "PAIModel");
    if ( elossModel == kPAIModel ) g4ElossModel = pai;
    if ( fluctModel == kPAIModel ) g4FluctModel = pai;
  }
  else if ( elossModel == kPAIPhotonModel || fluctModel == kPAIPhotonModel ) { 
    // PAIPhoton  
    G4PAIPhotonModel* pai = new G4PAIPhotonModel(particle, "PAIPhotonModel");
    if ( elossModel == kPAIPhotonModel ) g4ElossModel = pai;
    if ( fluctModel == kPAIPhotonModel ) g4FluctModel = pai;
  }
  
  fEmConfigurator.SetExtraEmModel(
                       particle->GetParticleName(), processName, g4ElossModel,
                       modelRegionName, 0.0, DBL_MAX, g4FluctModel);
}                               

//_____________________________________________________________________________
void TG4EmModelPhysics::AddModels()
{
/// Loop over all particles and their processes and check if
/// the process is present in the map

  if ( VerboseLevel() > 1 ) {
    G4cout << "TG4EmModelPhysics::AddModels" << G4endl;
  }  

  TG4MediumMap* mediumMap
    = TG4GeometryServices::Instance()->GetMediumMap();

  ModelToMediumIdMap::const_iterator it;
  for ( it = fModelMap.begin(); it !=  fModelMap.end(); it++ ) {

    // Get region name (which is equal to material name)
    TG4Medium* medium = mediumMap->GetMedium(it->first);
    G4String regionName = medium->GetMaterial()->GetName(); 
    
    // Get EM eloss model
    TG4EmModel elossModel = GetEmModel(it->second->GetElossModelName());
    TG4EmModel fluctModel = GetEmModel(it->second->GetFluctModelName());
    G4String particles = it->second->GetParticles();
    
    // Add selected models
#if G4VERSION_NUMBER < 1000
    theParticleIterator->reset();
    while ((*theParticleIterator)()) {
      G4ParticleDefinition* particle = theParticleIterator->value();
#else
    aParticleIterator->reset();
    while ((*aParticleIterator)()) {
      G4ParticleDefinition* particle = aParticleIterator->value();
#endif      
      G4String particleName = particle->GetParticleName();
      
      // skip particles which are not in selection
      if ( particles != "all" &&
           particles.find(particle->GetParticleName()) == std::string::npos ) {
           continue;
      }     
           
      G4ProcessVector* processVector 
        = particle->GetProcessManager()->GetProcessList();
      for (G4int i=0; i<processVector->length(); i++) {
        G4String processName = (*processVector)[i]->GetProcessName();
        
        if ( processName.contains("Ioni") ) {
          
          if ( VerboseLevel() > 1 ) {
            G4cout << "Adding eloss model: " << GetEmModelName(elossModel) 
                   << "  fluctuations model: " << GetEmModelName(fluctModel)
                   << " to particle: " << particle->GetParticleName()
                   << " process: " << processName
                   << " region: " << regionName << G4endl;
          }         
                 
          AddModel(elossModel, fluctModel, particle, processName, regionName);
        }
      }    
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
  if ( fModelMap.size() == 0 ) return; 

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
void TG4EmModelPhysics::SetEmModel(G4int mediumId, 
                                   const G4String& elossModelName,
                                   const G4String& fluctModelName,
                                   const G4String& particles)
{
/// Define extra EM model for given medium.

  ModelToMediumIdMap::const_iterator it = fModelMap.find(mediumId);
  if ( it != fModelMap.end() ) {
    it->second->SetElossModelName(elossModelName);
    it->second->SetFluctModelName(fluctModelName);
    it->second->SetParticles(particles);
  }  
  else {
    fModelMap[mediumId] 
      = new TG4EmModelConfiguration(elossModelName, fluctModelName, particles);
  }    
}

//_____________________________________________________________________________
void TG4EmModelPhysics::SetEmElossModel(G4int mediumId, 
                                        const G4String& elossModelName)
{
/// Set the extra Eloss EM model for given medium.

  ModelToMediumIdMap::const_iterator it = fModelMap.find(mediumId);
  if ( it != fModelMap.end() ) {
    it->second->SetElossModelName(elossModelName);
  }  
  else {
    fModelMap[mediumId] 
      = new TG4EmModelConfiguration(elossModelName, "", "");
  }
}      
                                       
                                        
//_____________________________________________________________________________
void TG4EmModelPhysics::SetEmFluctModel(G4int mediumId, 
                                        const G4String& fluctModelName)
{
/// Set the extra fluctuation EM model for given medium.

  ModelToMediumIdMap::const_iterator it = fModelMap.find(mediumId);
  if ( it != fModelMap.end() ) {
    it->second->SetFluctModelName(fluctModelName);
  }  
  else {
    fModelMap[mediumId] 
      = new TG4EmModelConfiguration("", fluctModelName, "");
  }
}      

                                        
//_____________________________________________________________________________
void TG4EmModelPhysics::SetEmModelParticles(G4int mediumId, 
                                            const G4String& particles)
{
/// Set particles for the extra EM model for given medium.

  ModelToMediumIdMap::const_iterator it = fModelMap.find(mediumId);
  if ( it != fModelMap.end() ) {
    it->second->SetParticles(particles);
  }  
  else {
    fModelMap[mediumId] 
      = new TG4EmModelConfiguration("", "", particles);
  }
}      

                                            

