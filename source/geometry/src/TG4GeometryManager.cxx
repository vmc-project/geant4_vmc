//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GeometryManager.cxx
/// \brief Implementation of the TG4GeometryManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4GeometryManager.h"
#include "TG4BiasingManager.h"
#include "TG4Field.h"
#include "TG4MagneticField.h"
#include "TG4FieldParameters.h"
#include "TG4G3ControlVector.h"
#include "TG4G3CutVector.h"
#include "TG4G3Units.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"
#include "TG4Limits.h"
#include "TG4MCGeometry.h"
#include "TG4Medium.h"
#include "TG4MediumMap.h"
#include "TG4ModelConfigurationManager.h"
#include "TG4OpGeometryManager.h"
#include "TG4RadiatorDescription.h"
#include "TG4RootDetectorConstruction.h"
#include "TG4SDManager.h"
#include "TG4StateManager.h"
#include "TG4VUserPostDetConstruction.h"
#include "TG4VUserRegionConstruction.h"

#include <G4FieldManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Material.hh>
#include <G4MonopoleFieldSetup.hh>
#include <G4PVPlacement.hh>
#include <G4ReflectionFactory.hh>
//#include <G4SystemOfUnits.hh>
#include <G4TransportationManager.hh>

#include <TGeoMCGeometry.h>
#include <TGeoManager.h>
#include <TGeoMedium.h>
#include <TGeoVolume.h>
#include <TList.h>
#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>

// Moved after ROOT includes to avoid warnings about shadowing variables
// from CLHEP units
#include <G4SystemOfUnits.hh>

#ifdef USE_G3TOG4
#include <G3MatTable.hh>
#include <G3MedTable.hh>
#include <G3SensVolVector.hh>
#include <G3VolTable.hh>
#include <G3toG4.hh>
#include <G3toG4BuildTree.hh>
#include <G3toG4MANY.hh>
#endif

#ifdef USE_VGM
#include <Geant4GM/volumes/Factory.h>
#include <RootGM/volumes/Factory.h>
#endif

TG4GeometryManager* TG4GeometryManager::fgInstance = 0;
const G4double TG4GeometryManager::fgDefaultLimitDensity = 0.001 * (g / cm3);
const G4double TG4GeometryManager::fgDefaultMaxStep = 10 * cm;

G4ThreadLocal std::vector<TG4Field*>* TG4GeometryManager::fgFields = 0;

//_____________________________________________________________________________
TG4GeometryManager::TG4GeometryManager(const TString& userGeometry)
  : TG4Verbose("geometryManager"),
    fMessenger(this),
    fGeometryServices(new TG4GeometryServices()),
    fMCGeometry(0),
    fRootDetectorConstruction(0),
    fOpManager(0),
    fFastModelsManager(0),
    fEmModelsManager(0),
    fBiasingManager(0),
    fUserGeometry(userGeometry),
    fFieldParameters(),
    fUserRegionConstruction(0),
    fUserPostDetConstruction(0),
    fIsLocalField(false),
    fIsZeroField(false),
    fIsUserMaxStep(false),
    fIsMaxStepInLowDensityMaterials(true),
    fLimitDensity(fgDefaultLimitDensity),
    fMaxStepInLowDensityMaterials(fgDefaultMaxStep)

{
  /// Standard constructor

  if (fgInstance) {
    TG4Globals::Exception("TG4GeometryManager",
      "TG4GeometryManager:", "Cannot create two instances of singleton.");
  }

  // Field parameters for global field
  fFieldParameters.push_back(new TG4FieldParameters());

  CreateMCGeometry();

  fOpManager = new TG4OpGeometryManager();

  fFastModelsManager = new TG4ModelConfigurationManager("fastSimulation");
  fEmModelsManager = new TG4ModelConfigurationManager("emModel");
  fBiasingManager = new TG4BiasingManager("biasing");

  fgInstance = this;
}

//_____________________________________________________________________________
TG4GeometryManager::~TG4GeometryManager()
{
  /// Destructor

  for (G4int i = 0; i < G4int(fFieldParameters.size()); ++i) {
    delete fFieldParameters[i];
  }

  delete fgFields;
  // magnetic field objects are deleted via G4 kernel

  delete fGeometryServices;
  delete fOpManager;
  delete fFastModelsManager;
  delete fEmModelsManager;
  delete fBiasingManager;

  fgInstance = 0;
  fgFields = 0;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4GeometryManager::CreateMCGeometry()
{
  /// Create MC geometry

  if (fUserGeometry == "VMCtoGeant4" || fUserGeometry == "Geant4" ||
      fUserGeometry == "RootToGeant4") {
    fMCGeometry = new TG4MCGeometry();
  }

  if (fUserGeometry == "VMCtoRoot" || fUserGeometry == "Root" ||
      fUserGeometry == "VMC+RootToGeant4") {
    if (!gGeoManager) new TGeoManager("TGeo", "Root geometry manager");
    fMCGeometry = new TGeoMCGeometry();
  }
}

//_____________________________________________________________________________
void TG4GeometryManager::ConstructG4GeometryViaVMC()
{
  /// Create G4 geometry objects according to the G3VolTable

#ifdef USE_G3TOG4
  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::ConstructG4GeometryViaVMC" << G4endl;

  // check if G4 tables were filled
  /*
    if ( ! TG4G3MCGeometry::Instance()->IsGeometryDefined() ) {
      TG4Globals::Exception(
        "TG4GeometryManager", "ConstructG4GeometryViaVMC",
        "Geometry was not defined via VMC.");
    }
  */
  // pass info about using G3toG4 to geometry services
  fGeometryServices->SetIsG3toG4(true);

  // set the first entry in the G3Vol table
  G4ggclos();
  G3VolTableEntry* first = G3Vol.GetFirstVTE();

  // transform MANY to Boolean solids
  G3toG4MANY(first);

  // create G4 geometry
  G3toG4BuildTree(first, 0);

  // fill medium map
  // FillMediumMapFromG3();

  // position the first entry with copyNo = 1
  // (in Geant3 the top volume cannot be positioned)
  //
  if (!fGeometryServices->GetWorld()) {
    G4VPhysicalVolume* world = new G4PVPlacement(
      0, G4ThreeVector(), first->GetName(), first->GetLV(), 0, false, 1);
    fGeometryServices->SetWorld(world);
  }

  // print G3 volume table statistics
  G3Vol.VTEStat();

#else
  TG4Globals::Exception("TG4GeometryManager", "ConstructG4GeometryViaVMC",
    "Geant4 VMC has been installed without G3toG4." + TG4Globals::Endl() +
      "Geometry construction via VMC is not supported.");
#endif
}

//_____________________________________________________________________________
void TG4GeometryManager::ConstructG4GeometryViaVGM()
{
  /// Convert Root geometry to G4 geometry objects
  /// using roottog4 convertor.

#ifdef USE_VGM
  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::ConstructG4GeometryViaVGM" << G4endl;

  // Check Root manager
  if (!gGeoManager) {
    TG4Globals::Exception("TG4GeometryManager", "ConstructG4GeometryViaVGM",
      "Geometry was not defined via Root.");
  }

  // Get and eventually also set the Root top volume
  TGeoVolume* topVolume = gGeoManager->GetTopVolume();
  if (!topVolume) {
    topVolume = (TGeoVolume*)gGeoManager->GetListOfVolumes()->First();
    if (!topVolume) {
      TG4Globals::Exception("TG4GeometryManager", "ConstructG4GeometryViaVGM",
        "Root top volume not found.");
    }
    gGeoManager->SetTopVolume(topVolume);
  }

  // Close Root geometry
  if (!gGeoManager->IsClosed()) gGeoManager->CloseGeometry();

  // Convert Root geometry to G4
  if (VerboseLevel() > 0)
    G4cout << "Converting Root geometry to Geant4 via VGM ... " << G4endl;

  // import Root geometry in VGM
  RootGM::Factory rootFactory;
  if (VerboseLevel() > 1) rootFactory.SetDebug(1);
  rootFactory.SetIgnore(true);
  rootFactory.Import(gGeoManager->GetTopNode());

  // export Root VGM geometry in Geant4
  Geant4GM::Factory g4Factory;
  if (VerboseLevel() > 1) g4Factory.SetDebug(1);
  rootFactory.Export(&g4Factory);

  G4VPhysicalVolume* g4World = g4Factory.World();
  fGeometryServices->SetWorld(g4World);

#else
  TG4Globals::Exception("TG4GeometryManager", "ConstructG4GeometryViaVGM",
    "Geant4 VMC has been installed without VGM." + TG4Globals::Endl() +
      "Root geometry conversion is not supported.");
#endif
}

//_____________________________________________________________________________
void TG4GeometryManager::ConstructG4Geometry()
{
  /// Construct Geant4 geometry depending on user geometry source

  if (VerboseLevel() > 1) {
    G4cout << "TG4GeometryManager::ConstructG4Geometry: "
           << "userGeometry=" << fUserGeometry << G4endl;
  }

  // VMC application construct geometry
  if (fUserGeometry == "VMCtoGeant4") {

    if (VerboseLevel() > 1)
      G4cout << "Running TVirtualMCApplication::ConstructGeometry" << G4endl;

    TG4StateManager::Instance()->SetNewState(kConstructGeometry);
    TVirtualMCApplication::Instance()->ConstructGeometry();
    TG4StateManager::Instance()->SetNewState(kMisalignGeometry);
    TVirtualMCApplication::Instance()->MisalignGeometry();
    TG4StateManager::Instance()->SetNewState(kNotInApplication);
  }

  // VMC application construct geometry
  if (fUserGeometry == "RootToGeant4" || fUserGeometry == "VMC+RootToGeant4") {
    if (VerboseLevel() > 1)
      G4cout << "Running TVirtualMCApplication::ConstructGeometry" << G4endl;

#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 22, 8)
    // Set Root default units to TGeo
    TGeoManager::LockDefaultUnits(false);
    TGeoManager::SetDefaultUnits(TGeoManager::kRootUnits);
    TGeoManager::LockDefaultUnits(true);
#endif

    TG4StateManager::Instance()->SetNewState(kConstructGeometry);
    TVirtualMCApplication::Instance()->ConstructGeometry();
    TG4StateManager::Instance()->SetNewState(kNotInApplication);

    // If Root geometry was not closed by user
    // we have to do it here
    if (!gGeoManager->IsClosed()) {
      TGeoVolume* top = (TGeoVolume*)gGeoManager->GetListOfVolumes()->First();
      gGeoManager->SetTopVolume(top);
      gGeoManager->CloseGeometry();
    }

    TG4StateManager::Instance()->SetNewState(kMisalignGeometry);
    TVirtualMCApplication::Instance()->MisalignGeometry();
    TG4StateManager::Instance()->SetNewState(kNotInApplication);
  }

  // Build G4 geometry
  if (fUserGeometry == "VMCtoGeant4") ConstructG4GeometryViaVMC();

  if (fUserGeometry == "RootToGeant4" || fUserGeometry == "VMC+RootToGeant4")
    ConstructG4GeometryViaVGM();

  // print G4 geometry statistics
  if (VerboseLevel() > 0) {
    G4cout << "G4 Stat: instantiated "
           << fGeometryServices->NofG4LogicalVolumes() << " logical volumes \n"
           << "                      "
           << fGeometryServices->NofG4PhysicalVolumes() << " physical volumes"
           << G4endl;
  }
}

//_____________________________________________________________________________
void TG4GeometryManager::FillMediumMapFromG3()
{
  /// Map G3 tracking medium IDs to volumes names.

#ifdef USE_G3TOG4
  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::FillMediumMapFromG3()" << G4endl;

  TG4MediumMap* mediumMap = fGeometryServices->GetMediumMap();

  // Create medium for each medium entry
  for (G4int i = 0; i < G4int(G3Med.GetSize()); i++) {
    G3MedTableEntry* mediumEntry = G3Med.GetMTE(i);
    G4int mediumID = mediumEntry->GetID();

    if (VerboseLevel() > 2) {
      G4cout << "Getting medium ID=" << mediumID << G4endl;
    }
    // Get medium from medium map
    TG4Medium* medium = mediumMap->GetMedium(mediumID);

    // Create a medium if it does not exist
    // (This should not happen, but let's check it anyway)
    if (!medium) {
      medium = mediumMap->AddMedium(mediumID, false);

      TString message = "Medium ";
      message += mediumID;
      message += " was not found in medium map. New medium will be created";
      TG4Globals::Warning("TG4GeometryManager", "FillMediumMapFromG3", message);
    }

    medium->SetLimits(mediumEntry->GetLimits());
    medium->SetMaterial(mediumEntry->GetMaterial());
  }

  if (VerboseLevel() > 2) G3Vol.PrintAll();

  // Map media to logical volumes
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];

    // Get medium ID from G3 tables
    G4String name = lv->GetName();
    G4String g3Name(name);
    // Filter out the reflected volume name extension
    // added by reflection factory
    G4String ext = G4ReflectionFactory::Instance()->GetVolumesNameExtension();
    if (name.find(ext)) g3Name = g3Name.substr(0, g3Name.find(ext));
    G4int mediumID = G3Vol.GetVTE(g3Name)->GetNmed();

    if (VerboseLevel() > 2)
      G4cout << "Mapping medium Id " << mediumID << " to LV " << name << G4endl;

    // Map medium to LV
    mediumMap->MapMedium(lv, mediumID);
  }

  // clear G3 tables
  G3Vol.Clear();
  G3SensVol.clear();
  G3Mat.Clear();
  G3Med.Clear();
#else
  TG4Globals::Exception("TG4GeometryManager", "FillMediumMapFromG3",
    "Geant4 VMC has been installed without G3toG4." + TG4Globals::Endl() +
      "Geometry construction via VMC is not supported.");
#endif
}

//_____________________________________________________________________________
void TG4GeometryManager::FillMediumMapFromG4()
{
  /// Map G4 materials in the medium map;
  /// the materialIndex is used to define medium ID.

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::FillMediumMapFromG4()" << G4endl;

  TG4MediumMap* mediumMap = fGeometryServices->GetMediumMap();

  // Create medium for each material
  const G4MaterialTable* materialTable = G4Material::GetMaterialTable();
  for (G4int i = 0; i < G4int(materialTable->size()); i++) {
    G4Material* material = (*materialTable)[i];

    if (VerboseLevel() > 2) {
      G4cout << "Adding medium name= " << material->GetName()
             << " Id=" << material->GetIndex() << G4endl;
    }
    TG4Medium* medium = mediumMap->AddMedium(material->GetIndex());
    medium->SetName(material->GetName());
    medium->SetMaterial(material);
  }

  // Map media to logical volumes
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    G4int mediumID = lv->GetMaterial()->GetIndex();

    if (VerboseLevel() > 2) {
      G4cout << "Mapping medium Id=" << mediumID << " to LV= " << lv->GetName()
             << G4endl;
    }
    mediumMap->MapMedium(lv, mediumID);
  }
}

//_____________________________________________________________________________
void TG4GeometryManager::FillMediumMapFromRoot()
{
  /// Map Root tracking media in the medium map

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::FillMediumMapFromRoot()" << G4endl;

  // fGeometryServices->PrintLogicalVolumeStore();

  TG4MediumMap* mediumMap = fGeometryServices->GetMediumMap();

  // Create TG4 medium for each TGeo madium
  TIter next(gGeoManager->GetListOfMedia());
  TGeoMedium* geoMedium;
  while ((geoMedium = (TGeoMedium*)next())) {
    Int_t mediumId = geoMedium->GetId();
    G4String mediumName = geoMedium->GetName();

    // Int_t isvol  = (Int_t) geoMedium->GetParam(0);
    Int_t ifield = (Int_t)geoMedium->GetParam(1);
    // Double_t fieldm = geoMedium->GetParam(2);
    // Double_t tmaxfd = geoMedium->GetParam(3);
    Double_t stemax = geoMedium->GetParam(4);
    // Double_t deemax = geoMedium->GetParam(5);
    // Double_t epsil  = geoMedium->GetParam(6);
    // Double_t stmin  = geoMedium->GetParam(7);

    // Only stemax parameter is passed to G4 if it is positive
    G4UserLimits* limits = 0;
    if (stemax > 0) {
      limits = new G4UserLimits();
      limits->SetMaxAllowedStep(stemax * cm);
    }

    if (VerboseLevel() > 2) {
      G4cout << "Adding medium Id=" << mediumId << " name=" << mediumName
             << " limits=" << limits << G4endl;
    }
    TG4Medium* medium = mediumMap->AddMedium(mediumId);
    medium->SetName(mediumName);
    medium->SetLimits(limits);
    medium->SetIfield(ifield);

    G4String matName = geoMedium->GetMaterial()->GetName();
    G4Material* material = G4Material::GetMaterial(matName);
    if (!material) {
      TG4Globals::Exception("TG4GeometryManager", "FillMediumMapFromRoot",
        "Material " + TString(matName) + " not found.");
    }
    medium->SetMaterial(material);
  }

  // Map media to logical volumes
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];

    TGeoVolume* geoVolume = nullptr;

    if (fRootDetectorConstruction == nullptr) {
      G4String volName = lv->GetName();

      // Filter out the reflected volumes name extension
      // added by reflection factory
      G4String ext = G4ReflectionFactory::Instance()->GetVolumesNameExtension();
      if (volName.find(ext)) volName = volName.substr(0, volName.find(ext));

      geoVolume = gGeoManager->GetVolume(volName.data());
    }
    else {
      geoVolume = fRootDetectorConstruction->GetVolume(lv);
    }

    if (!geoVolume) {
      TG4Globals::Exception("TG4GeometryManager", "FillMediumMapFromRoot",
        "Root volume " + TString(lv->GetName()) + " not found");
    }

    // skip assemblies
    if (geoVolume && geoVolume->IsAssembly()) continue;

    if (geoVolume && !geoVolume->GetMedium()) {
      TG4Globals::Exception("TG4GeometryManager", "FillMediumMapFromRoot",
        "Root volume " + TString(lv->GetName()) + " has not medium defined.");
    }

    G4int mediumID = geoVolume->GetMedium()->GetId();

    if (VerboseLevel() > 2) {
      G4cout << "Mapping medium Id=" << mediumID << " to LV=" << lv->GetName()
             << G4endl;
    }
    mediumMap->MapMedium(lv, mediumID);
  }
}

//_____________________________________________________________________________
void TG4GeometryManager::FillMediumMap()
{
  /// Fill medium map depending on user geometry source

  if (fUserGeometry == "VMCtoGeant4") FillMediumMapFromG3();

  if (fUserGeometry == "VMCtoRoot" || fUserGeometry == "Root" ||
      fUserGeometry == "RootToGeant4" || fUserGeometry == "VMC+RootToGeant4") {
    FillMediumMapFromRoot();
  }

  if (fUserGeometry == "Geant4") FillMediumMapFromG4();
}

//_____________________________________________________________________________
void TG4GeometryManager::CreateField(TVirtualMagField* magField,
  TG4FieldParameters* fieldParameters, G4LogicalVolume* lv)
{
  /// Create magnetic, electromagnetic or gravity field

  TG4Field* tg4Field = new TG4Field(*fieldParameters, magField, lv);

  if (VerboseLevel() > 0) {
    G4String fieldType =
      TG4FieldParameters::FieldTypeName(fieldParameters->GetFieldType());
    G4bool isCachedMagneticField = (fieldParameters->GetConstDistance() > 0.);
    if (!lv) {
      fieldType = "Global";
    }
    else {
      fieldType = "Local (in ";
      fieldType.append(lv->GetName());
      fieldType.append(")");
    }
    if (isCachedMagneticField) {
      fieldType.append(" cached");
    }

    G4cout << fieldType << " field created with stepper ";
    G4cout << TG4FieldParameters::StepperTypeName(
                fieldParameters->GetStepperType())
           << G4endl;
  }

  // create magnetic field vector
  if (!fgFields) {
    fgFields = new std::vector<TG4Field*>();
  }

  fgFields->push_back(tg4Field);
}

//_____________________________________________________________________________
void TG4GeometryManager::ConstructGlobalField()
{
  /// Construct Geant4 global magnetic field from the field set to gMC

  // Create global magnetic field
  if (gMC->GetMagField()) {
    CreateField(gMC->GetMagField(), fFieldParameters[0], 0);

    // create monopole field
    if (fFieldParameters[0]->GetIsMonopole()) {
      G4cout << "Create G4MonopoleFieldSetup" << G4endl;
      G4MonopoleFieldSetup* monFieldSetup =
        G4MonopoleFieldSetup::GetMonopoleFieldSetup();
      TG4Field* tg4Field = (*fgFields)[0];
      G4Field* field = tg4Field->GetG4Field();
      G4MagneticField* magField = dynamic_cast<G4MagneticField*>(field);
      if (!magField) {
        // add warning
        G4cerr << "Wrong field type. Only magnetic field is supported in "
                  "G4MonopoleFieldSetup."
               << G4endl;
      }
      else {
        monFieldSetup->SetMagneticField(magField);
        monFieldSetup->SetDefaultEquation(tg4Field->GetEquation());
        monFieldSetup->SetDefaultStepper(tg4Field->GetStepper());
        monFieldSetup->InitialiseAll();
      }
    }

    if (fIsZeroField) {
      ConstructZeroFields();
    }
  }
}

//_____________________________________________________________________________
void TG4GeometryManager::ConstructZeroFields()
{
  /// Loop over all logical volumes and set zero magnetic if the volume
  /// is associated with a tracking medium with ifield value = 0.
  /// This function is invoked only when a global magnetic field is defined.

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::ConstructZeroFields()" << G4endl;

  G4bool forceToAllDaughters = false;

  // Set a dummy field manager to all LV first in order to avoid propagation
  // of zero field to volume daughters
  G4FieldManager* dummyFieldManager = new G4FieldManager();
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    lv->SetFieldManager(dummyFieldManager, forceToAllDaughters);
  }

  // Set zero field manager to volumes associated with a tracking medium
  // with ifield value = 0.
  G4FieldManager* fieldManager = 0;
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {

    G4LogicalVolume* lv = (*lvStore)[i];

    // skip volume without medium
    TG4Medium* medium =
      TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(lv, false);
    if (!medium) continue;

    // Skip volumes with ifield != 0
    if (medium->GetIfield() != 0) {
      if (VerboseLevel() > 2) {
        G4cout << "Global field in logical volume: " << lv->GetName() << G4endl;
      }
      continue;
    }

    // create field manager if it does not exist yet
    if (!fieldManager) {
      fieldManager = new G4FieldManager();
      // CHECK if we need to delete it
      fieldManager->SetDetectorField(0);
      fieldManager->CreateChordFinder(0);
    }
    lv->SetFieldManager(fieldManager, forceToAllDaughters);

    if (VerboseLevel() > 1) {
      G4cout << "Zero magnetic field set to logical volume: " << lv->GetName()
             << G4endl;
    }
  }

  // Remove the  dummy field manager to all LV without zero field
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];

    if (lv->GetFieldManager() == dummyFieldManager) {
      lv->SetFieldManager(0, forceToAllDaughters);
    }
  }

  // Delete the dummy field manager
  delete dummyFieldManager;
}

//_____________________________________________________________________________
TG4FieldParameters* TG4GeometryManager::GetOrCreateFieldParameters(
  const G4String& volumeName)
{
  /// Get field parameters with the given volumeName or create them if they
  /// do not exist yet

  // Get user field parameters
  TG4FieldParameters* fieldParameters = 0;
  for (G4int i = 0; i < G4int(fFieldParameters.size()); ++i) {
    if (fFieldParameters[i]->GetVolumeName() == volumeName) {
      return fFieldParameters[i];
    }
  }

  // Create field parameters if not yet defined
  fieldParameters = new TG4FieldParameters(volumeName);
  fFieldParameters.push_back(fieldParameters);
  return fieldParameters;
}

//_____________________________________________________________________________
void TG4GeometryManager::ConstructLocalFields()
{
  /// Construct Geant4 local magnetic field from Root geometry.

  // Supported only for geomRoot and geomRootToGeant4.
  if ((fUserGeometry != "Root") && (fUserGeometry != "RootToGeant4")) return;

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::ConstructLocalFields()" << G4endl;

  TIter next(gGeoManager->GetListOfVolumes());
  TGeoVolume* geoVolume;
  while ((geoVolume = (TGeoVolume*)next())) {

    if (!geoVolume->GetField()) continue;

    // Get field
    TVirtualMagField* magField =
      dynamic_cast<TVirtualMagField*>(geoVolume->GetField());
    if (!magField) {
      TString message = geoVolume->GetName();
      message += ": uknown field type will be ignored.";
      TG4Globals::Warning("TG4GeometryManager", "ConstructLocalFields",
        "No magnetic field is defined.");
      continue;
    }

    // Volume name
    G4String volumeName = geoVolume->GetName();

    // Get Geant4 volume
    G4LogicalVolume* lv =
      TG4GeometryServices::Instance()->FindLogicalVolume(volumeName);
    if (!lv) {
      TString message = geoVolume->GetName();
      message += " volume not found in Geant4 geometry.";
      TG4Globals::Warning("TG4GeometryManager", "ConstructLocalFields",
        "No magnetic field is defined.");
      continue;
    }

    // Get or create user field parameters
    TG4FieldParameters* fieldParameters =
      GetOrCreateFieldParameters(volumeName);

    // Create magnetic field
    CreateField(magField, fieldParameters, lv);
  }
}

//
// public methods
//

//_____________________________________________________________________________
TVirtualMCGeometry* TG4GeometryManager::GetMCGeometry() const
{
  /// Return the instance of MC geometry;
  /// give exception if no object is instantiated

  if (!fMCGeometry) {
    TG4Globals::Exception(
      "TG4GeometryManager", "GetMCGeometry", "No MC geometry defined.");
  }

  return fMCGeometry;
}

//_____________________________________________________________________________
void TG4GeometryManager::ConstructGeometry()
{
  /// Construct Geant4 geometry depending on user geometry source

  // Construct G4 geometry
  ConstructG4Geometry();

  // Fill medium map
  FillMediumMap();

  // VMC application construct geometry for optical processes
  TG4StateManager::Instance()->SetNewState(kConstructOpGeometry);
  TVirtualMCApplication::Instance()->ConstructOpGeometry();
  TG4StateManager::Instance()->SetNewState(kNotInApplication);

  // Construct user regions
  if (fUserRegionConstruction) fUserRegionConstruction->Construct();
}

#include "TG4SDManager.h"

//_____________________________________________________________________________
void TG4GeometryManager::ConstructSDandField()
{
  /// Construct Geant4 geometry depending on user geometry source

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::ConstructSDandField() " << G4endl;

  // Call user class for geometry customization
  if (fUserPostDetConstruction) fUserPostDetConstruction->Construct();

  // Construct regions with fast simulation and EM models
  fFastModelsManager->CreateRegions();
  fEmModelsManager->CreateRegions();

  // Construct biasing operator
  fBiasingManager->CreateBiasingOperator();

  // Initialize SD manager (create SDs)
  TG4SDManager::Instance()->Initialize();

  // Create global field
  ConstructGlobalField();

  if (fIsLocalField) {
    ConstructLocalFields();
  }
}

//_____________________________________________________________________________
void TG4GeometryManager::FinishGeometry()
{
  /// Finish geometry construction after G4 run initialization

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::FinishGeometry" << G4endl;

  // Create magnetic field
  // ConstructField();

  // Fill medium map if not yet done
  if (fGeometryServices->GetMediumMap()->GetNofMedia() == 0) FillMediumMap();

  // Set world to geometry services
  fGeometryServices->SetWorld(
    G4TransportationManager::GetTransportationManager()
      ->GetNavigatorForTracking()
      ->GetWorldVolume());

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::FinishGeometry done" << G4endl;
}

//_____________________________________________________________________________
void TG4GeometryManager::UpdateField()
{
  /// Update magnetic field.
  /// This function must be called if the field parameters were changed
  /// in other than PreInit> phase.

  if (!fgFields) {
    TG4Globals::Warning(
      "TG4GeometryManager", "UpdateField", "No magnetic field is defined.");
    return;
  }

  if (VerboseLevel() > 1) G4cout << "TG4GeometryManager::UpdateField" << G4endl;

  // Only the parameters defined in TG4Magnetic field can be updated when
  // field already exists, so we can safely call the base class non virtual
  // method
  for (G4int i = 0; i < G4int(fgFields->size()); ++i) {
    fgFields->at(i)->Update(*fFieldParameters[i]);
  }
}

//_____________________________________________________________________________
void TG4GeometryManager::CreateFieldParameters(const G4String& fieldVolName)
{
  /// Create local magnetic field parameters which can be then configured
  /// by the user via UI commands.
  /// The parameters are used in geometry only if a local magnetic field is
  /// associated with the volumes with the given name

  fFieldParameters.push_back(new TG4FieldParameters(fieldVolName));
}

//_____________________________________________________________________________
TG4RadiatorDescription* TG4GeometryManager::CreateRadiator(
  const G4String& volName)
{
  /// Create radiator description with the given volume name

  TG4RadiatorDescription* radiatorDescription =
    new TG4RadiatorDescription(volName);
  fRadiators.push_back(radiatorDescription);

  return radiatorDescription;
}

//_____________________________________________________________________________
void TG4GeometryManager::SetUserLimits(
  const TG4G3CutVector& cuts, const TG4G3ControlVector& controls) const
{
  /// Set user limits defined in G3MedTable for all logical volumes.

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::SetUserLimits" << G4endl;

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    TG4Medium* medium = fGeometryServices->GetMediumMap()->GetMedium(lv, false);

    if (!medium) continue;

    // get limits if already exist
    TG4Limits* tg4Limits = 0;
    G4UserLimits* limits = medium->GetLimits();
    tg4Limits = fGeometryServices->GetLimits(limits, cuts, controls);

    // get tracking medium name
    G4String name = medium->GetName();

    if (tg4Limits) {
      tg4Limits->SetName(name);
    }
    else {
      // Check if the step below is needed
      tg4Limits = fGeometryServices->FindLimits2(name, true);
      if (!tg4Limits) {
        tg4Limits = new TG4Limits(name, cuts, controls);
      }
    }

    // set new limits back to medium
    medium->SetLimits(tg4Limits);

    // inactivate max step defined by user
    // if its activation was not asked explicitely
    if (!fIsUserMaxStep) tg4Limits->SetMaxAllowedStep(DBL_MAX);

    // limit max step for low density materials (< AIR)
    if (fIsMaxStepInLowDensityMaterials &&
        lv->GetMaterial()->GetDensity() < fLimitDensity)
      tg4Limits->SetMaxAllowedStep(fMaxStepInLowDensityMaterials);

    // set max step the default value
    tg4Limits->SetDefaultMaxAllowedStep();

    // update controls in limits according to the setup
    // in the passed vector
    tg4Limits->Update(controls);

    // set limits to logical volume
    lv->SetUserLimits(tg4Limits);
  }

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::SetUserLimits done" << G4endl;
}

//_____________________________________________________________________________
void TG4GeometryManager::SetIsLocalField(G4bool isLocalField)
{
  /// (In)Activate use of local magnetic field(s)

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::SetIsLocalField: " << std::boolalpha
           << isLocalField << G4endl;

  fIsLocalField = isLocalField;
}

//_____________________________________________________________________________
void TG4GeometryManager::SetIsZeroField(G4bool isZeroField)
{
  /// (In)Activate propagating 'ifield = 0' parameter defined in tracking media

  if (VerboseLevel() > 1)
    G4cout << "TG4GeometryManager::SetIsZeroField: " << std::boolalpha
           << isZeroField << G4endl;

  fIsZeroField = isZeroField;
}

//_____________________________________________________________________________
void TG4GeometryManager::SetIsUserMaxStep(G4bool isUserMaxStep)
{
  /// (In)Activate the max step defined by user in tracking media

  if (VerboseLevel() > 0)
    G4cout << "TG4GeometryManager::SetIsUserMaxStep: " << std::boolalpha
           << isUserMaxStep << G4endl;

  fIsUserMaxStep = isUserMaxStep;
}

//_____________________________________________________________________________
void TG4GeometryManager::SetIsMaxStepInLowDensityMaterials(G4bool isMaxStep)
{
  /// (In)Activate the max step defined in low density materials

  if (VerboseLevel() > 0)
    G4cout << "TG4GeometryManager::SetIsMaxStepInLowDensityMaterials: "
           << std::boolalpha << isMaxStep << G4endl;

  fIsMaxStepInLowDensityMaterials = isMaxStep;
}

//_____________________________________________________________________________
void TG4GeometryManager::SetUserRegionConstruction(
  TG4VUserRegionConstruction* userRegionConstruction)
{
  /// Set user region construction

  fUserRegionConstruction = userRegionConstruction;
}

//_____________________________________________________________________________
void TG4GeometryManager::SetUserPostDetConstruction(
  TG4VUserPostDetConstruction* userPostDetConstruction)
{
  /// Set user region construction

  fUserPostDetConstruction = userPostDetConstruction;
}

//_____________________________________________________________________________
void TG4GeometryManager::SetUserEquationOfMotion(
  G4EquationOfMotion* equation, G4String volumeName)
{
  if (!volumeName.size()) {
    // global field
    fFieldParameters[0]->SetUserEquationOfMotion(equation);
  }
  else {
    // local field
    // Get or create user field parameters
    TG4FieldParameters* fieldParameters =
      GetOrCreateFieldParameters(volumeName);
    fieldParameters->SetUserEquationOfMotion(equation);
  }
}

//_____________________________________________________________________________
void TG4GeometryManager::SetUserStepper(
  G4MagIntegratorStepper* stepper, G4String volumeName)
{
  if (!volumeName.size()) {
    // global field
    fFieldParameters[0]->SetUserStepper(stepper);
  }
  else {
    // local field
    // Get or create user field parameters
    TG4FieldParameters* fieldParameters =
      GetOrCreateFieldParameters(volumeName);
    fieldParameters->SetUserStepper(stepper);
  }
}

//_____________________________________________________________________________
void TG4GeometryManager::PrintFieldStatistics() const
{
  /// Print field statistics.
  /// Currently only the cached field prints the caching statistics.
  if (VerboseLevel() > 0 && fgFields) {
    for (G4int i = 0; i < G4int(fgFields->size()); ++i) {
      auto f = fgFields->at(i); // this is a TG4Field
      // we need to get the containing TG4MagneticField in order to print statistics
      auto mgfield = dynamic_cast<TG4MagneticField*>(f->GetG4Field());
      if (mgfield) {
        mgfield->PrintStatistics();
      }
    }
  }
}
