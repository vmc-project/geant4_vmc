// $Id: TG4AGDDGeometryGenerator.cxx,v 1.2 2003/12/18 13:25:11 brun Exp $
//
// Author: I. Hrivnacova, 27.07.2000 
// (redesigned  31.03.2004) 
//
// Class TG4AGDDGeometryGenerator
// ------------------------------
// See the class description in the header file.

#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4PVDivision.hh>

#include "TG4AGDDGeometryGenerator.h"
#include "TG4AGDDConvertor.h"

//_____________________________________________________________________________
TG4AGDDGeometryGenerator::TG4AGDDGeometryGenerator()
  : TG4VXMLGeometryGenerator("AGDD")
{
//
  fConvertor = new TG4AGDDConvertor(fOutFile);  
}

//_____________________________________________________________________________
TG4AGDDGeometryGenerator::TG4AGDDGeometryGenerator(
                                       const TG4AGDDGeometryGenerator& right) 
  : TG4VXMLGeometryGenerator(right)
{
// 
}

//_____________________________________________________________________________
TG4AGDDGeometryGenerator::~TG4AGDDGeometryGenerator() {
//
}

// operators

//_____________________________________________________________________________
TG4AGDDGeometryGenerator& 
TG4AGDDGeometryGenerator::operator=(const TG4AGDDGeometryGenerator& right)
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
void TG4AGDDGeometryGenerator::GenerateXMLGeometry(G4LogicalVolume* lv)  
{
// Generates XML geometry file for the geometry tree 
// starting from the specified logical volume.
// ---

  // filename
  G4String fileName;
  if (fFileName == fgkUndefinedFileName) {
    fileName = lv->GetName();
    fileName = fileName + ".xml";
  }  
  else 
    fileName = fFileName;
  
  // set top volume name
  G4String topName = lv->GetName() + "_comp";
  
  // generate XML  
  OpenFile(fileName);

  // generate materials 
  // not implemented
  // GenerateMaterials(version, "today", "Generated from G4", "v4", lv);

  // generate volumes tree
  GenerateSection(lv);
  CloseFile();
  
  if (fVerboseLevel > 0) 
    G4cout << "File " << fileName << " has been generated." << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDGeometryGenerator::GenerateSection(G4LogicalVolume* lv)
{
// Generates the XML section element containing
// all geometry objects defined in given logical volume:
// solids and volumes tree.
// ---

  // create section
  fConvertor->OpenSection(lv->GetName());  
  fConvertor->WriteEmptyLine();
  
  // process solids
  GenerateSolids(lv);
    
  // process geometry tree
  ProcessLogicalVolume(lv);
  fConvertor->WriteEmptyLine();
  ClearVolumeNames();
  
  // close section
  fConvertor->CloseSection(lv->GetName());
}   

//_____________________________________________________________________________
void TG4AGDDGeometryGenerator::ProcessLogicalVolume(G4LogicalVolume* lv) 
{
// Writes the logical volume tree.
// ---
  
  G4int nofDaughters = lv->GetNoDaughters();
  if (nofDaughters == 0) return;
  
  // open composition
  G4String lvName = lv->GetName();
  G4String name = lvName;
  name.append("_comp");
  fConvertor->OpenComposition(name, lv->GetMaterial()->GetName());
  
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
    
    G4PVPlacement* pvd = dynamic_cast<G4PVPlacement*>(vpvd);
    if (pvd) {
    
      // placement
      G4ThreeVector  position = vpvd->GetTranslation();

      if (!vpvd->GetRotation()) {
     	fConvertor->WritePosition(lvName, position);
        // if volume is not leaf node place its logical volume
        if (nd>0) 
    	  fConvertor->WritePosition(compName, position);
      }
      else {  
        const G4RotationMatrix* kMatrix = vpvd->GetObjectRotation();      
	
  	fConvertor->WritePositionWithRotation(lvName, position, kMatrix);
        if (nd>0) 
      	   fConvertor->WritePositionWithRotation(compName, position, kMatrix);
      }
    }
    else {
      G4PVReplica* pvr = dynamic_cast<G4PVReplica*>(vpvd);
      G4PVDivision* pvdiv = dynamic_cast<G4PVDivision*>(vpvd);
      if (pvr || pvdiv) {

        // get parameters
        EAxis axis;
        G4int nReplicas;
        G4double width;
        G4double offset;
        
	if (pvr) {
  	  G4bool consuming;
          pvr->GetReplicationData(axis, nReplicas, width, offset, consuming);
	}

	if (pvdiv) { 
          G4VDivisionParameterisation* param
            = (G4VDivisionParameterisation*)(pvdiv->GetParameterisation());
	    
          axis = param->GetAxis();
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

