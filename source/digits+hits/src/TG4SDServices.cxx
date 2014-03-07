// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SDServices.cxx
/// \brief Implementation of the TG4SDServices class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SDServices.h"
#include "TG4SensitiveDetector.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"

#include <G4VSensitiveDetector.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <iomanip>

TG4SDServices* TG4SDServices::fgInstance = 0;

//_____________________________________________________________________________
TG4SDServices::TG4SDServices()
  : fIsStopRun(false), 
    fVolNameToIdMap(),
    fVolIdToLVMap(),
    fLVToVolIdMap()
    
   
{
/// Default constructor

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4SDServices", "TG4SDServices", 
      "Cannot create two instances of singleton.");
  }
  fgInstance = this;
}

//_____________________________________________________________________________
TG4SDServices::~TG4SDServices()
{
/// Destructor
}

//
// public methods 
//

//_____________________________________________________________________________
void TG4SDServices::MapVolume(G4LogicalVolume* lv, G4int id)
{
/// Add the given volume in the maps.
/// Do nothing if a given volume id or name is already present.

  // cut copy number from sdName
  G4String volName 
    = TG4GeometryServices::Instance()->UserVolumeName(lv->GetName());
  
  if ( fVolNameToIdMap.find(volName) == fVolNameToIdMap.end() )
    fVolNameToIdMap[volName] = id;
  
  if ( fVolIdToLVMap.find(id) == fVolIdToLVMap.end() )  
    fVolIdToLVMap[id] = lv;

  if ( fLVToVolIdMap.find(lv) == fLVToVolIdMap.end() )
    fLVToVolIdMap[lv] = id;
}  

//_____________________________________________________________________________
void TG4SDServices::PrintStatistics(G4bool open, G4bool close) const
{
/// Print G4 SD statistics
  
  if (open)  TG4Globals::PrintStars(true);
     
   G4cout << "          " << std::setw(5) << NofSensitiveDetectors()  
                          << " sensitive detectors" << G4endl;

  if (close) TG4Globals::PrintStars(false);
}

//_____________________________________________________________________________
void TG4SDServices::PrintVolNameToIdMap() const
{
/// Print volume name -> volume id map

  if ( fVolNameToIdMap.size() ) {
    G4cout << "Dump of VolNameToIdMap - " << fVolNameToIdMap.size() 
           << " entries:" << G4endl;
    G4int counter = 0;
    std::map<G4String, G4int>::const_iterator it;
    for ( it = fVolNameToIdMap.begin(); it != fVolNameToIdMap.end(); ++it ) {
      G4cout << "Map element " << std::right << std::setw(3) << counter++ << "   ";
      G4cout << std::left << std::setw(20) << it->first <<  "   "; 
      G4cout << std::right << std::setw(8) << it->second <<  "   "; 
      G4cout << G4endl;
    }
  }
}  

//_____________________________________________________________________________
void TG4SDServices::PrintVolIdToLVMap() const
{
/// Print volume id -> lv map

  if ( fVolIdToLVMap.size() ) {
    G4cout << "Dump of VolIdToLVMap - " << fVolIdToLVMap.size() 
           << " entries:" << G4endl;
    G4int counter = 0;
    std::map<G4int, G4LogicalVolume*>::const_iterator it;
    for ( it = fVolIdToLVMap.begin(); it != fVolIdToLVMap.end(); ++it ) {
      G4cout << "Map element "<< std::right << std::setw(3) << counter++ << "   ";
      G4cout << std::right << std::setw(8)  << it->first <<  "   "; 
      G4cout << std::right << std::setw(12) << it->second <<  "   "; 
      G4cout << std::left  << std::setw(20) << it->second->GetName() <<  "   ";
      G4cout << std::right << G4endl;
    }
  }
}  

//_____________________________________________________________________________
void TG4SDServices::PrintSensitiveVolumes() const
{
/// Print volumes which have defined SD

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  G4cout << "Sensitive volumes (volId, volName): " << G4endl;
  for ( G4int i=0; i<G4int(lvStore->size()); i++ ) {
    G4LogicalVolume* lv = (*lvStore)[i];
    if ( lv->GetSensitiveDetector() ) {
      G4cout << "   ";
      G4cout << std::right << std::setw(4)  << GetVolumeID(lv) <<  "   "; 
      G4cout << std::left << std::setw(20) << lv->GetName(); 
      G4cout << std::right << G4endl;
    }
  }  
}  

//_____________________________________________________________________________
G4int TG4SDServices::GetVolumeID(const G4String& volName) const
{ 
/// Return the volume identifier from volumes name map.

  G4String g4VolName
    = TG4GeometryServices::Instance()->CutName(volName);
    
  std::map<G4String, G4int>::const_iterator it 
    = fVolNameToIdMap.find(volName);
    
  if ( it == fVolNameToIdMap.end() ) { 
    TG4Globals::Warning(
      "TG4SDServices", "GetVolumeID", 
      "Unknown Volume Id for " + TString(volName.data())); 
    return 0;
  }  
    
  return it->second;  
}


//_____________________________________________________________________________
G4int TG4SDServices::GetVolumeID(G4LogicalVolume* logicalVolume) const 
{
/// Return the volume ID of the specified logical volume.
/// The volume ID is defined via associated sensitive detector ID or
/// is taken from a map (if user applies sentitive volumes selection.)

 // return logicalVolume->GetInstanceID();

#ifdef MCDEBUG
  G4VSensitiveDetector* sd
    = logicalVolume->GetSensitiveDetector();

  if (sd) {
    return GetSensitiveDetector(sd)->GetID();
  }  
  else {
    std::map<G4LogicalVolume*, G4int>::const_iterator it;
    it = fLVToVolIdMap.find(logicalVolume);
    if ( it != fLVToVolIdMap.end() ) {
      return it->second;
    }  
    else {    
      TG4Globals::Warning(
      "TG4SDServices", "GetVolumeID", 
      "Unknown Volume Id for " + TString(logicalVolume->GetName())); 
     return 0;
    } 
  }  
#else
  G4VSensitiveDetector* sd
    = logicalVolume->GetSensitiveDetector();
  if ( sd ) 
    return ((TG4SensitiveDetector*)sd)->GetID(); 
  else {
    std::map<G4LogicalVolume*, G4int>::const_iterator it;
    it = fLVToVolIdMap.find(logicalVolume);
    if ( it != fLVToVolIdMap.end() ) {
      return it->second;
    }  
    else {    
      TG4Globals::Warning(
      "TG4SDServices", "GetVolumeID", 
      "Unknown Volume Id for" + TString(logicalVolume->GetName())); 
     return 0;
    } 
  }  
#endif   
} 


//_____________________________________________________________________________
G4int TG4SDServices::GetMediumID(G4LogicalVolume* logicalVolume) const 
{
/// Return the medium ID for the specified logical volume.
 
#ifdef MCDEBUG
  G4VSensitiveDetector* sd
    = logicalVolume->GetSensitiveDetector();

  if (sd) {
    return GetSensitiveDetector(sd)->GetMediumID();
  }  
  else {
    return TG4GeometryServices::Instance()->GetMediumId(logicalVolume);
  }  
#else
  G4VSensitiveDetector* sd
    = logicalVolume->GetSensitiveDetector();
  if ( sd ) 
    return ((TG4SensitiveDetector*)sd)->GetMediumID(); 
  else {
    return TG4GeometryServices::Instance()->GetMediumId(logicalVolume);
  }  
#endif   
} 


//_____________________________________________________________________________
G4String TG4SDServices::GetVolumeName(G4int volumeId) const
{
/// Return the name of the volume with the given identifier.
/// Give a warning in case logical volume is not associated with 
/// a sensitive detector.

  G4LogicalVolume* lv = GetLogicalVolume(volumeId, false);
  
  if ( ! lv ) {
    TString text = "volumeId=";
    text += volumeId;
    TG4Globals::Warning(
      "TG4SDServices", "GetVolumeName", 
      "Sensitive detector with " + text + " is not defined.");
    return "";
  }  

  return TG4GeometryServices::Instance()->UserVolumeName(lv->GetName());  
}


//_____________________________________________________________________________
G4LogicalVolume* TG4SDServices::GetLogicalVolume(G4int volumeId, G4bool warn) const 
{
/// Return the first found logical volume with specified volumeId 
/// (sensitive detector ID) in G4LogicalVolumeStore.

  std::map<G4int, G4LogicalVolume*>::const_iterator it 
    = fVolIdToLVMap.find(volumeId);
    
  if ( it == fVolIdToLVMap.end() ) { 
    if ( warn ) {
      TString text = "volumeId=";
      text += volumeId;
      TG4Globals::Warning(
        "TG4SDServices", "GetLogicalVolume", 
        "Sensitive detector with " + text + " is not defined.");
    }    
    return 0;
  }  
    
  return it->second;  
}  


//_____________________________________________________________________________
Int_t TG4SDServices::GetMediumId(G4int volumeId)  const
{
/// Return the material number for a given volume Id

  return TG4GeometryServices::Instance()
            ->GetMediumId(GetLogicalVolume(volumeId));                                
}
 

//_____________________________________________________________________________
Int_t TG4SDServices::NofSensitiveDetectors() const
{
/// Return the total number of sensitive detectors.

  return TG4SensitiveDetector::GetTotalNofSensitiveDetectors();
}

//_____________________________________________________________________________
TG4SensitiveDetector* TG4SDServices::GetSensitiveDetector(
                                         G4VSensitiveDetector* sd) const
{
/// Check and convert type of the sensitive detector.

  if (!sd) return 0;
  
  TG4SensitiveDetector* tsd = dynamic_cast<TG4SensitiveDetector*>(sd);
  
  if (!tsd) {
    TG4Globals::Exception(
      "TG4SDServices", "GetSensitiveDetector", 
      "Wrong sensitive detector type.");
    return 0;
  }    
  else 
    return tsd;  
} 

//_____________________________________________________________________________
Int_t TG4SDServices::NofVolDaughters(const char* volName) const
{
/// Return the number of daughter of the volume specified by name
  
  G4int volId = GetVolumeID(volName);
  G4LogicalVolume* lv = GetLogicalVolume(volId);
  
  if (!lv) return 0;
  
  return lv->GetNoDaughters();  
}

//_____________________________________________________________________________
const char*  TG4SDServices::VolDaughterName(const char* volName, Int_t i) const
{
/// Return the name of the i-th daughter of the volume specified by name.

  G4int volId = GetVolumeID(volName);
  G4LogicalVolume* lv = GetLogicalVolume(volId);
  
  if (!lv) return "";

  G4int nofDaughters = lv->GetNoDaughters();
  if (i<0 || i>=nofDaughters) {
    TString text =  "index=";
    text += i;
    TG4Globals::Warning(
      "TG4SDServices", "VolDaughterName", 
      "Mother volume " + TString(volName) + " has no daughter with "
      + text + ".");
    return "";
  }    
 
  G4VPhysicalVolume* daughter = lv->GetDaughter(i);
  G4String g4Name = daughter->GetLogicalVolume()->GetName();
  
  return TG4GeometryServices::Instance()->UserVolumeName(g4Name);
  
}

//_____________________________________________________________________________
Int_t  TG4SDServices::VolDaughterCopyNo(const char* volName, Int_t i) const
{
/// Return the copyNo of the i-th daughter of the volume specified by name.

  G4int volId = GetVolumeID(volName);
  G4LogicalVolume* lv = GetLogicalVolume(volId);

  if (!lv) return 0;

  G4int nofDaughters = lv->GetNoDaughters();
  if (i<0 || i>=nofDaughters) {
    TString text =  "index=";
    text += i;
    TG4Globals::Warning(
      "TG4SDServices", "VolDaughterCopyNo", 
      "Mother volume " + TString(volName) + " has no daughter with "+
      text + ".");
    return 0;
  }    
 
  return lv->GetDaughter(i)->GetCopyNo();
}

 



 
