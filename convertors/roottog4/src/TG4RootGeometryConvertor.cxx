// $Id: TG4RootGeometryConvertor.cxx,v 1.3 2003/12/19 14:39:43 brun Exp $
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
#include <G4PVDivision.hh>
#include <G4Transform3D.hh>
#include <G4ReflectionFactory.hh>
#include <G3toG4RotationMatrix.hh>

#include "TG4RootGeometryConvertor.h"
#include "TG4MaterialConvertor.h"
#include "TG4ShapeConvertor.h"
#include "TG4TGeoUnits.h"

const char TG4RootGeometryConvertor::fgDefaultSeparator = '@';

//_____________________________________________________________________________
TG4RootGeometryConvertor::TG4RootGeometryConvertor() 
  : fMaterialConvertor(0),
    fShapeConvertor(0),
    fVolumesMap(),
    fSeparator(fgDefaultSeparator)
{
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
  newName.append(fSeparator); 
  Append(newName, number);
	   
  lv->SetName(newName);
}  

//_____________________________________________________________________________
Bool_t TG4RootGeometryConvertor::IsDivided(const TGeoVolume* mother)
{
// Returns true if the volume is divided
// and can be processed via G4PVDivision.
// ---

  // Check if division is present
  const TGeoPatternFinder* finder = mother->GetFinder();    
  if (!finder) return false;
    
  // Get division axis
  EAxis axis = GetAxis(finder);
  if (axis == kUndefined) return false;
  
  // Volume can be processed via G4PVDivision
  return true;
}  

//_____________________________________________________________________________
EAxis TG4RootGeometryConvertor::GetAxis(const TGeoPatternFinder* finder) const
{
// Checks the finder concrete type and returns the division axis.
// ---

  const TGeoPatternX*  finderX 
    = dynamic_cast<const TGeoPatternX*>(finder);
  if (finderX) return kXAxis;

  const TGeoPatternY*  finderY 
    = dynamic_cast<const TGeoPatternY*>(finder);
  if (finderY) return kYAxis;

  const TGeoPatternZ*  finderZ 
    = dynamic_cast<const TGeoPatternZ*>(finder);
  if (finderZ) return kZAxis;

  const TGeoPatternParaX*  finderParaX 
    = dynamic_cast<const TGeoPatternParaX*>(finder);
  if (finderParaX) return kXAxis;

  const TGeoPatternParaY*  finderParaY 
    = dynamic_cast<const TGeoPatternParaY*>(finder);
  if (finderParaY) return kYAxis;

  const TGeoPatternParaZ*  finderParaZ 
    = dynamic_cast<const TGeoPatternParaZ*>(finder);
  if (finderParaZ) return kZAxis;

  const TGeoPatternTrapZ*  finderTrapZ 
    = dynamic_cast<const TGeoPatternTrapZ*>(finder);
  if (finderTrapZ) return kZAxis;

  const TGeoPatternCylR* finderR 
    = dynamic_cast<const TGeoPatternCylR*>(finder);
  if (finderR) return kRho;

  const TGeoPatternCylPhi* finderPhi 
    = dynamic_cast<const TGeoPatternCylPhi*>(finder);
  if (finderPhi) return kPhi;

  const TGeoPatternSphR* finderSphR 
    = dynamic_cast<const TGeoPatternSphR*>(finder);
  if (finderSphR) return kRadial3D;

  const TGeoPatternSphPhi* finderSphPhi 
    = dynamic_cast<const TGeoPatternSphPhi*>(finder);
  if (finderSphPhi) return kPhi;
  
  // Not available in G4
  // sphTheta
  // Honeycomb
  
  return  kUndefined;
}

//_____________________________________________________________________________
Int_t TG4RootGeometryConvertor::GetIAxis(EAxis axis) const
{
// Converts G4 axis enum to Root axis number.
// ---

  switch (axis) {
    case kXAxis: return 1;
    case kYAxis: return 2;
    case kZAxis: return 3;
    case kRho:   return 1;
    case kPhi:   return 2;
    case kRadial3D:  return 1;
    case kUndefined: return 0;
  }
  
  return 0;
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
void TG4RootGeometryConvertor::CreatePlacements(const TGeoVolume* mother, 
                                                G4LogicalVolume* mLV)
{
// Builds G4 geometry for the daughters of the specified volume.
// ---

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

//_____________________________________________________________________________
void TG4RootGeometryConvertor::CreateDivision(const TGeoVolume* mother,
                                              G4LogicalVolume* mLV)
{
// Creates G4 physical volume division for the specified mother
// ---

  // Get  pattern finder
  const TGeoPatternFinder* finder = mother->GetFinder();    
  TGeoPatternFinder* finderNonConst = mother->GetFinder();    
  if (!finder) { 
    // add warning
    return;
  }  

  // Get division axis
  EAxis axis = GetAxis(finder);
  if (axis == kUndefined) {
    // add warning
    return;
  }

  // Get the first division volume
  TGeoNode* dNode = finderNonConst->GetNodeOffset(0);
  TGeoVolume* daughter = dNode->GetVolume();

  // Debug info
  // daughter->InspectShape();
   
  VolumesMapIterator itd = fVolumesMap.find(daughter);
  G4LogicalVolume* dLV = (*itd).second;
  
  // Debug info
  //G4cout << "Mother solid: " << *mLV->GetSolid() << G4endl;
  //G4cout << "Cell solid: " << *dLV->GetSolid() << G4endl;
  
  // Get division parameters
  G4int ndiv = finderNonConst->GetNdiv();
  G4double start = finder->GetStart();
  Double_t xlo, xhi;
  mother->GetShape()->GetAxisRange(GetIAxis(axis), xlo, xhi);
  G4double offset = start - xlo; 
  G4double width  = finder->GetStep();
    
  // Convert units
  if (axis == kXAxis || axis == kYAxis || axis == kZAxis ||
      axis == kRho   || axis == kRadial3D) { 
    offset *= TG4TGeoUnits::Length();
    width *= TG4TGeoUnits::Length();
  }  

  if (axis == kPhi) {
    offset *= TG4TGeoUnits::Angle();
    width *= TG4TGeoUnits::Angle();
  }  
  
  // Place this node
  new G4PVDivision(G4String(daughter->GetName()), dLV, mLV, 
                   axis, ndiv, width, offset);
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
    
    if (lvName.contains(fSeparator)) continue;
     
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
void TG4RootGeometryConvertor::ProcessPositions()
{
// Builds G4 geometry for the daughters of all volumes.
// ---

  VolumesMapIterator it;
  for (it = fVolumesMap.begin(); it != fVolumesMap.end(); it++) {
    const TGeoVolume* mother = (*it).first;
    G4LogicalVolume* mLV = (*it).second;

    if (IsDivided(mother))
      CreateDivision(mother, mLV);
    else 
      CreatePlacements(mother, mLV);
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

