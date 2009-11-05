// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4RegionsManager.cxx
/// \brief Implementation of the TG4RegionsManager class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RegionsManager.h"
#include "TG4RegionsMessenger.h"
#include "TG4G3PhysicsManager.h"
#include "TG4GeometryServices.h"
#include "TG4G3CutVector.h"
#include "TG4Limits.h"
#include "TG4Globals.h"

#include <G4RunManager.hh>
#include <G4VUserPhysicsList.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Region.hh>
#include <G4RegionStore.hh>
#include <G4ProductionCuts.hh>
#include <G4RToEConvForElectron.hh>
#include <G4RToEConvForGamma.hh>
#include <G4UnitsTable.hh>

#include <map>
#include <set>

TG4RegionsManager* TG4RegionsManager::fgInstance = 0;

const G4int    TG4RegionsManager::fgkDefaultRangePrecision = 2;
const G4double TG4RegionsManager::fgkRangeTolerance = 1e-3*mm;
const G4double TG4RegionsManager::fgkEnergyTolerance = 1e-03;
const G4int    TG4RegionsManager::fgkNofBins = 10;
const G4int    TG4RegionsManager::fgkMinRangeOrder = -3;
const G4int    TG4RegionsManager::fgkMaxRangeOrder = 3;
const G4String TG4RegionsManager::fgkDefaultRegionName = "DefaultRegionForTheWorld";

//_____________________________________________________________________________
TG4RegionsManager::TG4RegionsManager()
  : TG4Verbose("regionsManager"),
    fMessenger(this),
    fRangePrecision(fgkDefaultRangePrecision),
    fIsCheck(false)
    
{ 
/// Default constructor

  fgInstance = this;
}

//_____________________________________________________________________________
TG4RegionsManager::~TG4RegionsManager() 
{
/// Destructor

  fgInstance = 0;
}

//
// private methods
//

//_____________________________________________________________________________
G4double TG4RegionsManager::GetDefaultRangeCut() const
{
/// Return the default range cut value defined in the registered
/// physics list

  G4RunManager* runManager = G4RunManager::GetRunManager();
  
  const G4VUserPhysicsList* kPhysicsList = runManager->GetUserPhysicsList();
  if ( ! kPhysicsList ) {
    TG4Globals::Exception(
      "TG4RegionsManager", "GetDefaultRangeCut",
      "No physics list is defined.");
  }    
  
  return kPhysicsList->GetDefaultCutValue();
}  

//_____________________________________________________________________________
G4Region* TG4RegionsManager::GetDefaultRegion() const
{
/// Return the default region associated with world

  G4LogicalVolume* worldLV 
    = TG4GeometryServices::Instance()->GetWorld()->GetLogicalVolume();
  
  return worldLV->GetRegion();
}  

//_____________________________________________________________________________
G4double  TG4RegionsManager::GetGlobalEnergyCut(TG4G3Cut cutType) const
{
/// Return global cut in energy of given cutType.
/// Return DBL_MAX if cut value is not defined

  TG4boolVector* isCutVector
    = TG4G3PhysicsManager::Instance()->GetIsCutVector();
  TG4G3CutVector* cutVector
    = TG4G3PhysicsManager::Instance()->GetCutVector();

  G4double cutValue = DBL_MAX;
  if ( (*isCutVector)[cutType] && 
       (*cutVector)[cutType] > DBL_MIN) {  
    cutValue = (*cutVector)[cutType];
  }  

  return cutValue;
}    

//_____________________________________________________________________________
G4double  TG4RegionsManager::GetEnergyCut(TG4Limits* limits, 
                                          TG4G3Cut cutType,
                                          G4double globalCutValue) const
{
/// Return cut in energy defined in limits of given cutType.
/// Return DBL_MAX if cut value is not defined.

  G4double cut = DBL_MAX;
  if (  limits->GetCutVector() &&
      (*limits->GetCutVector())[cutType] > DBL_MIN ) { 
    cut = (*limits->GetCutVector())[cutType];
  }  
  else {
    cut = globalCutValue; 
  }  
  
  return cut;
}    
   
//_____________________________________________________________________________
G4bool TG4RegionsManager::Iterate(
                                G4double  energyCut,
                                G4double& lowerCut, 
                                G4double& higherCut,
                                G4double lowEdgeEnergy, 
                                G4double highEdgeEnergy,
                                G4int nbin,
                                std::map<G4double, G4double>& energyToRangeMap,
                                std::map<G4double, G4double>::const_iterator& it,
                                G4Material* material,
                                G4VRangeToEnergyConverter& converter) const
{                                   
  G4double step = ( higherCut - lowerCut ) / nbin;
  energyToRangeMap.clear();
  G4String indent("     ");
  for ( G4int i=0; i<nbin; i++ ) {
    G4double rangeCut = lowerCut + i*step;
    if ( rangeCut < GetDefaultRangeCut() ) continue;
    G4double energy = converter.Convert(rangeCut, material);
    if ( VerboseLevel() > 2 ) {
      G4cout << indent << "For range: " << rangeCut 
             << " mm  got energy "   << energy/MeV << " MeV" << G4endl;
    }         
    
    if ( energy < lowEdgeEnergy )  energy = lowEdgeEnergy;
    if ( energy > highEdgeEnergy ) energy = highEdgeEnergy; 
    
    energyToRangeMap.insert(std::pair<G4double,G4double>(energy/MeV, rangeCut));
  }
    
  // Now find the closest value of energy
  it = energyToRangeMap.lower_bound(energyCut);

  if ( it == energyToRangeMap.end() ) {
    if ( VerboseLevel() > 2 ) {
      G4cout << indent << "Range  not found " << G4endl;
    }
    return false;
  }
  else { 
    if ( VerboseLevel() > 2 ) {
      G4cout << indent << "Found range: " 
             << it->second << " mm" << G4endl;
    }
    return true;  
  }  
}  

//_____________________________________________________________________________
G4double TG4RegionsManager::ConvertEnergyToRange(
                                   G4double energyCut, 
                                   G4Material* material,
                                   G4VRangeToEnergyConverter& converter,
                                   G4double defaultRangeCut) const
{
/// Estimate cut in range for given cut in energy.
/// Return negative value if range cut is not fount.

  G4double lowEdgeEnergy  =  converter.GetLowEdgeEnergy();   
  G4double highEdgeEnergy =  converter.GetHighEdgeEnergy();
  
  // First compute default energy cut
  std::map<G4double, G4double> energyToRangeMap;
  G4double energy = converter.Convert(defaultRangeCut,material);
  if ( energy < lowEdgeEnergy )  energy = lowEdgeEnergy;
  if ( energy > highEdgeEnergy ) energy = highEdgeEnergy; 
  energyToRangeMap.insert(std::pair<G4double,G4double>(energy/MeV, defaultRangeCut));

  // Compute the range order where we will start iterating
  // withing the orders in between fgkMinRangeOrder and fgkMaxRangeOrder
  // (1e-03mm, 1e-02, 1e-01, 1mm, 1cm, 10 cm, 1m)   
  G4String indent("     ");
  for ( G4int i=fgkMinRangeOrder; i<=fgkMaxRangeOrder; i++ ) {
    G4double rangeCut = pow(10.,i);
    if ( rangeCut < defaultRangeCut ) continue;
    G4double energy = converter.Convert(rangeCut,material);
    if ( VerboseLevel() > 2 ) {
      G4cout << indent 
             << "For range: " << std::setw(5) << rangeCut 
             << " mm  got energy "   << energy/MeV <<  " MeV" 
             << G4endl;
    }         
    
    if ( energy < lowEdgeEnergy )  energy = lowEdgeEnergy;
    if ( energy > highEdgeEnergy ) energy = highEdgeEnergy; 
    energyToRangeMap.insert(std::pair<G4double,G4double>(energy/MeV, rangeCut));
  }
  
  // Now find the closest value of energy which is
  // equal or higher than given energyCut
  std::map<G4double, G4double>::const_iterator it
    = energyToRangeMap.lower_bound(energyCut);

  if ( it == energyToRangeMap.end() ) {
    if ( VerboseLevel() > 2 ) {
      G4cout << indent << "Range not found " << G4endl;
    }  
    return -1.0;
  }  
    
  if ( VerboseLevel() > 2 ) {
    G4cout << indent << "Found range: "  << it->second << " mm" << G4endl;
  }  

  // Now iterate up to desired precision of range
  G4int iteration = 0;
  while ( iteration < fRangePrecision ) {
    G4int nbin = fgkNofBins;
    if ( iteration == 0  ) --nbin;
    
    G4double higherCut = it->second;  
    //G4cout << "higherCut = " << higherCut << G4endl;
    --it;
    if ( it == energyToRangeMap.end() ) return higherCut;
  
    G4double lowerCut = it->second;
    //G4cout << "lowerCut = " << lowerCut << G4endl;
    G4bool found 
      = Iterate(energyCut, lowerCut, higherCut, lowEdgeEnergy, highEdgeEnergy,
                nbin, energyToRangeMap, it, material, converter);
              
    if ( ! found )  return higherCut;
    ++iteration;
  }  

  return it->second;;
}            

//_____________________________________________________________________________
G4double TG4RegionsManager::GetRangeCut(
                                    G4double energyCut, 
                                    G4Material* material,
                                    G4VRangeToEnergyConverter& converter,
                                    G4double defaultRangeCut) const
{
/// Convert energy cuts in range cuts;
/// return defaultRangeCut, if found value is smaller than default
    
  if ( energyCut == DBL_MAX ) {
    if ( VerboseLevel() > 1 ) {
      G4cout << "   " 
             << converter.GetParticleType()->GetParticleName() 
             << " energy cut not defined, keeping default range" << G4endl;
    }         
    return defaultRangeCut;
  }  
  
  if ( VerboseLevel() > 1 ) {
    G4cout << "   "
           << converter.GetParticleType()->GetParticleName() 
           << ": energy cut = " << energyCut << " MeV" << G4endl; 
  }  

  G4double rangeCut 
    = ConvertEnergyToRange(energyCut, material, converter, defaultRangeCut);
    
  if ( rangeCut < 0. ) {
    if ( VerboseLevel() > 1 ) {
      G4cout << "  " 
             << converter.GetParticleType()->GetParticleName() 
             << " rangeCut not found, keeping default range" << G4endl;
    }         
    return defaultRangeCut;
  }
    
  if ( rangeCut < defaultRangeCut ) {
    if ( VerboseLevel() > 1 ) {
      G4cout << "  " 
             << converter.GetParticleType()->GetParticleName() 
             << " rangeCut found " << rangeCut << " mm " 
             << " is below default value, it will be ignored" << G4endl;
    }         
    return defaultRangeCut;
  }
  
  return rangeCut;
}     
      
//_____________________________________________________________________________
G4bool TG4RegionsManager::IsCoupleUsedInTheRegion(
                            const G4MaterialCutsCouple* couple,
                            const G4Region* region) const
{
/// Reimplemented G4ProductionCutsTable::IsCoupleUsedInTheRegion
/// which is declared private

  G4ProductionCuts* productionCuts = region->GetProductionCuts();
  std::vector<G4Material*>::const_iterator itm 
    = region->GetMaterialIterator();
  size_t nofMaterials = region->GetNumberOfMaterials();
  for ( size_t i=0; i<nofMaterials; i++, itm++) {
    if ( couple->GetMaterial() == (*itm) &&
         couple->GetProductionCuts() == productionCuts ) {
      return true;
    }
  }
  
  return false;
}
              
//_____________________________________________________________________________
G4bool TG4RegionsManager::CheckCut(TG4Limits* limits, 
                            TG4G3Cut cut, const G4String& particleName,
                            G4double range, G4double energy) const
{
/// Check if energy from region properties is consistent with energy cuts
/// defined in limits. 

  G4double energyLimits = GetEnergyCut(limits, cut, DBL_MAX);
  if ( energyLimits == DBL_MAX) return true;
  
  if ( VerboseLevel() > 1 ) {
    G4cout << "  " << particleName << " cut:" << G4endl
           << "  range = " << range << " mm" << G4endl
           << "  energy from range = " << energy/MeV  << " MeV"
           << "  energy from limits = " << energyLimits << " MeV" << G4endl;
  }         

  // Get default range cut value from physics list
  G4double defaultRangeCut = GetDefaultRangeCut();

  if ( energy < energyLimits*(1.0 - fgkEnergyTolerance) &&
       fabs( range - defaultRangeCut) > fgkRangeTolerance ) {
    G4cout << "  !! " << particleName
           << " cut from range < cut from limits !!"  << G4endl;
    if ( VerboseLevel() <= 1 ) {       
      G4cout << "  energy from range = " << energy/MeV  << " MeV"
             << "  energy from limits = " << energyLimits << " MeV" << G4endl;
    }         
    return false;       
  } 
          
  return true;
}
              
//_____________________________________________________________________________
void TG4RegionsManager::CheckRegionsRanges() const
{
/// Loop over production cuts table and check if region properties 
/// are consistent with energy cuts defined in limits

  // Check consistence of range and energy cuts
  G4ProductionCutsTable* productionCutsTable =  
    G4ProductionCutsTable::GetProductionCutsTable();
    
  Bool_t good = true;
  for ( G4int i=0; i< G4int(productionCutsTable->GetTableSize()); i++ ) {
    const G4MaterialCutsCouple* couple 
      = productionCutsTable->GetMaterialCutsCouple(i);
      
    const G4Material* material = couple ->GetMaterial();  
    G4ProductionCuts* cuts = couple ->GetProductionCuts(); 
    
    G4double rangeGam = cuts->GetProductionCut(0);
    G4double rangeEle = cuts->GetProductionCut(1);
    if ( couple->IsRecalcNeeded() ) {
      TG4Globals::Warning("TG4RegionsManager", "CheckRegions", 
         "Recalculation is needed - cannot perform check"); 
      return;   
    }

    const std::vector<G4double>* energyCutsGam 
      = productionCutsTable->GetEnergyCutsVector(0);
    const std::vector<G4double>* energyCutsEle 
      = productionCutsTable->GetEnergyCutsVector(1);
     
    G4double cutGam = (*energyCutsGam)[couple->GetIndex()];
    G4double cutEle = (*energyCutsEle)[couple->GetIndex()];

    // Get limits via material
    TG4Limits* limits
      = TG4GeometryServices::Instance()->FindLimits(material, true);
    if ( ! limits ) {  
      TG4Globals::Warning("TG4RegionsManager", "CheckRegions", 
         "Limits for material " + TString(material->GetName()) + 
         " not found. " + TG4Globals::Endl() +
         " Cannot perform check.");
      continue;
    }
    
    if ( VerboseLevel() > 1 ) {
      G4cout << "In material: " << material->GetName() << G4endl;
    }    

    G4bool checkGam = CheckCut(limits, kCUTGAM, "gamma", rangeGam, cutGam);
    G4bool checkEle = CheckCut(limits, kCUTELE, "e-", rangeEle, cutEle);
    
    if ( ! checkGam || ! checkEle ) good = false;     
  }
  if ( good ) {
      G4cout << ".. Ranges in regions are consistent with energy cuts." 
             << G4endl;
  }    
}             
         
//_____________________________________________________________________________
void TG4RegionsManager::CheckRegionsInGeometry() const
{
/// Loop over all logical volumes and check if the region to which
/// the volume belongs correspond to its material


  Bool_t good = true;
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i=0; i<G4int(lvStore->size()); i++) {
  
    G4LogicalVolume* lv = (*lvStore)[i];
    
    // skip volumes without media 
    TG4Medium* medium 
      = TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(lv, false);
    if ( ! medium ) continue;  
    
    if ( lv->GetRegion()->GetName() != lv->GetMaterial()->GetName() &&
         lv->GetRegion()->GetName() != fgkDefaultRegionName ) {
    
      G4cout << "The region name " << lv->GetRegion()->GetName() 
             << " for LV = " << lv->GetName() 
             << " does not match its material name " 
             << lv->GetMaterial()->GetName() << G4endl;
      
      good = false;       
    }
  }
  if ( good ) {
      G4cout << ".. Regions are consistent with materials."  << G4endl;
  }    
}            
                         
//
// public methods
//

//_____________________________________________________________________________
void TG4RegionsManager::DefineRegions()
{
/// Apply cuts defined in tracking media 

  if ( VerboseLevel() > 0 )
    G4cout << "Converting VMC cuts in regions" << G4endl; 
  
  // Create G4 range to energy converters
  G4RToEConvForElectron g4ConverterEle;
  G4RToEConvForGamma g4ConverterGam;
  
  // Get default range cut value from physics list
  G4double defaultRangeCut = GetDefaultRangeCut();

  // Get default region 
  G4Region* worldRegion = GetDefaultRegion();

  // Get global energy cuts
  G4double cutEleGlobal = GetGlobalEnergyCut(kCUTELE);
  G4double cutGamGlobal = GetGlobalEnergyCut(kCUTGAM);
  if ( VerboseLevel() > 2 ) {
      G4cout << "Global cut values: "
             << "  CUTELE = " << cutEleGlobal << " MeV"
             << "  CUTGAM = " << cutGamGlobal << " MeV" << G4endl; 
  }           
  
  // Define region for each logical volume  
  //
  
  G4int counter = 0;
  std::set<G4Material*> processedMaterials;
  
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(lvStore->size()); i++) {
  
    G4LogicalVolume* lv = (*lvStore)[i];
    if ( lv->IsRootRegion() ) {
    
      if ( VerboseLevel() > 1 ) {
        G4cout << "Skipping Root region LV = " << lv->GetName() << G4endl;
      }  
      continue;
    }   

    TG4Medium* medium 
      = TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(lv, false);
    if ( ! medium ) continue;  
      
    // If region already exists, only add the logical volume
    // and continue the loop   
    G4Material* material = lv->GetMaterial();
    G4String regionName = material->GetName();
    G4Region* region 
      = G4RegionStore::GetInstance()->GetRegion(regionName, false );
    if ( region ) {
      if ( VerboseLevel() > 1 ) {
        G4cout << "   " << "adding volume in region = " << regionName << G4endl;
      }
      region->AddRootLogicalVolume(lv);
      continue;
    } 

    if ( VerboseLevel() > 1 ) {
      G4cout << "-- Volume = "  << i << "  "
             << lv << "  " << lv->GetName()
             << " material = "  << material->GetName() << G4endl;
    }         

    // If this material was already processed and did not result
    // in a new region: add the logical volume to the world region
    if ( processedMaterials.find(material) != processedMaterials.end() ) {
      if ( VerboseLevel() > 1 ) {
        G4cout << "   " << "adding volume in default World region" << G4endl;
      }         
      worldRegion->AddRootLogicalVolume(lv);
      continue;
    } 
    
    // Get energy cuts defined in limits
    TG4Limits* limits = (TG4Limits*) lv->GetUserLimits();
    G4double cutEle = GetEnergyCut(limits, kCUTELE, cutEleGlobal);
    G4double cutGam = GetEnergyCut(limits, kCUTGAM, cutGamGlobal);

    // Convert energy cuts defined in limits in range cuts
    G4double rangeEle 
      = GetRangeCut(cutEle, material, g4ConverterEle, defaultRangeCut);
    G4double rangeGam 
      = GetRangeCut(cutGam, material, g4ConverterGam, defaultRangeCut);
    if ( VerboseLevel() > 1 ) {
      G4cout << ".. converted in e- rangeCut = " << rangeEle << " mm  " 
             << "gamma rangeCut = " << rangeGam << " mm" << G4endl;
    }          
           
    if ( fabs ( rangeEle - defaultRangeCut ) < 1e-03 && 
         fabs ( rangeGam - defaultRangeCut ) < 1e-03 ) {
      // Do not create a new region if range cuts do not differ
      // from those in default region
      worldRegion->AddRootLogicalVolume(lv);
      processedMaterials.insert(material);
    }  
    else {           
      // Create new region
      region = new G4Region(regionName);
      region->AddRootLogicalVolume(lv);
      ++counter;

      G4ProductionCuts* cuts = new G4ProductionCuts();
      cuts->SetProductionCut(rangeGam, 0);
      cuts->SetProductionCut(rangeEle, 1);
      cuts->SetProductionCut(defaultRangeCut, 2);
      region->SetProductionCuts(cuts);
    }  
  }
  
  if ( VerboseLevel() > 0 ) {
    G4cout << "Number of added regions: " << counter << G4endl;
  }  
}    


//_____________________________________________________________________________
void TG4RegionsManager::DumpRegion(const G4String& volName) const
{
/// Dump region properties for volume with given name

  // Get volume
  G4LogicalVolume* lv 
    = TG4GeometryServices::Instance()->FindLogicalVolume(volName);
  if ( ! lv ) return;
  
  // Get region
  G4Region* region = lv->GetRegion();
  if ( ! region )  {
    TG4Globals::Warning(
       "TG4RegionsManager", "DumpRegion",
       "Region for volume " + TString(volName) + "not found.");
       return;
  }
 
  G4ProductionCuts* cuts = region->GetProductionCuts(); 
  
  // Range cuts
  G4cout << G4endl;
  G4cout << " Material : " << lv->GetMaterial()->GetName() << G4endl;
  G4cout << " Range cuts        : " 
         << " gamma " << G4BestUnit(cuts->GetProductionCut("gamma"),"Length")
         << "    e- " << G4BestUnit(cuts->GetProductionCut("e-"),"Length")
         << "    e+ " << G4BestUnit(cuts->GetProductionCut("e+"),"Length")
         << G4endl;
         
  // Energy cuts
  G4cout << " Energy thresholds : ";

  G4ProductionCutsTable* productionCutsTable 
    = G4ProductionCutsTable::GetProductionCutsTable();
    
  const G4MaterialCutsCouple* couple
    = productionCutsTable->GetMaterialCutsCouple(lv->GetMaterial(), cuts);
  if ( ! couple || couple->IsRecalcNeeded() ) {   
    G4cout << " not ready to print" << G4endl;
    return;
  } 
  
  const std::vector<G4double>* energyCutsGam 
    = productionCutsTable->GetEnergyCutsVector(0);
  const std::vector<G4double>* energyCutsEle 
    = productionCutsTable->GetEnergyCutsVector(1);
  const std::vector<G4double>* energyCutsPos 
    = productionCutsTable->GetEnergyCutsVector(2);
    
  G4double cutGam = (*energyCutsGam)[couple->GetIndex()];
  G4double cutEle = (*energyCutsEle)[couple->GetIndex()];
  G4double cutPos = (*energyCutsPos)[couple->GetIndex()];
  
  G4cout << " gamma " << G4BestUnit(cutGam,"Energy")
         << "    e- " << G4BestUnit(cutEle,"Energy")
         << "    e+ " << G4BestUnit(cutPos,"Energy");
  G4cout << G4endl;

  G4RegionStore* regionStore =  G4RegionStore::GetInstance();
  if ( couple->IsUsed() ) {
     G4cout << " Region(s) which use this couple : ";
     std::vector<G4Region*>::iterator it;
     for ( it = regionStore->begin(); it != regionStore->end(); it++ ) {
       if ( IsCoupleUsedInTheRegion(couple, *it) ) {
         G4cout << "    " << (*it)->GetName();
       }
     }
     G4cout << G4endl;
  }
}  

//_____________________________________________________________________________
void  TG4RegionsManager::CheckRegions() const
{
/// Perform two checks:
/// -  if region properties are consistent with energy cuts defined in limits
/// -  if the region to which the volume belongs correspond to its material

  G4cout << "Checking regions:" << G4endl;

  CheckRegionsRanges();
  CheckRegionsInGeometry();
}  
  
