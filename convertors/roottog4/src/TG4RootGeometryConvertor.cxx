// $Id: TG4RootGeometryConvertor.cxx,v 1.1 2003/07/22 06:46:58 brun Exp $
//
// Author: I. Hrivnacova, 8.1.2003 
//
// Class TG4RootGeometryConvertor
// ------------------------------
// See the class description in the header file.

#if __GNUC__ >= 3
#include <sstream>
#else
#include <strstream>
#endif

#include <TGeoVolume.h>
#include <TGeoNode.h>
#include <TGeoMatrix.h>
#include <TGeoManager.h>

#include <globals.hh>
#include <G4VSolid.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Transform3D.hh>
#include <G4ReflectionFactory.hh>
#include <G3toG4RotationMatrix.hh>

#include "TG4RootGeometryConvertor.h"
#include "TG4MaterialConvertor.h"
#include "TG4ShapeConvertor.h"
#include "TG4TGeoUnits.h"

const char TG4RootGeometryConvertor::fgSeparator = '@';

//_____________________________________________________________________________
TG4RootGeometryConvertor::TG4RootGeometryConvertor() {
//
  fMaterialConvertor = new TG4MaterialConvertor();
  fShapeConvertor    = new TG4ShapeConvertor();
}

//_____________________________________________________________________________
TG4RootGeometryConvertor::TG4RootGeometryConvertor(
                                       const TG4RootGeometryConvertor& right) 
{
// 
  G4Exception("G4RootGeometryConvertor: Copy constructor not implemented");
}

//_____________________________________________________________________________
TG4RootGeometryConvertor::~TG4RootGeometryConvertor() {
//
  delete fShapeConvertor;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4RootGeometryConvertor::Append(G4String& s, G4int a) const
{
// Appends number to string.
// ---

#if __GNUC__ >= 3
  std::ostringstream tmpStream;
#else  
  std::strstream tmpStream;
#endif

  tmpStream << a;  

  s.append(tmpStream.str());
}

//_____________________________________________________________________________
void TG4RootGeometryConvertor::SetUniqueName(G4LogicalVolume* lv, 
                                             G4int number) const
{
// Appends number with the separator to the logical volume name.
// ---
 
  G4String newName(lv->GetName());
  newName.append('@'); 
  Append(newName, number);
	   
  lv->SetName(newName);
}  

//_____________________________________________________________________________
G4LogicalVolume* 
TG4RootGeometryConvertor::CreateLV(const TGeoVolume* volume)
{
// Creates a solid and logical volume corresponding to the specified
// Root volume.
// ---

   VolumesMapIterator it = fVolumesMap.find(volume);
   if (it != fVolumesMap.end()) 
     return (*it).second;
   
   //G4cout << "Create LV: " << volume->GetName() << G4endl;

   // Convert shape
   G4VSolid* solid 
     = fShapeConvertor->Convert(volume->GetShape());
    
   // Convert material
   G4Material* material
     = fMaterialConvertor->GetMaterial(volume->GetMedium()->GetMaterial());

   // Create logical volume 
   G4LogicalVolume* lv 
     = new G4LogicalVolume(solid, material, G4String(volume->GetName()));
 
   fVolumesMap[volume] = lv;
   
   return lv;
}     

//_____________________________________________________________________________
void TG4RootGeometryConvertor::ProcessDaughters(G4LogicalVolume* motherLV, 
                                                const TGeoVolume* mother)
{
// Builds G4 geometry for the daughters of specified volume.
// ---

  for (G4int i=0; i<mother->GetNdaughters(); i++) {
    const TGeoNode* dNode = mother->GetNode(i);
    const TGeoVolume* daughter = dNode->GetVolume();

    VolumesMapIterator it = fVolumesMap.find(daughter);
      
    G4LogicalVolume* dLV = 0;
    if (it == fVolumesMap.end()) {
      // G4cout << "Process daughters: " << daughter->GetName() << G4endl;

      // Create logical volume 
      G4LogicalVolume* dLV = CreateLV(daughter);
      fVolumesMap[daughter] = dLV;
    }
    else
      dLV = (*it).second;

    if (it == fVolumesMap.end())     
      for (G4int j=0; j<daughter->GetNdaughters(); j++) 
         ProcessDaughters(dLV, daughter); 
  }  
}

//_____________________________________________________________________________
void TG4RootGeometryConvertor::SetUniqueNames()
{
// Set unique names to logical volumes and solids;
// names are composed as NAME@N, where NAME is the TGeomVolume name
// and N is integer number starting from 0.
// ---

  VolumesMapIterator it;
  for (it = fVolumesMap.begin(); it != fVolumesMap.end(); it++) {

    G4LogicalVolume* lv = (*it).second;
    G4String lvName(lv->GetName());
    
    if (lvName.contains(fgSeparator)) continue;
     
    G4int counter = 0;
    VolumesMapIterator it2;
    for (it2 = it; it2 != fVolumesMap.end(); it2++) {
      if (it2 == it) continue;
      G4LogicalVolume* lv2 = (*it2).second;
      if (lv2->GetName() == lvName) {               
        if (counter == 0) SetUniqueName(lv, counter++);
        SetUniqueName(lv2, counter++);
      }	
    }
  }
}

//_____________________________________________________________________________
void TG4RootGeometryConvertor::ProcessPositions()
{
// Builds G4 geometry for the daughters of specified volume.
// ---

  VolumesMapIterator it;
  for (it = fVolumesMap.begin(); it != fVolumesMap.end(); it++) {
    const TGeoVolume* mother = (*it).first;
    G4LogicalVolume* mLV = (*it).second;
 
    for (G4int i=0; i<mother->GetNdaughters(); i++) {
      const TGeoNode* node = mother->GetNode(i);
      const TGeoVolume* daughter = node->GetVolume();
      
      VolumesMapIterator itd = fVolumesMap.find(daughter);
      G4LogicalVolume* dLV = (*itd).second;

      // Convert transformation
      node->cd();
      TGeoMatrix* matrix = node->GetMatrix();
      const G4double* translation = matrix->GetTranslation();
      const G4double* rotation = matrix->GetRotationMatrix();
      const G4double* scale = matrix->GetScale();
     
      G4Translate3D translate3D(translation[0] * TG4TGeoUnits::Length(),
                                translation[1] * TG4TGeoUnits::Length(), 
	 		        translation[2] * TG4TGeoUnits::Length());
      G3toG4RotationMatrix g3g4Rotation;
      g3g4Rotation.SetRotationMatrixByRow(
                      G4ThreeVector(rotation[0], rotation[1], rotation[2]),
                      G4ThreeVector(rotation[3], rotation[4], rotation[5]),
                      G4ThreeVector(rotation[6], rotation[7], rotation[8]));
      G4Rotate3D rotate3D(g3g4Rotation);
      G4Scale3D scale3D(scale[0], scale[1], scale[2]);
 
      G4Transform3D transform3D = translate3D * (rotate3D) * scale3D;

      // Place this node
      //G4ReflectionFactory::Instance()
      //  ->Place(transform3D, G4String(node->GetName()), dLV, mLV, false, i);
      G4ReflectionFactory::Instance()
        ->Place(transform3D, G4String(daughter->GetName()), dLV, mLV, false, i);
    }
  }    	
}

//
// public methods
//

//_____________________________________________________________________________
G4VPhysicalVolume* 
TG4RootGeometryConvertor::Convert(const TGeoVolume* world)
{
// Converts the full geometry tree of specified root world volume.
// ---

  // Convert materials
  fMaterialConvertor->Convert(gGeoManager->GetListOfMaterials());
 
  // Create the top logical volume 
  G4LogicalVolume* worldLV = CreateLV(world);
  
  // Process daughters
  ProcessDaughters(worldLV, world);
  
  // Set unique names to volumes
  SetUniqueNames();

  // Process daughters
  ProcessPositions();
 
  // Place the top volume
  return new G4PVPlacement(0, G4ThreeVector(), G4String(world->GetName()),
                           worldLV, 0, false, 0);  
}

