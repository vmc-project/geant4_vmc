// $Id: TG4SDServices.cxx,v 1.7 2007/05/11 09:22:44 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: digits+hits
// Class TG4SDServices
// --------------------
// See the class description in the header file.

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
  : fIsStopRun(false) 
{
//
  if (fgInstance) {
    TG4Globals::Exception(
      "TG4SDServices", "TG4SDServices", 
      "Cannot create two instances of singleton.");
  }
  fgInstance = this;
}

//_____________________________________________________________________________
TG4SDServices::~TG4SDServices(){
//
}

//
// public methods 
//

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
G4int TG4SDServices::GetVolumeID(const G4String& volName) const
{ 
/// Return the sensitive detector identifier.
/// !! Give exception in case logical volume is not associated with 
/// a sensitive detector.

  G4String g4VolName
    = TG4GeometryServices::Instance()->CutName(volName);

  G4LogicalVolumeStore* pLVStore = G4LogicalVolumeStore::GetInstance();
  
  for (G4int i=0; i<G4int(pLVStore->size()); i++) {
    G4LogicalVolume* lv = (*pLVStore)[i];
    G4VSensitiveDetector* sd = lv->GetSensitiveDetector();
  
    if (sd && sd->GetName() == g4VolName) 
        return GetSensitiveDetector(sd)->GetID();
  }

  TG4Globals::Warning(
    "TG4SDServices", "GetVolumeID", 
    "Sensitive detector " + TString(volName) + " is not defined."); 
  return 0;
}


//_____________________________________________________________________________
G4int TG4SDServices::GetVolumeID(G4LogicalVolume* logicalVolume) const 
{
/// Return the sensitive detector ID of the specified
/// logical volume.
 
  G4VSensitiveDetector* sd
    = logicalVolume->GetSensitiveDetector();

  if (sd) return GetSensitiveDetector(sd)->GetID();

  TG4Globals::Warning(
    "TG4SDServices", "GetVolumeID", 
    "Volume " + TString(logicalVolume->GetName()) + 
    " has not a sensitive detector."); 
  return 0;
} 


//_____________________________________________________________________________
G4String TG4SDServices::GetVolumeName(G4int volumeId) const
{
/// Return the name of the sensitive detector with the given identifier.
/// Give a warning in case logical volume is not associated with 
/// a sensitive detector.

  G4LogicalVolumeStore* pLVStore = G4LogicalVolumeStore::GetInstance();
  
  for (G4int i=0; i<G4int(pLVStore->size()); i++) {
    G4LogicalVolume* lv = (*pLVStore)[i];
    G4VSensitiveDetector* sd = lv->GetSensitiveDetector();
    
    if (sd && GetSensitiveDetector(sd)->GetID() == volumeId) 
        return sd->GetName();
  }

  TString text = "volumeId=";
  text += volumeId;
  TG4Globals::Warning(
    "TG4SDServices", "VolName", 
    "Sensitive detector with " + text + " is not defined.");
  return "";                                
}


//_____________________________________________________________________________
G4LogicalVolume* TG4SDServices::GetLogicalVolume(G4int volumeId) const 
{
/// Find the first logical volume with specified volumeId 
/// (sensitive detector ID) in G4LogicalVolumeStore.

  G4LogicalVolumeStore* pLVStore = G4LogicalVolumeStore::GetInstance();
  
  for (G4int i=0; i<G4int(pLVStore->size()); i++) {
    G4LogicalVolume* lv = (*pLVStore)[i];
    if (GetVolumeID(lv) == volumeId) return lv;
  }
  
  G4String text = "TG4SDServices::GetLogicalVolume: \n";
  text = text + "    Logical volume with given ID does not exist.";
  return 0;                                
}  


//_____________________________________________________________________________
Int_t TG4SDServices::GetMediumId(G4int volumeId)  const
{
/// Return the material number for a given volume id

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
/// Check and converts type of the sensitive detector.

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
/// Return number of daughter of the volume specified by name
  
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

 



 
