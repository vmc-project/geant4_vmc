// $Id: TG4VXMLGeometryGenerator.cxx,v 1.1 2004/04/26 17:05:04 brun Exp $
//
// Author: I. Hrivnacova, 27.07.2000 
//
// Class TG4VXMLGeometryGenerator
// -----------------------------
// See the class description in the header file.

#include <iomanip>
#include <vector>
#include <stdlib.h>
#if __GNUC__ >= 3
#include <sstream>
#else
#include <strstream>
#endif

#include <G4VSolid.hh>
#include <G4ReflectedSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4TransportationManager.hh>
#include <G4Navigator.hh>

#include "TG4VXMLGeometryGenerator.h"
#include "TG4VXMLConvertor.h"
#include "TG4XMLUnits.h"

const G4String TG4VXMLGeometryGenerator::fgkUndefinedFileName = "Undefined";

//_____________________________________________________________________________
TG4VXMLGeometryGenerator::TG4VXMLGeometryGenerator(const G4String& xmlFormat)
  : fConvertor(0),
    fVolumeNames(),
    fOutFile(),
    fFileName(fgkUndefinedFileName),
    fVerboseLevel(1), 
    fMessenger(this, xmlFormat),
    fPositions(),
    fRotations(),
    fElements(),
    fMaterials()
{
//
}

//_____________________________________________________________________________
TG4VXMLGeometryGenerator::TG4VXMLGeometryGenerator()
  : fConvertor(0),
    fVolumeNames(),
    fOutFile(),
    fFileName(fgkUndefinedFileName),
    fVerboseLevel(1), 
    fMessenger(this, ""),
    fPositions(),
    fRotations(),
    fElements(),
    fMaterials()
{
//
// 
  G4cerr << "   TG4VXMLGeometryGenerator::TG4VXMLGeometryGenerator:" << G4endl;
  G4cerr << "   Not allowed constructor." << G4endl;
  G4cerr << "** Exception: Aborting execution **" << G4endl;   
  exit(1);
}

//_____________________________________________________________________________
TG4VXMLGeometryGenerator::TG4VXMLGeometryGenerator(
                                       const TG4VXMLGeometryGenerator& right) 
  : fConvertor(0),
    fVolumeNames(),
    fOutFile(),
    fFileName(fgkUndefinedFileName),
    fVerboseLevel(0),  
    fMessenger(this, ""),
    fPositions(),
    fRotations(),
    fElements(),
    fMaterials()
{
// 
  G4cerr << "   TG4VXMLGeometryGenerator::TG4VXMLGeometryGenerator:" << G4endl;
  G4cerr << "   Copy constructor not implemented." << G4endl;
  G4cerr << "** Exception: Aborting execution **" << G4endl;   
  exit(1);
}

//_____________________________________________________________________________
TG4VXMLGeometryGenerator::~TG4VXMLGeometryGenerator() {
//
  delete fConvertor;
}

//
// operators
//

//_____________________________________________________________________________
TG4VXMLGeometryGenerator& 
TG4VXMLGeometryGenerator::operator=(const TG4VXMLGeometryGenerator& right)
{
  // check assignement to self
  if (this == &right) return *this;

  G4cerr << "   TG4VXMLGeometryGenerator::operator=:" << G4endl;
  G4cerr << "   Assignement operator not implemented." << G4endl;
  G4cerr << "** Exception: Aborting execution **" << G4endl;   
  exit(1);
    
  return *this;  
}    
          

//
// private methods
//

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::CutName(G4String& name) const
{
// Removes spaces after the name if present.
// ---

  G4int i = name.length();
  while (name(--i) == ' ') name = name(0,i);
}  

//_____________________________________________________________________________
G4double TG4VXMLGeometryGenerator::Round(G4double number) const
{
// Rounds the position elements to the numeric precision of the
// convertor.

  G4double precision = fConvertor->GetNumPrecision();
  return fConvertor->Round(number*pow(10.,precision))/pow(10.,precision);
}

//_____________________________________________________________________________
G4String TG4VXMLGeometryGenerator::AddPositionToMap(G4ThreeVector position)
{
// Check if the specified position is not yet present (within the precision
// of the convertor) and add it to the map.
// Returns the XML name of the position element (if added)
// or empty string (if not added).
// ---

  G4double x = position.x()/ TG4XMLUnits::Length();
  G4double y = position.y()/ TG4XMLUnits::Length();
  G4double z = position.z()/ TG4XMLUnits::Length();
  G4ThreeVector roundedPosition(Round(x), Round(y), Round(z));
  
  if (fPositions.find(roundedPosition) != fPositions.end()) return G4String();
  
  // Generate position name
  //
  G4String name("pos_");

#if __GNUC__ >= 3
  std::ostringstream tmpStream;
#else  
  std::strstream tmpStream;
#endif
  tmpStream << fPositions.size();  
  name.append(tmpStream.str());

  // Add position to the map
  fPositions[roundedPosition] = name;
  
  return name;  
}    


//_____________________________________________________________________________
G4String 
TG4VXMLGeometryGenerator::AddRotationToMap(const G4RotationMatrix* rotation)
{
// Check if the specified rotation matrix is not yet present and 
// add it to the map.
// Returns the XML name of the rotation element (if added)
// or empty string (if not added).
// ---

  G4double phi   = rotation->getPhi()  / TG4XMLUnits::Angle();
  G4double theta = rotation->getTheta()/ TG4XMLUnits::Angle();
  G4double psi   = rotation->getPsi()  / TG4XMLUnits::Angle();
  G4ThreeVector roundedAngles(Round(phi), Round(theta), Round(psi));
  
  if (fRotations.find(roundedAngles) != fRotations.end()) return G4String();
  
  // Generate position name
  //
  G4String name("rot_");

#if __GNUC__ >= 3
  std::ostringstream tmpStream;
#else  
  std::strstream tmpStream;
#endif
  tmpStream << fRotations.size();  
  name.append(tmpStream.str());

  // Add position to the map
  fRotations[roundedAngles] = name;
  
  return name;  
}    

//_____________________________________________________________________________
const G4Element* 
TG4VXMLGeometryGenerator::AddElementToMap(const G4Element* element)
{
// Check if the specified element is not yet present (within the precision
// of the convertor) and add it to the map.
// Returns the element (if added) or 0.
// ---

  G4double x = element->GetZ();
  G4double y = element->GetN();
  G4double z = element->GetA();
  G4ThreeVector roundedValues(Round(x), Round(y), Round(z));
  
  //if (fElements.find(roundedValues) != fElements.end()) return 0;
  ElementMap::iterator it;
  for (it=fElements.begin(); it != fElements.end(); it++) {
    if ( (*it).first  == roundedValues &&
         (*it).second->GetName() == element->GetName() ) return 0; 
  }      
  
  // Add element to the map
  //fElements[roundedValues] = element;
  fElements.insert(std::make_pair(roundedValues, element));
 
  return element;  
}    

//_____________________________________________________________________________
const G4Material* 
TG4VXMLGeometryGenerator::AddMaterialToMap(const G4Material* material)
{
// Check if the specified position is not yet present (within the precision
// of the convertor) and add it to the map.
// Returns the XML name of the position element (if added)
// or empty string (if not added).
// ---

  G4String name = material->GetName();
  CutName(name);

  if (fMaterials.find(name) != fMaterials.end()) return 0;
  
  // Add material to the map
  fMaterials[name] = material;
  
  return material;  
}    


//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::ProcessPositions(G4LogicalVolume* lv) 
{
// Writes all position in the daughters tree of the given logical volume.
// ---

  G4String lvName = lv->GetName();

  // store the name of logical volume in the set
  fVolumeNames.insert(fVolumeNames.begin(), lvName); 
  
  G4int nofDaughters = lv->GetNoDaughters();

  if (nofDaughters>0) {
    G4int i; 
    for (i=0; i<nofDaughters; i++) {
      
      G4VPhysicalVolume* pvd = lv->GetDaughter(i);
 
      G4ThreeVector position = pvd->GetTranslation();
      G4String posName = AddPositionToMap(position);
      
      if (posName != G4String())
        fConvertor->WritePosition(posName, position);

      G4String lvdName = pvd->GetLogicalVolume()->GetName();
      if (fVolumeNames.find(lvdName) == fVolumeNames.end()) {
        // process lvd only if it was not yet processed
        ProcessPositions(pvd->GetLogicalVolume());
      }	
    }
  }  
}  

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::ProcessRotations(G4LogicalVolume* lv) 
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

      const G4RotationMatrix* kRotation = pvd->GetObjectRotation();
      if (kRotation) {
        G4String rotName = AddRotationToMap(kRotation);

        if (rotName != G4String())
          fConvertor->WriteRotation(rotName, kRotation);
      }

      G4String lvdName = pvd->GetLogicalVolume()->GetName();
      if (fVolumeNames.find(lvdName) == fVolumeNames.end()) {
        // process lvd only if it was not yet processed
        ProcessRotations(pvd->GetLogicalVolume());
      }	
    }
  }  
}  

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::ProcessMaterials(G4LogicalVolume* lv) 
{
// Writes all position in the daughters tree of the given logical volume.
// ---

  G4String lvName = lv->GetName();

  const G4Material* material 
    = AddMaterialToMap(lv->GetMaterial());
	
  if (material) {
    // process material elements
    for (G4int j=0; j<G4int(material->GetNumberOfElements()); j++) 
    AddElementToMap(material->GetElement(j));
  }	

  // store the name of logical volume in the set
  fVolumeNames.insert(fVolumeNames.begin(), lvName); 
  
  G4int nofDaughters = lv->GetNoDaughters();

  if (nofDaughters>0) {
    G4int i; 
    for (i=0; i<nofDaughters; i++) {
      
      G4VPhysicalVolume* pvd = lv->GetDaughter(i);

/*
      const G4Material* material 
        = AddMaterialToMap(pvd->GetLogicalVolume()->GetMaterial());
	
      if (material) {
        // process material elements
	for (G4int j=0; j<G4int(material->GetNumberOfElements()); j++) 
	  AddElementToMap(material->GetElement(j));
      }	
*/
      G4String lvdName = pvd->GetLogicalVolume()->GetName();
      if (fVolumeNames.find(lvdName) == fVolumeNames.end()) {
        // process lvd only if it was not yet processed
        ProcessMaterials(pvd->GetLogicalVolume());
      }	
    }
  }  
}  

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::ProcessSolids(G4LogicalVolume* lv) 
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

//
// protected methods
//

//_____________________________________________________________________________
G4String 
TG4VXMLGeometryGenerator::FindPositionName(G4ThreeVector position) const
{
// Finds the specified position in the map (within the precision of the 
// convertor) and returns its xml name.
// Returns empty string if not found.
// ---

  G4double x = position.x()/ TG4XMLUnits::Length();
  G4double y = position.y()/ TG4XMLUnits::Length();
  G4double z = position.z()/ TG4XMLUnits::Length();
  G4ThreeVector roundedPosition(Round(x), Round(y), Round(z));
  
  ThreeVectorMap::const_iterator it = fPositions.find(roundedPosition);    
  if (it != fPositions.end())
    return (*it).second;
  else
    return G4String();
}      

//_____________________________________________________________________________
G4String  
TG4VXMLGeometryGenerator::FindRotationName(const G4RotationMatrix* rotation) const
{
// Finds the rotation in the map and returns its xml name.
// Returns empty string if not found.
// ---

  G4double phi   = rotation->getPhi()  / TG4XMLUnits::Angle();
  G4double theta = rotation->getTheta()/ TG4XMLUnits::Angle();
  G4double psi   = rotation->getPsi()  / TG4XMLUnits::Angle();
  G4ThreeVector roundedAngles(Round(phi), Round(theta), Round(psi));
  
  ThreeVectorMap::const_iterator it = fRotations.find(roundedAngles);
  if (it != fRotations.end())
    return (*it).second;
  else
    return G4String();
}      

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::GeneratePositions(G4LogicalVolume* lv)
{
// Generates the XML elements containing
// all positions present in given logical volume daughters tree.
// ---

  // Open section
  fConvertor->OpenPositions();  
  
  // Store first the center position
  G4String posName = AddPositionToMap(G4ThreeVector());
  fConvertor->WritePosition(posName, G4ThreeVector());

  // Process positions
  ProcessPositions(lv);
  ClearVolumeNames();

  // Close section
  fConvertor->ClosePositions();
  fConvertor->WriteEmptyLine();
}   

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::GenerateRotations(G4LogicalVolume* lv)
{
// Generates the XML elements containing
// all positions present in given logical volume daughters tree.
// ---

  // Open section
  fConvertor->OpenRotations();  
  
  // Store first the identity matrix
  G4String rotName = AddRotationToMap(&HepRotation::IDENTITY);
  fConvertor->WriteRotation(rotName, &HepRotation::IDENTITY);

  // Process rotations
  ProcessRotations(lv);
  ClearVolumeNames();

  // Close section
  fConvertor->CloseRotations();
  fConvertor->WriteEmptyLine();
}   

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::GenerateMaterials(G4LogicalVolume* lv)
{
// Generates the XML material element containing
// all materials present in given logical volume.
// ---

  // Create section
  fConvertor->OpenMaterials();  
  
  // Fill maps of elements and materials
  ProcessMaterials(lv);
  
  // Write elements from the map
  ElementMap::const_iterator it1;
  for (it1 = fElements.begin(); it1 != fElements.end(); it1++)
    fConvertor->WriteElement((*it1).second);

  // Empty line
  fConvertor->WriteEmptyLine();
  
  // Write materials from the map
  MaterialMap::const_iterator it2;
  for (it2 = fMaterials.begin(); it2 != fMaterials.end(); it2++)
    fConvertor->WriteMaterial((*it2).second);
  
  fConvertor->WriteEmptyLine();
  fElements.erase(fElements.begin(), fElements.end());
  fMaterials.erase(fMaterials.begin(), fMaterials.end());
  ClearVolumeNames();

  // Close section
  fConvertor->CloseMaterials();
  fConvertor->WriteEmptyLine();
}   

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::GenerateSolids(G4LogicalVolume* lv)
{
// Generates the XML solids element containing
// all solids present in given logical volume.
// ---

  fConvertor->OpenSolids();  
  ProcessSolids(lv);
  fConvertor->CloseSolids();  
  fConvertor->WriteEmptyLine();
  ClearVolumeNames();
}   

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::OpenFile(G4String filePath)
{ 
// Opens output file.
// ---

  fOutFile.open(filePath, std::ios::out); 
  
  if (!fOutFile) {
    G4cerr << "   Cannot open " << filePath << G4endl;  
    G4cerr << "** Exception: Aborting execution **" << G4endl;   
    exit(1);
  }
  
  // use FORTRAN compatibility output
  fOutFile.setf(std::ios::fixed, std::ios::floatfield);
}


//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::CloseFile()
{ 
// Closes output file.
// ---

  fOutFile.close(); 
}

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::ClearVolumeNames() 
{
// Clears the set of volume names.
// ---

  fVolumeNames.erase(fVolumeNames.begin(), fVolumeNames.end());
}  

//
// public methods
//

//_____________________________________________________________________________
void TG4VXMLGeometryGenerator::GenerateXMLGeometry()
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
void TG4VXMLGeometryGenerator::GenerateXMLGeometry(const G4String& lvName) 
{
// Generates XML geometry file from the logical volume specified by name.
// ---

  // Find volume
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    if (lv->GetName() == lvName) {

      // Generate XML
      GenerateXMLGeometry(lv);
      
      // Clear temprary maps
      fPositions.erase(fPositions.begin(), fPositions.end());
      fRotations.erase(fRotations.begin(), fRotations.end());
      
      return;
    }  
  }
  
  G4cerr << "++ Warning: ++ " << G4endl;
  G4cerr << "   TG4VXMLGeometryGenerator::GenerateXMLGeometry:" << G4endl; 
  G4cerr << "   Logical volume " << lvName << " does not exist." << G4endl;
}  

