// $Id: TG4GeometryManager.cxx,v 1.25 2007/05/24 09:05:24 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: geometry
// Class TG4GeometryManager
// ------------------------
// See the class description in the header file.

#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4MCGeometry.h"
#include "TG4OpGeometryManager.h"
#include "TG4StateManager.h"
#include "TG4MediumMap.h"
#include "TG4Medium.h"
#include "TG4Limits.h"
#include "TG4MagneticField.h"
#include "TG4UniformMagneticField.h"
#include "TG4G3Units.h"
#include "TG4G3CutVector.h"
#include "TG4G3ControlVector.h"
#include "TG4Globals.h"

#include <G3toG4.hh> 
#include <G3toG4MANY.hh>
#include <G3toG4BuildTree.hh>
#include <G3MatTable.hh>
#include <G3MedTable.hh>
#include <G3VolTable.hh>
#include <G3SensVolVector.hh>

#include <G4LogicalVolumeStore.hh>
#include <G4ReflectionFactory.hh>
#include <G4Material.hh>
#include <G4TransportationManager.hh>
#include <G4PVPlacement.hh>

#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TGeoMedium.h>
#include <TGeoMCGeometry.h>
#include <TVirtualMCApplication.h>
#include <TList.h>

#ifdef USE_VGM
#include <Geant4GM/volumes/Factory.h>
#include <RootGM/volumes/Factory.h>
#endif

TG4GeometryManager* TG4GeometryManager::fgInstance = 0;
const G4double      TG4GeometryManager::fgLimitDensity = 0.001*(g/cm3);
const G4double      TG4GeometryManager::fgMaxStep = 10*cm;

//_____________________________________________________________________________
TG4GeometryManager::TG4GeometryManager(const TString& userGeometry) 
  : TG4Verbose("geometryManager"),
    fMessenger(this),
    fGeometryServices(new TG4GeometryServices()),
    fMCGeometry(0),
    fOpManager(0),
    fUserGeometry(userGeometry),
    fMagneticFieldType(kMCApplicationField), 
    fMagneticField(0) 
{
/// Standard constructor

  if ( fgInstance ) {
    TG4Globals::Exception(
      "TG4GeometryManager", "TG4GeometryManager:",
      "Cannot create two instances of singleton.");
  }
  
  CreateMCGeometry();

  fOpManager = new TG4OpGeometryManager();
  
  fgInstance = this;
}

//_____________________________________________________________________________
TG4GeometryManager::~TG4GeometryManager() 
{
/// Destructor

  delete fMagneticField;
}

//
// private methods
//
 
//_____________________________________________________________________________
void TG4GeometryManager::CreateMCGeometry()
{
  if ( fUserGeometry == "VMCtoGeant4" || 
       fUserGeometry == "Geant4"      || 
       fUserGeometry == "RootToGeant4" ) {
    fMCGeometry = new TG4MCGeometry();
  }  
    
  if ( fUserGeometry == "VMCtoRoot" || 
       fUserGeometry == "Root") {
    if ( !gGeoManager) new TGeoManager("TGeo", "Root geometry manager");    
    fMCGeometry = new TGeoMCGeometry();
  }  
}    


//_____________________________________________________________________________
void TG4GeometryManager::ConstructG4GeometryViaVMC()
{
/// Create G4 geometry objects according to the G3VolTable 

  if ( VerboseLevel() > 1 ) 
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
  G3toG4BuildTree(first,0);  
  
  // fill medium map
  // FillMediumMapFromG3();

  // position the first entry with copyNo = 1
  // (in Geant3 the top volume cannot be positioned)
  // 
  if (!fGeometryServices->GetWorld()) {
    G4VPhysicalVolume* world
       = new G4PVPlacement(0, G4ThreeVector(), first->GetName(), 
                           first->GetLV(), 0, false, 1);
    fGeometryServices->SetWorld(world);                            
  }

  // print G3 volume table statistics
  G3Vol.VTEStat();
}

//_____________________________________________________________________________
void TG4GeometryManager::ConstructG4GeometryViaVGM()
{
/// Convert Root geometry to G4 geometry objects
/// using roottog4 convertor.

#ifdef USE_VGM
  if ( VerboseLevel() > 1 ) 
    G4cout << "TG4GeometryManager::ConstructG4GeometryViaVGM" << G4endl;

 // Check Root manager
  if (!gGeoManager) {
    TG4Globals::Exception(
      "TG4GeometryManager", "ConstructG4GeometryViaVGM",
      "Geometry was not defined via Root.");          
  }    

  // Get and eventually also set the Root top volume 
  TGeoVolume* topVolume = gGeoManager->GetTopVolume();
  if ( ! topVolume ) {
    topVolume = (TGeoVolume*)gGeoManager->GetListOfVolumes()->First();
    if ( ! topVolume ) {
      TG4Globals::Exception(
        "TG4GeometryManager", "ConstructG4GeometryViaVGM",
        "Root top volume not found.");        
    }    
    gGeoManager->SetTopVolume(topVolume);
  }  

  // Close Root geometry
  if (!gGeoManager->IsClosed()) gGeoManager->CloseGeometry();  

  // Convert Root geometry to G4
  if (VerboseLevel()>0)
    G4cout << "Converting Root geometry to Geant4 via VGM ... " << G4endl;
  
  // import Root geometry in VGM
  RootGM::Factory rootFactory;
  if ( VerboseLevel() > 1 ) rootFactory.SetDebug(1);
  rootFactory.Import(gGeoManager->GetTopNode());
    
  // export Root VGM geometry in Geant4
  Geant4GM::Factory g4Factory;
  if ( VerboseLevel() > 1 ) g4Factory.SetDebug(1);
  rootFactory.Export(&g4Factory);
    
  G4VPhysicalVolume* g4World = g4Factory.World();
  fGeometryServices->SetWorld(g4World);
    
#else
  TG4Globals::Exception(
    "TG4GeometryManager", "ConstructG4GeometryViaVGM",
    "Geant4 VMC has been installed without VGM." + TG4Globals::Endl() +
    "Root geometry conversion is not supported.");
#endif
}                   

//_____________________________________________________________________________
void TG4GeometryManager::ConstructG4Geometry()
{
/// Construct Geant4 geometry depending on user geometry source

  if ( VerboseLevel() > 1 ) {
    G4cout << "TG4GeometryManager::ConstructG4Geometry: " 
           << "userGeometry=" << fUserGeometry << G4endl;
  }           
 

  // VMC application construct geometry 
  if ( fUserGeometry == "VMCtoGeant4" ) {

    if ( VerboseLevel() > 1 ) 
      G4cout << "Running TVirtualMCApplication::ConstructGeometry" << G4endl;

    TG4StateManager::Instance()->SetNewState(kConstructGeometry);
    TVirtualMCApplication::Instance()->ConstructGeometry(); 
    TG4StateManager::Instance()->SetNewState(kMisalignGeometry);
    TVirtualMCApplication::Instance()->MisalignGeometry(); 
    TG4StateManager::Instance()->SetNewState(kNotInApplication);
  }    

  // VMC application construct geometry 
  if ( fUserGeometry == "RootToGeant4" ) {

    if ( VerboseLevel() > 1 ) 
      G4cout << "Running TVirtualMCApplication::ConstructGeometry" << G4endl;

    TG4StateManager::Instance()->SetNewState(kConstructGeometry);
    TVirtualMCApplication::Instance()->ConstructGeometry(); 
    TG4StateManager::Instance()->SetNewState(kNotInApplication);
    
    // If Root geometry was not closed by user
    // we have to do it here
    if ( ! gGeoManager->IsClosed() ) {
      TGeoVolume *top = (TGeoVolume*)gGeoManager->GetListOfVolumes()->First();
      gGeoManager->SetTopVolume(top);
      gGeoManager->CloseGeometry();  
    }  
    
    TG4StateManager::Instance()->SetNewState(kMisalignGeometry);
    TVirtualMCApplication::Instance()->MisalignGeometry(); 
    TG4StateManager::Instance()->SetNewState(kNotInApplication);
  }    

  // Build G4 geometry
  if ( fUserGeometry == "VMCtoGeant4" ) 
    ConstructG4GeometryViaVMC();
  
  if ( fUserGeometry == "RootToGeant4" ) 
    ConstructG4GeometryViaVGM();

  // print G4 geometry statistics
  if ( VerboseLevel() > 0 ) {
    G4cout << "G4 Stat: instantiated " 
           << fGeometryServices->NofG4LogicalVolumes()  
           << " logical volumes \n"
           << "                      " 
           << fGeometryServices->NofG4PhysicalVolumes() 
           << " physical volumes" << G4endl;
  }           
}                   

//_____________________________________________________________________________
void TG4GeometryManager::FillMediumMapFromG3()
{
/// Map G3 tracking medium IDs to volumes names.

  if ( VerboseLevel() > 1 ) 
    G4cout << "TG4GeometryManager::FillMediumMapFromG3()" << G4endl;

  TG4MediumMap* mediumMap = fGeometryServices->GetMediumMap();
 
  // Create medium for each medium entry
   for ( G4int i=0; i<G4int(G3Med.GetSize()); i++ ) {
    G3MedTableEntry* mediumEntry = G3Med.GetMTE(i);
    G4int mediumID = mediumEntry->GetID();
    
    if ( VerboseLevel() > 2 ) {
      G4cout << "Adding medium ID=" << mediumID << G4endl; 
    }
    TG4Medium* medium = mediumMap->AddMedium(mediumID, false);
    medium->SetLimits(mediumEntry->GetLimits());
    medium->SetMaterial(mediumEntry->GetMaterial());
  }   

  if ( VerboseLevel() > 2 ) 
    G3Vol.PrintAll();
    
  // Map media to logical volumes
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for ( G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv  = (*lvStore)[i];

    // Get medium ID from G3 tables    
    G4String name  = lv->GetName();
    G4String g3Name(name);
    // Filter out the reflected volume name extension
    // added by reflection factory 
    G4String ext = G4ReflectionFactory::Instance()->GetVolumesNameExtension();
    if (name.find(ext)) g3Name = g3Name.substr(0, g3Name.find(ext));
    G4int mediumID = G3Vol.GetVTE(g3Name)->GetNmed();
    
    if ( VerboseLevel() > 2 ) 
      G4cout << "Mapping medium Id " << mediumID << " to LV "<< name << G4endl; 

    // Map medium to LV
    mediumMap->MapMedium(lv, mediumID);
  }

  // clear G3 tables 
  G3Vol.Clear();  
  G3SensVol.clear(); 
  G3Mat.Clear();
  G3Med.Clear();
}    

//_____________________________________________________________________________
void TG4GeometryManager::FillMediumMapFromG4()
{
/// Map G4 materials in the medium map;
/// the materialIndex is used to define medium ID

  if ( VerboseLevel() > 1 ) 
    G4cout << "TG4GeometryManager::FillMediumMapFromG4()" << G4endl;

  TG4MediumMap* mediumMap = fGeometryServices->GetMediumMap();

  // Create medium for each material
  const G4MaterialTable* materialTable = G4Material::GetMaterialTable();
  for ( G4int i=0; i<G4int(materialTable->size()); i++ ) {
    G4Material* material = (*materialTable)[i];

    if ( VerboseLevel() > 2 ) {
      G4cout << "Adding medium name= " << material->GetName() 
             << " Id="  << material->GetIndex() << G4endl; 
    }             
    TG4Medium* medium = mediumMap->AddMedium(material->GetIndex()); 
    medium->SetName(material->GetName());
    medium->SetMaterial(material);
  }    
    
  // Map media to logical volumes
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for ( G4int i=0; i<G4int(lvStore->size()); i++ ) {
    G4LogicalVolume* lv  = (*lvStore)[i];
    G4int mediumID = lv->GetMaterial()->GetIndex();

    if ( VerboseLevel() > 2 ) {
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

  if ( VerboseLevel() > 1 ) 
    G4cout << "TG4GeometryManager::FillMediumMapFromRoot()" << G4endl;

  // fGeometryServices->PrintLogicalVolumeStore();
  
  TG4MediumMap* mediumMap = fGeometryServices->GetMediumMap();
 
  // Create TG4 medium for each TGeo madium
  TIter next(gGeoManager->GetListOfMedia());
  TGeoMedium* geoMedium;
  while ( ( geoMedium = (TGeoMedium*)next() ) )  {
    Int_t mediumId = geoMedium->GetId(); 
    G4String mediumName = geoMedium->GetName();
    
    //Int_t isvol  = (Int_t) geoMedium->GetParam(0);
    //Int_t ifield = (Int_t) geoMedium->GetParam(1);
    //Double_t fieldm = geoMedium->GetParam(2);
    //Double_t tmaxfd = geoMedium->GetParam(3);
    Double_t stemax = geoMedium->GetParam(4);
    //Double_t deemax = geoMedium->GetParam(5);
    //Double_t epsil  = geoMedium->GetParam(6);
    //Double_t stmin  = geoMedium->GetParam(7);
              
   // Only stemax parameter is passed to G4 if it is positive
    G4UserLimits* limits = 0;
    if ( stemax > 0 ) {
      limits = new G4UserLimits();
      limits->SetMaxAllowedStep(stemax*cm);
    } 
    
    if ( VerboseLevel() > 2 ) {
      G4cout << "Adding medium Id=" << mediumId << " name=" << mediumName
             << " limits=" << limits << G4endl; 
    }             
    TG4Medium* medium = mediumMap->AddMedium(mediumId);
    medium->SetName(mediumName);
    medium->SetLimits(limits);

    G4String matName = geoMedium->GetMaterial()->GetName();
    G4Material* material = G4Material::GetMaterial(matName);
    if ( ! material ) {
      TG4Globals::Exception(
        "TG4GeometryManager", "FillMediumMapFromRoot",
        "Material " + TString(matName) + " not found.");
    }
    medium->SetMaterial(material);
  }   

  // Map media to logical volumes
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i=0; i<G4int(lvStore->size()); i++ ) {
    G4LogicalVolume* lv  = (*lvStore)[i];
    G4String volName =lv->GetName(); 

    // Filter out the reflected volumes name extension
    // added by reflection factory 
    G4String ext = G4ReflectionFactory::Instance()->GetVolumesNameExtension();
    if (volName.find(ext)) volName = volName.substr(0, volName.find(ext));
    
    TGeoVolume* geoVolume = gGeoManager->GetVolume(volName.data());
    
    if ( ! geoVolume) {
      TG4Globals::Exception(
        "TG4GeometryManager", "FillMediumMapFromRoot",
        "Root volume " + TString(volName) + " not found"); 
    }  
    
    if ( geoVolume && ! geoVolume->GetMedium() ) {
      if ( ! geoVolume->IsAssembly() ) {
        TG4Globals::Exception(
          "TG4GeometryManager", "FillMediumMapFromRoot",
          "Root volume " + TString(volName) + " has not medium defined.");
      }
      else 
        continue;   
    }  
    
    G4int mediumID = geoVolume->GetMedium()->GetId();
    
    if ( VerboseLevel() > 2 ) {
      G4cout << "Mapping medium Id=" << mediumID << " to LV="<< volName 
             << G4endl; 
    }             
    mediumMap->MapMedium(lv, mediumID);   
  }
}    

//_____________________________________________________________________________
void TG4GeometryManager::FillMediumMap()
{
/// Fill medium map depending on user geometry source

  if ( fUserGeometry == "VMCtoGeant4" )
    FillMediumMapFromG3(); 

  if ( fUserGeometry == "VMCtoRoot" ||
       fUserGeometry == "Root"  || 
       fUserGeometry == "RootToGeant4" )
    FillMediumMapFromRoot(); 

  if ( fUserGeometry == "Geant4" )
    FillMediumMapFromG4(); 
}                   

//
// public methods
//
 
//_____________________________________________________________________________
TVirtualMCGeometry*  TG4GeometryManager::GetMCGeometry() const
{
/// Return the instance of MC geometry;
/// give exception if no object is instantiated

  if ( ! fMCGeometry ) {
    TG4Globals::Exception(
      "TG4GeometryManager", "GetMCGeometry",
      "No MC geometry defined.");
  }
  
  return fMCGeometry;
}      

//_____________________________________________________________________________
void TG4GeometryManager::ConstructMagField()
{
/// Create magnetic field

  if ( VerboseLevel() > 1 ) 
    G4cout << "TG4GeometryManager::ConstructMagField" << G4endl;

  switch (fMagneticFieldType) {
  
    case kMCApplicationField:
      fMagneticField = new TG4MagneticField();
      if ( VerboseLevel() > 1 )
        G4cout << "kMCApplicationField" << G4endl;
      break;

    case kUniformField:
      fMagneticField = new TG4UniformMagneticField();
      if ( VerboseLevel() > 1 )
        G4cout << "kUniformField" << G4endl;
      break;
      
    case kNoField:
      if ( VerboseLevel() > 1 )
        G4cout << "kNoField" << G4endl;
      ;;
  }  
}

//_____________________________________________________________________________
void TG4GeometryManager::ConstructGeometry()
{
/// Construct Geant4 geometry depending on user geometry source

  // Create magnetic field
  ConstructMagField();  

  // Construct G4 geometry 
  ConstructG4Geometry();

  // Fill medium map
  FillMediumMap(); 

  // VMC application construct geometry for optical processes
  TG4StateManager::Instance()->SetNewState(kConstructOpGeometry);
  TVirtualMCApplication::Instance()->ConstructOpGeometry();   
  TG4StateManager::Instance()->SetNewState(kNotInApplication);

}                   

//_____________________________________________________________________________
void TG4GeometryManager::FinishGeometry()
{
/// Finish geometry construction after G4 run initialization

  // Fill medium map if not yet done
  if ( fGeometryServices->GetMediumMap()->GetNofMedia() == 0 )
    FillMediumMap(); 

  // Set world to geometry services
  fGeometryServices->SetWorld(
    G4TransportationManager::GetTransportationManager()
      ->GetNavigatorForTracking()->GetWorldVolume());
    
}

//_____________________________________________________________________________
void TG4GeometryManager::SetUserLimits(const TG4G3CutVector& cuts,
                               const TG4G3ControlVector& controls) const
{
/// Set user limits defined in G3MedTable for all logical volumes.

  if ( VerboseLevel() > 1 ) 
    G4cout << "TG4GeometryManager::SetUserLimits" << G4endl; 

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    TG4Medium* medium 
      = fGeometryServices->GetMediumMap()->GetMedium(lv, false);
      
    if ( !medium) continue;  
      
    // get limits if already exist
    TG4Limits* tg4Limits = 0;
    G4UserLimits* limits = medium->GetLimits();
    tg4Limits = fGeometryServices->GetLimits(limits, cuts, controls);

    // get tracking medium name
    G4String name = medium->GetName();
    
    if (tg4Limits) 
      tg4Limits->SetName(name);
    else {
      tg4Limits = fGeometryServices->FindLimits(name, true);  
      if (!tg4Limits) 
        tg4Limits = new TG4Limits(name, cuts, controls); 
    }
    
    // set new limits back to medium
    medium->SetLimits(tg4Limits);

    // limit max step for low density materials (< AIR)
    if (lv->GetMaterial()->GetDensity() < fgLimitDensity ) 
      tg4Limits->SetMaxAllowedStep(fgMaxStep);
      
    // update controls in limits according to the setup 
    // in the passed vector
    tg4Limits->Update(controls);

    // set limits to logical volume
    lv->SetUserLimits(tg4Limits);
  } 
}


//_____________________________________________________________________________
void TG4GeometryManager::SetFieldType(TG4MagneticFieldType fieldType)
{
/// Select from available magnetic field types:
/// field defined by TVirtualMCApplication, uniform magnetic field
/// or no magnetic field.                                                    \n
/// Applicable only when no field has been yet created (PreInit).

  if (fMagneticField) {
    TG4Globals::Warning(
      "TG4GeometryManager", "SetFieldType",
      "The magnetic field already exists." + TG4Globals::Endl() +
      "Selection was ignored.");
  }   

  fMagneticFieldType = fieldType;
}  

//_____________________________________________________________________________
void TG4GeometryManager::SetUniformFieldValue(G4double fieldValue)
{
/// Set uniform magnetic field to specified value.
  
  if ( ! fMagneticField ) {
     TG4Globals::Exception(
       "TG4GeometryManager", "SetUniformMagField",
       "Magnetic field is not defined.");
  }   

  // Check field type 
  TG4UniformMagneticField* uniformField 
    =dynamic_cast<TG4UniformMagneticField*>(fMagneticField);

  if ( ! uniformField ) {
    TG4Globals::Exception(
      "TG4GeometryManager", "SetUniformMagField",
      "Defined magnetic field is not uniform.");
  }   

  // Set value
  uniformField->SetFieldValue(fieldValue);
}

