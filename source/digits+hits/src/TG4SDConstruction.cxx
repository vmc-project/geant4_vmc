// $Id: TG4SDConstruction.cxx,v 1.2 2004/07/09 16:56:08 brun Exp $
// Category: digits+hits
//
// Author: I.Hrivnacova
//
// Class TG4SDConstruction
// ------------------------
// See the class description in the header file.

#include "TG4SDConstruction.h"
#include "TG4SensitiveDetector.h"
#include "TG4GeometryServices.h"

#include <G4SDManager.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>

#include <TVirtualMCApplication.h>

//_____________________________________________________________________________
TG4SDConstruction::TG4SDConstruction()    
  : TG4Verbose("SDConstruction") {
//
}

//_____________________________________________________________________________
TG4SDConstruction::~TG4SDConstruction(){
//
}

//
// private methods
//

//_____________________________________________________________________________
void TG4SDConstruction::CreateSD(G4LogicalVolume* lv) const
{ 
/// Create/retrieve a sensitive detector for the logical volume.

  TG4GeometryServices* geometryServices = TG4GeometryServices::Instance();
  G4SDManager* pSDManager = G4SDManager::GetSDMpointer();

  G4String lvName = lv->GetName();   
  G4String sdName = "/" + lvName;

  // cut copy number from sdName
  sdName = geometryServices->G4ToG3VolumeName(sdName);
  
  // create/retrieve the sensitive detector
  G4VSensitiveDetector* sd = 0; 
  sd = pSDManager->FindSensitiveDetector(sdName, false);
  if (!sd) {
    TG4SensitiveDetector* newSD = new TG4SensitiveDetector(sdName);	
    pSDManager->AddNewDetector(newSD);

    if (VerboseLevel() > 1) {
      G4cout << "Sensitive detector " << sdName << "(" 
             << newSD->GetID() << ") has been created." << G4endl;
    }
    	     
    sd = newSD;  
  }	
  lv->SetSensitiveDetector(sd);	     
}

//
// public methods
//

//_____________________________________________________________________________
void TG4SDConstruction::Construct()
{ 
/// Create sensitive detectors and initialize the VMC application.
/// Sensitive detectors are set to all logical volumes
  
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  
  // OFF VMC - comment next line
  for (G4int i=0; i<G4int(lvStore->size()); i++) CreateSD((*lvStore)[i]);
  
  TVirtualMCApplication::Instance()->InitGeometry();
}


