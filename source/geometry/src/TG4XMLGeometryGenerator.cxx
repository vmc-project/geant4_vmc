// $Id: TG4XMLGeometryGenerator.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: geometry
//
// Author: I. Hrivnacova, 27.07.2000 
//
// Class TG4XMLGeometryGenerator
// -----------------------------
// See the class description in the header file.

#include "TG4XMLGeometryGenerator.h"
#include "TG4XMLConvertor.h"

#include <G4Material.hh>
#include <G4VSolid.hh>
#include <G4ReflectedSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4TransportationManager.hh>
#include <G4Navigator.hh>
#include <globals.hh>

#include <g4std/iomanip>
#include <g4std/vector>

#include <stdlib.h>

TG4XMLGeometryGenerator* TG4XMLGeometryGenerator::fgInstance = 0;

//_____________________________________________________________________________
TG4XMLGeometryGenerator::TG4XMLGeometryGenerator()
  : fMessenger(this),
    fVerboseLevel(1) 
{
//
  if (fgInstance) {
    G4cerr << "    TG4XMLGeometryGenerator::TG4XMLGeometryGenerator: " << G4endl;
    G4cerr << "    Attempt to create two instances of singleton." << G4endl;
    G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
    exit(1);
  }

  fConvertor = new TG4XMLConvertor(fOutFile);  
}

//_____________________________________________________________________________
TG4XMLGeometryGenerator::TG4XMLGeometryGenerator(
                                       const TG4XMLGeometryGenerator& right) 
  : fMessenger(this),
    fVerboseLevel(0)  
{
// 
    G4cerr << "    Attempt to copy TG4XMLGeometryGenerator singleton." << G4endl;
    G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
    exit(1);
}

//_____________________________________________________________________________
TG4XMLGeometryGenerator::~TG4XMLGeometryGenerator() {
//
  delete fConvertor;
  fgInstance = 0;
}

// operators

//_____________________________________________________________________________
TG4XMLGeometryGenerator& 
TG4XMLGeometryGenerator::operator=(const TG4XMLGeometryGenerator& right)
{
  // check assignement to self
  if (this == &right) return *this;

  G4cerr << "    Attempt to assign TG4XMLGeometryGenerator singleton." << G4endl;
  G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
  exit(1);
    
  return *this;  
}    
          

// private methods

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::CutName(G4String& name) const
{
// Removes spaces after the name if present.
// ---

  G4int i = name.length();
  while (name(--i) == ' ') name = name(0,i);
}  

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::ProcessSolids(G4LogicalVolume* lv) 
{
// Writes all solids of given logical volume.
// ---

  G4VSolid* solid = lv->GetSolid();
  G4String lvName = lv->GetName();
  G4String material = lv->GetMaterial()->GetName();
  fConvertor->WriteSolid(lvName, solid, material);
  
  // store the name of logical volume in the set
  fVolumeNames.insert(fVolumeNames.begin(), lvName); 

  // process daughters
  G4int nofDaughters = lv->GetNoDaughters();
  if (nofDaughters>0) 
    for (G4int i=0; i<nofDaughters; i++) {

      if (fVerboseLevel > 1) {
        G4cout << "processing " << i << "th daughter of " 
               << lv->GetName() << G4endl;
      }
              
      G4LogicalVolume* lvd = lv->GetDaughter(i)->GetLogicalVolume();
      G4String lvdName = lvd->GetName();

      if (fVolumeNames.find(lvdName) == fVolumeNames.end()) {
        // process lvd only if it was not yet processed
        ProcessSolids(lvd);
      }	
    }
}  

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::ProcessMaterials(G4LogicalVolume* lv) 
{
// Writes all materials of given logical volume.
// ---

  G4Material* material = lv->GetMaterial();
  
  // check if this material was already written
  G4bool written = false;
  G4String name = material->GetName();
  CutName(name);
  if (fMaterialNames.find(name) != fMaterialNames.end()) written = true;
  
  if (!written) {
    fConvertor->WriteMaterial(material);
    fMaterialNames.insert(fMaterialNames.begin(), name); 
  }  
  
  // store the name of logical volume in the set
  G4String lvName = lv->GetName();
  fVolumeNames.insert(fVolumeNames.begin(), lvName); 

  G4int nofDaughters = lv->GetNoDaughters();
  if (nofDaughters>0) 
    for (G4int i=0; i<nofDaughters; i++) {
      G4LogicalVolume* lvd = lv->GetDaughter(i)->GetLogicalVolume();
      G4String lvdName = lvd->GetName();

      if (fVolumeNames.find(lvdName) == fVolumeNames.end()) {
        // process lvd only if it was not yet processed
        ProcessMaterials(lvd);
      }	
    }
}  

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::ProcessRotations(G4LogicalVolume* lv) 
{
// Writes all rotation matrices of given logical volume.
// ---

  G4String lvName = lv->GetName();

  // store the name of logical volume in the set
  fVolumeNames.insert(fVolumeNames.begin(), lvName); 
  
  G4int nofDaughters = lv->GetNoDaughters();

  if (nofDaughters>0) {
    G4int i; 
    for (i=0; i<nofDaughters; i++) {
      
      G4VPhysicalVolume* pvd = lv->GetDaughter(i);
      const G4RotationMatrix* kRotation = pvd->GetRotation();
      if (kRotation) 
        fConvertor->WriteRotation(kRotation);

      G4LogicalVolume* lvd = pvd->GetLogicalVolume();
      G4String lvdName = lvd->GetName();

      if (fVolumeNames.find(lvdName) == fVolumeNames.end()) {
        // process lvd only if it was not yet processed
        ProcessRotations(lvd);
      }	
    }
  }  
}  

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::ProcessLogicalVolume(G4LogicalVolume* lv) 
{
// Writes logical volume tree.
// ---
  
  G4int nofDaughters = lv->GetNoDaughters();
  if (nofDaughters == 0) return;
  
  // open composition
  G4String lvName = lv->GetName();
  G4String name = lvName;
  name.append("_comp");
  fConvertor->OpenComposition(name);
  
  // write positions  
  G4int i;
  for (i=0; i<nofDaughters; i++) {

    if (fVerboseLevel > 1) {
      G4cout << "processing " << i << "th daughter of " 
             << lv->GetName() << G4endl;
    }	     
   
    G4VPhysicalVolume* vpvd = lv->GetDaughter(i);
    G4LogicalVolume* lvd = vpvd->GetLogicalVolume();
      
    // get parameters
    G4String lvName = lvd->GetName();
    G4String compName = lvd->GetName();
    compName.append("_comp");      
    G4int nd = lvd->GetNoDaughters(); 
    
    G4bool isReflected = false;
    if (dynamic_cast<G4ReflectedSolid*>(lvd->GetSolid())) 
      isReflected = true;
 
    G4PVPlacement* pvd = dynamic_cast<G4PVPlacement*>(vpvd);
    if (pvd) {
    
      // placement
      G4ThreeVector  position = vpvd->GetTranslation();

      if (!vpvd->GetRotation() && !isReflected) {
     	fConvertor->WritePosition(lvName, position);
        // if volume is not leaf node place its logical volume
        if (nd>0) 
    	  fConvertor->WritePosition(compName, position);
      }
      else {  
        static G4RotationMatrix kIdentity;
        const G4RotationMatrix* kMatrix = vpvd->GetObjectRotation();      
	if (!kMatrix)           kMatrix = &kIdentity;
	
  	fConvertor->WritePositionWithRotation(
	                 lvName, position, kMatrix, isReflected);
        if (nd>0) 
      	   fConvertor->WritePositionWithRotation(
	                    compName, position, kMatrix, isReflected);
      }
    }
    else {
      G4PVReplica* pvr = dynamic_cast<G4PVReplica*>(vpvd);
      if (pvr) {
        // replica
    	fConvertor->WriteReplica(lvName, pvr);
        // if volume is not leaf node place its logical volume
        if (nd>0) 
      	  fConvertor->WriteReplica(compName, pvr);
      }
      else {
        G4cerr << "    TG4XMLGeometryGenerator::ProcessLogicalVolume: " << G4endl;
        G4cerr << "    Limitation: " << G4endl;
        G4cerr << "    Other physical volumes than PVPlacement and PVReplica"
        G4cerr << " are not implemented.";
        G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
        exit(1);
      }
    }  
  }  

  // close composition
  fConvertor->CloseComposition();	
  fConvertor->WriteEmptyLine();

  // store the name of logical volume in the set
  fVolumeNames.insert(fVolumeNames.begin(), lvName); 

  // process daughters
  for (i=0; i<nofDaughters; i++) {
    G4LogicalVolume* lvd = lv->GetDaughter(i)->GetLogicalVolume();
    G4String lvdName = lvd->GetName();

    if (fVolumeNames.find(lvdName) == fVolumeNames.end()) {
      // process lvd only if it was not yet processed
      ProcessLogicalVolume(lvd);
    }
  }    
}  

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::GenerateXMLGeometry(G4LogicalVolume* lv)  
{
// Generates XML geometry file starting from the specified logical volume.
// ---

  // filename
  G4String fileName(lv->GetName());
  fileName = fileName + ".xml";
  
  // set top volume name
  G4String topName = lv->GetName() + "_comp";
  
  // generate XML  
  OpenFile(fileName);

  // generate materials 
  // not implemented
  // GenerateMaterials(version, "today", "Generated from G4", "v4", lv);

  // generate volumes tree
  GenerateSection("v6", lv->GetName(), "0", "today",
                  "Generated from Geant4", topName, lv);
  CloseFile();
  
  if (fVerboseLevel > 0) 
    G4cout << "File " << fileName << " has been generated." << G4endl;
}  

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::ClearMaterialNames() 
{
// Clears the set of material names.
// ---

  fMaterialNames.erase(fMaterialNames.begin(), fMaterialNames.end());
}  

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::ClearVolumeNames() 
{
// Clears the set of volume names.
// ---

  fVolumeNames.erase(fVolumeNames.begin(), fVolumeNames.end());
}  

// public methods

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::GenerateXMLGeometry()
{
// Generates XML geometry file from the top (world) volume.
// ---

  // Get world volume
  G4VPhysicalVolume* world 
    = G4TransportationManager::GetTransportationManager()
      ->GetNavigatorForTracking()->GetWorldVolume();
  GenerateXMLGeometry(world->GetLogicalVolume());
}  

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::GenerateXMLGeometry(const G4String& lvName) 
{
// Generates XML geometry file from the logical volume specified by name.
// ---

  // Find volume
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    if (lv->GetName() == lvName) {
      GenerateXMLGeometry(lv);
      return;
    }  
  }

  G4String text = "TG4XMLGeometryGenerator::GenerateXMLGeometry:\n"; 
  text = text + "    Logical volume " + lvName + " does not exist.";
  G4cerr << "Warning: " << text << G4endl;
}  

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::GenerateMaterials( 
                        const G4String& version, const G4String& date,
		        const G4String& author,  const G4String dtdVersion,
			G4LogicalVolume* lv)
{
// Generates the XML material element containing
// all materials present in given logical volume.
// ---

  // create section
  fConvertor->OpenMaterials(version, date, author, dtdVersion);  
  fConvertor->WriteEmptyLine();
  
  // process materials
  ProcessMaterials(lv);
  fConvertor->WriteEmptyLine();
  ClearMaterialNames();
  ClearVolumeNames();

  // close section
  fConvertor->CloseMaterials();
  fConvertor->WriteEmptyLine();
}   

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::GenerateSection(const G4String& versionDTD,
                        const G4String& name, const G4String& version,
		        const G4String& date, const G4String& author, 
			const G4String& topVolume, G4LogicalVolume* lv)
{
// Generates the XML section element containing
// all geometry objects defined in given logical volume:
// rotation matrices, solids and volumes hierarchy.
// ---

  // create section
  fConvertor->OpenSection(versionDTD, name, version, date, author, topVolume);  
  fConvertor->WriteEmptyLine();
  
  // process rotations
  //ProcessRotations(lv);
  //fConvertor->WriteEmptyLine();
  //ClearRotations();
  //ClearVolumeNames();
    
  // process solids
  ProcessSolids(lv);
  fConvertor->WriteEmptyLine();
  ClearVolumeNames();
    
  // process geometry tree
  ProcessLogicalVolume(lv);
  fConvertor->WriteEmptyLine();
  ClearVolumeNames();
  
  // close section
  fConvertor->CloseSection();
}   

//_____________________________________________________________________________
void TG4XMLGeometryGenerator::OpenFile(G4String filePath)
{ 
// Opens output file.
// ---

  fOutFile.open(filePath, G4std::ios::out); 
  
  if (!fOutFile) {
    G4String text = "Cannot open ";
    text = text + filePath;
    G4cerr << "Warning: " << text << G4endl;  
  }
  
  // use FORTRAN compatibility output
  fOutFile.setf(G4std::ios::fixed, G4std::ios::floatfield);
}


//_____________________________________________________________________________
void TG4XMLGeometryGenerator::CloseFile()
{ 
// Closes output file.
// ---

  fOutFile.close(); 
}
