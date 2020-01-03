//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SDConstruction.cxx
/// \brief Implementation of the TG4SDConstruction class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SDConstruction.h"
#include "TG4GeometryServices.h"
#include "TG4GflashSensitiveDetector.h"
#include "TG4SDServices.h"
#include "TG4SensitiveDetector.h"
#include "TG4StateManager.h"

#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SDManager.hh>
#include <G4Threading.hh>

#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMCSensitiveDetector.h>

#include <TVirtualMC.h>

const G4String TG4SDConstruction::fgkDefaultSVLabel = "SV";

//_____________________________________________________________________________
TG4SDConstruction::TG4SDConstruction()
  : TG4Verbose("SDConstruction"),
    fMessenger(this),
    fExclusiveSDScoring(false),
    fSelectionFromTGeo(false),
    fSVLabel(fgkDefaultSVLabel),
    fSelection(),
    fIsGflash(false)
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
void TG4SDConstruction::CreateSD(
  G4LogicalVolume* lv, TVirtualMCSensitiveDetector* userSD) const
{
  /// Create/retrieve a sensitive detector for the given logical volume.

  if (VerboseLevel() > 2) {
    G4cout << "TG4SDConstruction::CreateSD:  lv=" << lv->GetName()
      << " userSD=" << userSD << G4endl;
  }

  TG4GeometryServices* geometryServices = TG4GeometryServices::Instance();
  G4SDManager* pSDManager = G4SDManager::GetSDMpointer();

  G4String sdName;
  if (userSD) {
    sdName = userSD->GetName();
  }
  else {
    G4String lvName = lv->GetName();
    sdName = "/" + lvName;
    // cut copy number from sdName
    sdName = geometryServices->UserVolumeName(sdName);
  }

  // create/retrieve the sensitive detector
  G4VSensitiveDetector* sd = 0;
  sd = pSDManager->FindSensitiveDetector(sdName, false);
  if (!sd) {

    G4int mediumId = TG4GeometryServices::Instance()->GetMediumId(lv);

    TG4SensitiveDetector* newSD = 0;
    if (fIsGflash) {
      newSD = new TG4GflashSensitiveDetector(sdName, mediumId);
      if (VerboseLevel() > 2) {
        G4cout << "Created TG4GflashSensitiveDetector with sdName=" << sdName
          << " mediumId=" << mediumId << G4endl;
      }
    }
    else if (userSD) {
      newSD = new TG4SensitiveDetector(userSD, mediumId, fExclusiveSDScoring);
      if (VerboseLevel() > 2) {
        G4cout << "Created TG4SensitiveDetector with userSD=" << userSD << " mediumId="
          << mediumId << " exclusiveSoring=" << fExclusiveSDScoring << G4endl;
      }
    }
    else {
      newSD = new TG4SensitiveDetector(sdName, mediumId);
      if (VerboseLevel() > 2) {
        G4cout << "Created TG4SensitiveDetector with sdName=" << sdName << " mediumId="
          << mediumId << G4endl;
      }
    }
    pSDManager->AddNewDetector(newSD);
    if (VerboseLevel() > 1) {
      G4cout << "Sensitive detector " << sdName << "  ID=" << newSD->GetID()
        << "  medium ID=" << newSD->GetMediumID() << " has been created." << G4endl;
    } else {
      if (VerboseLevel() > 2) {
        G4cout << "Sensitive detector " << sdName << ", " << sd << " already exists."
          << G4endl;
      }
    }
    sd = newSD;
  }
  lv->SetSensitiveDetector(sd);
}

//_____________________________________________________________________________
void TG4SDConstruction::FillSDSelectionFromTGeo()
{
  /// Retrieve the selection of sensitive volumes from TGeo.
  /// The volumes are marked as sensitive when there is set the TGeoVolume
  /// option to a given value (TG4SDConstruction::fgkDefaultSVLabel by default)

  if (!gGeoManager) {
    TG4Globals::Exception(
      "TG4SDServices", "FillSDSelectionFromTGeo", "TGeo manager not defined.");
  }

  TObjArray* volumes = gGeoManager->GetListOfVolumes();
  TIterator* it = volumes->MakeIterator();
  TGeoVolume* volume = 0;
  while ((volume = (static_cast<TGeoVolume*>(it->Next())))) {
    if (TString(volume->GetOption()) == TString(fSVLabel.data())) {
      G4cout << "Adding volume " << volume->GetName() << " in SD selection"
             << G4endl;
      fSelection.insert(volume->GetName());
    }
  }

  if (!fSelection.size()) {
    TString text = "No volumes with the option = \"";
    text += TString(fSVLabel.data());
    text += "\" were found in TGeo geometry.\n";
    text += "    The SD selection will not be applied.";
    TG4Globals::Warning("TG4SDServices", "FillSDSelectionFromTGeo", text);
  }
}

//_____________________________________________________________________________
void TG4SDConstruction::MapVolumesToInstanceIds()
{
  /// Define VMC volume Ids when new sensitive detectors framework is used.
  /// The volume Ids correspond to the Geant4 logical volume instance number.
  ///
  /// A special care is needed whin combining the old way of geometry definition
  /// with the new sensitive detectors framework.  Note that in this case, if
  /// geometry is defined via the VMC functions like gsposp, then the "same"
  /// volume in the context of Geant3, which is represented by more than one
  /// logical volumes in Geant4, will have several volume ids.

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];

    if (VerboseLevel() > 1) {
      G4cout << "Setting volId as instance Id "
             << lv->GetInstanceID() + TG4SDServices::GetFirstVolumeId()
             << " to " << lv->GetName() << G4endl;
    }

    TG4SDServices::Instance()->MapVolume(
      lv, lv->GetInstanceID() + TG4SDServices::GetFirstVolumeId(), false);
  }
}

//_____________________________________________________________________________
void TG4SDConstruction::MapVolumesToSDIds()
{
  /// Define VMC volume Ids if new sensitive detectors framework is not used,
  /// The volume ID is defined via sensitive detector Id.
  /// The sensitive detector is associated with maximum one logical volume,
  /// that's why it can hold volume and medium Id.

  // Set volume IDs to volumes which have not SD
  G4int counter = TG4SensitiveDetector::GetTotalNofSensitiveDetectors();

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];

    G4int id = 0;
    if (lv->GetSensitiveDetector()) {
      id =
        static_cast<TG4SensitiveDetector*>(lv->GetSensitiveDetector())->GetID();
    }
    else {
      id = counter++;
    }

    if (VerboseLevel() > 1) {
      G4cout << "Setting volId as SD id " << id << " to " << lv->GetName()
             << G4endl;
    }

    TG4SDServices::Instance()->MapVolume(lv, id, true);
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

  if (VerboseLevel() > 1) G4cout << "TG4SDConstruction::Construct" << G4endl;

  G4bool isMaster = !G4Threading::IsWorkerThread();

  // Get volumes selected as SD in TGeo geometry
  if (fSelectionFromTGeo && isMaster) FillSDSelectionFromTGeo();

  // Construct user SDs
  TG4StateManager::Instance()->SetNewState(kConstructSD);
  TVirtualMCApplication::Instance()->ConstructSensitiveDetectors();
  TG4StateManager::Instance()->SetNewState(kNotInApplication);

  G4bool isUserSD = false;
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];

    // Check if a user SD is defined
    TVirtualMCSensitiveDetector* userSD =
      TG4SDServices::Instance()->GetUserSD(lv->GetName(), false);

    // Create SD calling user sensitive detector

    if (userSD) {
      CreateSD(lv, userSD);
      // if ( isMaster ) TG4SDServices::Instance()->MapVolume(lv, sdID);
      isUserSD = true;
    }
    else {
      // Create SD calling MCApplication::Stepping
      // if exclusive scoring via user sensitive detectors is not activated and
      // if selection is empty or if selection is defined and the volume name is
      // in selection
      if ((!fExclusiveSDScoring) &&
          (!fSelection.size() ||
            fSelection.find(lv->GetName()) != fSelection.end())) {

        CreateSD(lv, 0);
      }
    }
  }

  // Define volume Ids if VMC SD is not defined for all volumes
  // (either due to user defined SDs or user selection of sensitive volumes)
  if (isMaster) {
    if (isUserSD) {
      MapVolumesToInstanceIds();
    }
    else {
      MapVolumesToSDIds();
    }
  }

  // Initialize user geometry
  TG4StateManager::Instance()->SetNewState(kInitGeometry);
  TVirtualMCApplication::Instance()->InitGeometry();
  TG4StateManager::Instance()->SetNewState(kNotInApplication);

  // Initialize user SDs
  if (TG4SDServices::Instance()->GetUserSDs()) {
    for (auto& userSD : (*TG4SDServices::Instance()->GetUserSDs())) {
      userSD->Initialize();
    }
  }

  if (VerboseLevel() > 1) {
    TG4SDServices::Instance()->PrintVolNameToIdMap();
    TG4SDServices::Instance()->PrintVolIdToLVMap();
    if (fSelection.size()) {
      TG4SDServices::Instance()->PrintSensitiveVolumes();
    }
    if (TG4SDServices::Instance()->GetUserSDs()) {
      TG4SDServices::Instance()->PrintUserSensitiveDetectors();
    }
  }

  if (VerboseLevel() > 1)
    G4cout << "TG4SDConstruction::Construct done" << G4endl;
}

//_____________________________________________________________________________
void TG4SDConstruction::AddSelection(const G4String& selection)
{
  /// Add the selection in the set of volume names which will be
  /// made sensitive.

  std::istringstream is(selection);
  G4String token;
  while (is >> token) {
    if (VerboseLevel() > 1) {
      G4cout << "Adding volume " << token << " in SD selection." << G4endl;
    }
    fSelection.insert(token);
  }
}
