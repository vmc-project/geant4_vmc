// $Id: TG4SDServices.cxx,v 1.4 2004/10/12 07:47:11 brun Exp $
// Category: digits+hits
//
// Class TG4SDServices
// --------------------
// See the class description in the header file.
//
// Author: I.Hrivnacova

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
      "TG4SDServices: attempt to create two instances of singleton.");
  }
  fgInstance = this;
}

//_____________________________________________________________________________
TG4SDServices::TG4SDServices(const TG4SDServices& right) {
// 
  TG4Globals::Exception(
    "Attempt to copy TG4SDServices singleton.");
}


//_____________________________________________________________________________
TG4SDServices::~TG4SDServices(){
//
}

//
// operators
//

//_____________________________________________________________________________
TG4SDServices& TG4SDServices::operator=(const TG4SDServices& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception(
    "Attempt to assign TG4SDServices singleton.");
    
  return *this;  
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

  G4String text = "TG4SDServices::GetVolumeID: Sensitive detector ";
  text = text + g4VolName;
  text = text + " is not defined.\n"; 
  TG4Globals::Warning(text);
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

  G4String text = "TG4SDServices::GetVolumeID: \n";
  text = text + "    Volume " + logicalVolume->GetName();
  text = text + " has not a sensitive detector.";
  //TG4Globals::Exception(text);
  TG4Globals::Warning(text);
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

  G4String text = "TG4SDServices::VolName:\n";
  text = text + "    Sensitive detector with given id is not defined. \n";
  TG4Globals::Warning(text);
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
      "TG4SDServices::GetSensitiveDetector: Wrong sensitive detector type.");
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
     G4cerr << "Mother volume name: " << volName
            << "  index: " << i << G4endl;
     TG4Globals::Warning(
      "TG4SDServices::VolDaughterName: Wrong index.");
    return "";
  }    
 
  G4VPhysicalVolume* daughter = lv->GetDaughter(i);
  G4String g4Name = daughter->GetLogicalVolume()->GetName();
  
  return TG4GeometryServices::Instance()->G4ToG3VolumeName(g4Name);
  
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
     G4cerr << "Mother volume name: " << volName
            << "  index: " << i << G4endl;
     TG4Globals::Warning(
      "TG4SDServices::VolDaughterCopyNo: Wrong index.");
    return 0;
  }    
 
  return lv->GetDaughter(i)->GetCopyNo();
}

 



 
