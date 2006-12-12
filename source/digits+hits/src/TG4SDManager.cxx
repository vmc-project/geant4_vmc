// $Id: TG4SDManager.cxx,v 1.4 2006/01/13 16:59:38 brun Exp $
// Category: digits+hits
//
// Class TG4SDManager
// -------------------
// See the class description in the header file.
//
// Author: I.Hrivnacova

#include "TG4SDManager.h"
#include "TG4SDConstruction.h"
#include "TG4SDServices.h"
#include "TG4GeometryServices.h"
#include "TG4MediumMap.h"
#include "TG4Medium.h"
#include "TG4Globals.h"

TG4SDManager* TG4SDManager::fgInstance = 0;

//_____________________________________________________________________________
TG4SDManager::TG4SDManager()
  : fSDConstruction(0),
    fSDServices(0)
     
{
//

  if (fgInstance)
    TG4Globals::Exception(
      "TG4SDManager", "TG4SDManager", 
      "Cannot create two instances of singleton.");
      
  fgInstance = this; 
  
  fSDConstruction = new TG4SDConstruction();
  fSDServices = new TG4SDServices();
}

//_____________________________________________________________________________
TG4SDManager::~TG4SDManager()
{
//

  delete fSDConstruction;
  delete fSDServices;
}

//
// public methods 
//

//_____________________________________________________________________________
void TG4SDManager::Initialize() 
{
/// Create sensitive detectors,
/// sets second indexes for materials (corresponding to G3 tracking 
/// media) and clear remaing G3 tables.

  fSDConstruction->Construct();
}  
  

//_____________________________________________________________________________
Int_t TG4SDManager::VolId(const Text_t* volName) const
{ 
/// Return the volume ID = sensitive detector identifier.

  return fSDServices->GetVolumeID(volName);
}


//_____________________________________________________________________________
Int_t TG4SDManager::MediumId(const Text_t* medName) const
{
// Returns the medium id for medium with given name

  TG4MediumMap* mediumMap = TG4GeometryServices::Instance()->GetMediumMap();
  TG4Medium* medium = mediumMap->GetMedium(G4String(medName), false);
  
  if ( ! medium ) {
    TG4Globals::Warning(
      "TG4SDManager", "MediumId", 
      "Medium " + TString(medName) + " not found.");
    return 0;
  }    

  return medium->GetID();
}                   

//_____________________________________________________________________________
const char* TG4SDManager::VolName(Int_t id) const
{
/// Return the name of the volume specified by volume ID
/// ( = sensitive detector name)

  return fSDServices->GetVolumeName(id);
}


//_____________________________________________________________________________
Int_t TG4SDManager::NofVolumes() const
{
/// Return the total number of VMC volumes 
/// ( = number of sensitive detectors).

  return fSDServices->NofSensitiveDetectors();
}


//_____________________________________________________________________________
Int_t TG4SDManager::NofVolDaughters(const char* volName) const
{
/// Return the number of daughters of the volume specified by name
  
  return fSDServices->NofVolDaughters(volName);  
}

//_____________________________________________________________________________
const char*  TG4SDManager::VolDaughterName(const char* volName, Int_t i) const
{
/// Return the name of the i-th daughter of the volume specified by name.
// --- 

  return fSDServices->VolDaughterName(volName, i);  
}

//_____________________________________________________________________________
Int_t  TG4SDManager::VolDaughterCopyNo(const char* volName, Int_t i) const
{
/// Return the copyNo of the i-th daughter of the volume specified by name.
// 

  return fSDServices->VolDaughterCopyNo(volName, i);  
}

 
//_____________________________________________________________________________
Int_t TG4SDManager::VolId2Mate(Int_t volumeId)  const
{
/// Return the material number for a given volume id

  return fSDServices->GetMediumId(volumeId);                                
}
 
