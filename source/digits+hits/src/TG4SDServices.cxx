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
  : fIsStopRun(false)
    //fVolNameToIdMap()
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
void TG4SDServices::PrintStatistics(G4bool open, G4bool close) const
{
/// Print G4 SD statistics
  
  if (open)  TG4Globals::PrintStars(true);
     
   G4cout << "          " << std::setw(5)
          << TG4SensitiveDetector::GetTotalNofSensitiveDetectors()
          << " sensitive detectors" << G4endl;

  if (close) TG4Globals::PrintStars(false);
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
      G4cout << std::right << std::setw(4)  << lv->GetInstanceID() <<  "   ";
      G4cout << std::left << std::setw(20) << lv->GetName(); 
      G4cout << std::right << G4endl;
    }
  }  
}  

//_____________________________________________________________________________
G4int TG4SDServices::GetMediumId(G4LogicalVolume* logicalVolume) const
{
/// Return the medium Id for the specified logical volume.
 
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

