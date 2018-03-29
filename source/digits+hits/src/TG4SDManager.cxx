//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SDManager.cxx
/// \brief Implementation of the TG4SDManager class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SDManager.h"
#include "TG4SDConstruction.h"
#include "TG4SDServices.h"
#include "TG4GeometryServices.h"
#include "TG4MediumMap.h"
#include "TG4Medium.h"
#include "TG4Globals.h"

#include <TVirtualMC.h>


TG4SDManager* TG4SDManager::fgInstance = 0;

//_____________________________________________________________________________
TG4SDManager::TG4SDManager()
  : fSDConstruction(0),
    fSDServices(0),
    fNameBuffer()
     
{
/// Default constructor

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
/// Destructor

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

  //G4cout << "TG4SDManager::Initialize" << G4endl;
  fSDConstruction->Construct();
  //G4cout << "TG4SDManager::Initialize done" << G4endl;
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

  fNameBuffer = fSDServices->GetVolumeName(id).c_str();
  return fNameBuffer.data();
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

  return fSDServices->VolDaughterName(volName, i);  
}

//_____________________________________________________________________________
Int_t  TG4SDManager::VolDaughterCopyNo(const char* volName, Int_t i) const
{
/// Return the copyNo of the i-th daughter of the volume specified by name.

  return fSDServices->VolDaughterCopyNo(volName, i);  
}

 
//_____________________________________________________________________________
Int_t TG4SDManager::VolId2Mate(Int_t volumeId)  const
{
/// Return the material number for a given volume Id

  return fSDServices->GetMediumId(volumeId);                                
}
 
