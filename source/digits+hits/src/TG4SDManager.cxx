// $Id: TG4SDManager.cxx,v 1.2 2004/10/12 07:47:11 brun Exp $
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
#include "TG4Globals.h"

TG4SDManager* TG4SDManager::fgInstance = 0;

//_____________________________________________________________________________
TG4SDManager::TG4SDManager(TG4SDConstruction* sdConstruction)
  : fSDConstruction(sdConstruction) {
//
  if (fgInstance)
    TG4Globals::Exception(
      "TG4SDManager: attempt to create two instances of singleton.");
      
  fgInstance = this; 
  
  fSDServices = new TG4SDServices();
}

//_____________________________________________________________________________
TG4SDManager::TG4SDManager(const TG4SDManager& right) {
// 
  TG4Globals::Exception(
    "Attempt to copy TG4SDManager singleton.");
}


//_____________________________________________________________________________
TG4SDManager::TG4SDManager() {
// 
  TG4Globals::Exception(
    "TG4SDManager has protected the default constructor.");
}


//_____________________________________________________________________________
TG4SDManager::~TG4SDManager(){
//

  delete fSDServices;
}

//
// operators
//

//_____________________________________________________________________________
TG4SDManager& TG4SDManager::operator=(const TG4SDManager& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception(
    "Attempt to assign TG4SDManager singleton.");
    
  return *this;  
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
 
