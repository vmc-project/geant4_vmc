// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SDConstruction.cxx
/// \brief Implementation of the TG4SDConstruction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SDConstruction.h"
#include "TG4SDServices.h"
#include "TG4SensitiveDetector.h"
#include "TG4GeometryServices.h"
#include "TG4StateManager.h"

#include <G4SDManager.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>

#include <TVirtualMCApplication.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>

const G4String  TG4SDConstruction::fgkDefaultSVLabel = "SV";

//_____________________________________________________________________________
TG4SDConstruction::TG4SDConstruction()    
  : TG4Verbose("SDConstruction"),
    fMessenger(this),
    fSelectionFromTGeo(false),
    fSVLabel(fgkDefaultSVLabel), 
    fSelection()
{
/// Default constructor
}

//_____________________________________________________________________________
TG4SDConstruction::~TG4SDConstruction()
{
/// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
G4int TG4SDConstruction::CreateSD(G4LogicalVolume* lv) const
{ 
/// Create/retrieve a sensitive detector for the given logical volume.
/// Return the ID of created/retrievd sensitive detector,

  TG4GeometryServices* geometryServices = TG4GeometryServices::Instance();
  G4SDManager* pSDManager = G4SDManager::GetSDMpointer();

  G4String lvName = lv->GetName();   
  G4String sdName = "/" + lvName;

  // cut copy number from sdName
  sdName = geometryServices->UserVolumeName(sdName);
  
  // create/retrieve the sensitive detector
  G4VSensitiveDetector* sd = 0; 
  sd = pSDManager->FindSensitiveDetector(sdName, false);
  if (!sd) {
    TG4SensitiveDetector* newSD = new TG4SensitiveDetector(sdName);        
    pSDManager->AddNewDetector(newSD);

    if (VerboseLevel() > 1) {
      G4cout << "Sensitive detector " << sdName << "  ID=" 
             << newSD->GetID() << " has been created." << G4endl;
    }
                 
    sd = newSD;  
  }        
  lv->SetSensitiveDetector(sd);             
  
  return ((TG4SensitiveDetector*)sd)->GetID();
}

//_____________________________________________________________________________
void  TG4SDConstruction::FillSDSelectionFromTGeo()
{
/// Retrieve the selection of sensitive volumes from TGeo.
/// The volumes are marked as sensitive when there is set the TGeoVolume option
/// to a given value (TG4SDConstruction::fgkDefaultSVLabel by default)

  if ( ! gGeoManager ) {
    TG4Globals::Exception(
      "TG4SDServices", "FillSDSelectionFromTGeo", 
      "TGeo manager not defined.");
  }
  
  TObjArray* volumes = gGeoManager->GetListOfVolumes();
  TIterator* it = volumes->MakeIterator();
  TGeoVolume* volume = 0;
  while ( ( volume = (static_cast<TGeoVolume*>(it->Next())) ) ) {
    if ( TString(volume->GetOption()) == TString(fSVLabel.data()) ) {
      G4cout << "Adding volume " << volume->GetName() << " in SD selection" 
             << G4endl; 
      fSelection.insert(volume->GetName());
    }  
  }

  if ( ! fSelection.size() ) {
    TString text = "No volumes with the option = \"";
    text += TString(fSVLabel.data());
    text += "\" were found in TGeo geometry.\n";
    text += "    The SD selection will not be applied.";
    TG4Globals::Warning(
      "TG4SDServices", "FillSDSelectionFromTGeo", text);
  }
}        

//
// public methods
//

//_____________________________________________________________________________
void TG4SDConstruction::Construct()
{ 
/// Create sensitive detectors and initialize the VMC application.
/// Sensitive detectors are set to all logical volumes

  if ( fSelectionFromTGeo ) FillSDSelectionFromTGeo();

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  
  for ( G4int i=0; i<G4int(lvStore->size()); i++ ) {
    G4LogicalVolume* lv = (*lvStore)[i];
    // Create SD if selection is empty; 
    // or if volume name is in selection if selection is defined
    if ( ! fSelection.size() ||
          fSelection.find(lv->GetName()) != fSelection.end() ) {
      G4int sdID = CreateSD(lv);
      TG4SDServices::Instance()->MapVolume(lv, sdID);
    }  
  }
  
  
  if ( fSelection.size() ) {
    // Set volume IDs to volumes which have not SD
    G4int counter = TG4SensitiveDetector::GetTotalNofSensitiveDetectors();
    for ( G4int i=0; i<G4int(lvStore->size()); i++ ) {
      G4LogicalVolume* lv = (*lvStore)[i];
      if ( ! lv->GetSensitiveDetector() ) 
        TG4SDServices::Instance()->MapVolume(lv, counter++);
    }
  }    
  
  TG4StateManager::Instance()->SetNewState(kInitGeometry);
  TVirtualMCApplication::Instance()->InitGeometry();
  TG4StateManager::Instance()->SetNewState(kNotInApplication);

  if (VerboseLevel() > 1) {
    TG4SDServices::Instance()->PrintVolNameToIdMap();
    TG4SDServices::Instance()->PrintVolIdToLVMap();
    if ( fSelection.size() ) {
      TG4SDServices::Instance()->PrintSensitiveVolumes();
    }  
  }  
}

//_____________________________________________________________________________
void TG4SDConstruction::AddSelection(const G4String& selection)
{
/// Add the selection in the set of volume names which will be
/// made sensitive.

  std::istringstream is(selection);  
  G4String token;
  while ( is >> token ) {
    if (VerboseLevel() > 1) {
      G4cout << "Adding volume " << token <<  " in SD selection." << G4cout;
    }  
    fSelection.insert(token);
  }  
}  
