// $Id: $
//
// Author: I. Hrivnacova, 31.03.2004 
//
// Class TG4GDMLGeometryGenerator
// ------------------------------
// See the class description in the header file.

#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>

#include "TG4GDMLGeometryGenerator.h"
#include "TG4GDMLConvertor.h"

//_____________________________________________________________________________
TG4GDMLGeometryGenerator::TG4GDMLGeometryGenerator()
  : TG4VXMLGeometryGenerator("GDML")
{
//
  fConvertor = new TG4GDMLConvertor(fOutFile);  
}

//_____________________________________________________________________________
TG4GDMLGeometryGenerator::TG4GDMLGeometryGenerator(
                                       const TG4GDMLGeometryGenerator& right) 
  : TG4VXMLGeometryGenerator(right)
{
// 
}

//_____________________________________________________________________________
TG4GDMLGeometryGenerator::~TG4GDMLGeometryGenerator() {
//
}

// operators

//_____________________________________________________________________________
TG4GDMLGeometryGenerator& 
TG4GDMLGeometryGenerator::operator=(const TG4GDMLGeometryGenerator& right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  // call assignement of the base class
  TG4VXMLGeometryGenerator::operator=(right);

  return *this;  
}    
          
//
// protected methods
//

//_____________________________________________________________________________
void TG4GDMLGeometryGenerator::GenerateXMLGeometry(G4LogicalVolume* lv)  
{
// Generates XML geometry file starting from the specified logical volume.
// ---

  // Compose filename
  G4String fileName;
  if (fFileName == fgkUndefinedFileName) {
    fileName = lv->GetName();
    fileName = fileName + ".gdml";
  }  
  else
    fileName = fFileName;
  
  // Set top volume name
  G4String topName = lv->GetName() + "_comp";
  
  // Open XML file  
  OpenFile(fileName);

  // Generate volumes tree
  GenerateSection(lv);
  CloseFile();
  
  if (fVerboseLevel > 0) 
    G4cout << "File " << fileName << " has been generated." << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLGeometryGenerator::GenerateSection(G4LogicalVolume* lv)
{
// Generates the XML section element containing
// all geometry objects defined in given logical volume:
// rotation matrices, solids and volumes hierarchy.
// ---

  // Create section
  fConvertor->OpenSection(lv->GetName());  
  fConvertor->WriteEmptyLine();
  
  // Process basic elements needed by geometry tree
  GeneratePositions(lv);
  GenerateRotations(lv);
  GenerateMaterials(lv);
  GenerateSolids(lv);
    
  // Process geometry tree
  fConvertor->OpenStructure();  
  ProcessLogicalVolume(lv);
  fConvertor->CloseStructure();  
  fConvertor->WriteEmptyLine();
  ClearVolumeNames();
  
  // Close section
  fConvertor->CloseSection(lv->GetName());
}   

//_____________________________________________________________________________
void TG4GDMLGeometryGenerator::ProcessLogicalVolume(G4LogicalVolume* lv) 
{
// Writes logical volume tree.
// ---
  
  G4int nofDaughters = lv->GetNoDaughters();

  if (nofDaughters == 0) {
    // Open composition
    fConvertor->OpenComposition(lv->GetName(), 
                                lv->GetMaterial()->GetName());
  }
  else {
   
    // Recursively process daughters
    //
    for (G4int i=0; i<nofDaughters; i++) {
      
      G4LogicalVolume* lvd = lv->GetDaughter(i)->GetLogicalVolume();
      
      if (fVolumeNames.find(lvd->GetName()) == fVolumeNames.end())
        ProcessLogicalVolume(lvd);
    }
    
    // Write the volume with its childs now
    //
    // Open composition
    fConvertor->OpenComposition(lv->GetName(), 
                                lv->GetMaterial()->GetName());
    
    // Write positions  
    for (G4int j=0; j<nofDaughters; j++) {

      if (fVerboseLevel > 1) {
        G4cout << "processing " << j << "th daughter of " 
             << lv->GetName() << G4endl;
      }	     
   
      G4VPhysicalVolume* vpvd = lv->GetDaughter(j);
      G4LogicalVolume* lvd = vpvd->GetLogicalVolume();
      
      G4PVPlacement* pvd = dynamic_cast<G4PVPlacement*>(vpvd);
      if (pvd) {
    
        // Get position
        G4ThreeVector  position = vpvd->GetTranslation();
	G4String positionRef = FindPositionName(position);
      
        // Get rotation
        const G4RotationMatrix* rotation = vpvd->GetObjectRotation();
        if (!rotation) rotation = &HepRotation::IDENTITY;      
	G4String rotationRef = FindRotationName(rotation);
	
        fConvertor->WritePositionWithRotation(
	                 lvd->GetName(), positionRef, rotationRef);
      }
      else {
          G4cerr << "    TG4GDMLGeometryGenerator::ProcessLogicalVolume: " << G4endl;
          G4cerr << "    Limitation: " << G4endl;
          G4cerr << "    Other physical volumes than PVPlacement";
          G4cerr << " are not supported." << G4endl;
	  G4cerr << "    Volume " << vpvd->GetName() << " was not converted." << G4endl;  
	  G4cerr << G4endl;  
          //G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
          //exit(1);
      }
/*
    else {
      G4PVReplica* pvr = dynamic_cast<G4PVReplica*>(vpvd);
      G4PVDivision* pvd = dynamic_cast<G4PVDivision*>(vpvd);
      if (pvr || pvd) {

        // get parameters
        EAxis axis;
        G4int nReplicas;
        G4double width;
        G4double offset;
        
	if (pvr) {
  	  G4bool consuming;
          pvr->GetReplicationData(axis, nReplicas, width, offset, consuming);
	}

	if (pvd) { 
          G4VDivisionParameterisation* param
            = (G4VDivisionParameterisation*)(pvd->GetParameterisation());
	    
          exis = param->GetAxis();
          nReplicas = param->GetNoDiv();
          width =  param->GetWidth();
          offset = param->GetOffset();
	}  
	    
        // write multiple position
    	fConvertor
	  ->WriteMultiplePosition(lvName, axis, nReplicas, width, offset);

        // if volume is not leaf node place its logical volume
        if (nd>0) 
      	  fConvertor
	    ->WriteMultiplePosition(compName, axis, nReplicas, width, offset);
      }
      else {
        G4cerr << "    TG4AGDDGeometryGenerator::ProcessLogicalVolume: " << G4endl;
        G4cerr << "    Limitation: " << G4endl;
        G4cerr << "    Other physical volumes than "<< G4endl;
        G4cerr << "       PVPlacement, PVReplica and PVDivision" << G4endl;
        G4cerr << "    are not implemented." << G4endl;
	G4cerr << "    Volume " << vpvd->GetName() << " was not converted." << G4endl;  
        //G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
        //exit(1);
      }
*/        
    }  
  }
  
  // Close composition
  fConvertor->CloseComposition();	
  fConvertor->WriteEmptyLine();

  // store the name of logical volume in the set
  fVolumeNames.insert(fVolumeNames.begin(), lv->GetName()); 
}  

