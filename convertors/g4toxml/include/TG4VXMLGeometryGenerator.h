// $Id: TG4VXMLGeometryGenerator.h,v 1.2 2003/12/18 13:25:11 brun Exp $
//
// Author: I. Hrivnacova, 27.07.2000 
// (redesigned  31.03.2004) 
//
// Class TG4VXMLGeometryGenerator
// ------------------------------
// Class for generation of geometry data files in XML,
// the XML format is independent from G4 geometry object model.
// It has three abstract methods (for processing the volume tree)
// that have to be provided by derived classes.

#ifndef TG4_V_XML_GEOMETRY_GENERATOR_H
#define TG4_V_XML_GEOMETRY_GENERATOR_H

#include <fstream>
#include <set>
#include <map>

#include <globals.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4Element.hh>
#include <G4Material.hh>

#include "TG4XMLMessenger.h"

class G4LogicalVolume;

class TG4VXMLConvertor;

class TG4VXMLGeometryGenerator
{
  public:
    typedef std::set <G4String, std::less<G4String> > StringSet; 
    typedef std::map <G4ThreeVector, G4String, 
                      std::less<G4ThreeVector> >      ThreeVectorMap; 
    typedef std::multimap <G4ThreeVector, const G4Element*, 
                      std::less<G4ThreeVector> >      ElementMap; 
    typedef std::map <G4String, const G4Material*, 
                      std::less<G4String> >           MaterialMap; 

  public:
    TG4VXMLGeometryGenerator(const G4String& xmlFormat);
    // --> protected
    // TG4VXMLGeometryGenerator();
    // TG4VXMLGeometryGenerator(const TG4VXMLGeometryGenerator& right);
    virtual ~TG4VXMLGeometryGenerator();

    // static access method
    // static TG4VXMLGeometryGenerator* Instance();

    // methods
    void GenerateXMLGeometry();
    void GenerateXMLGeometry(const G4String& lvName);

    // set methods
    void SetFileName(const G4String& fileName);
    void SetVerboseLevel(G4int verboseLevel);

    // get methods
    G4String GetFileName() const;
    G4int    GetVerboseLevel() const;

  protected:
    TG4VXMLGeometryGenerator();
    TG4VXMLGeometryGenerator(const TG4VXMLGeometryGenerator& right);

    // operators
    TG4VXMLGeometryGenerator& operator=(const TG4VXMLGeometryGenerator& right);

    // abstract methods
    // specific to XML definition
    //
    virtual void GenerateXMLGeometry(G4LogicalVolume* lv) = 0;
    virtual void GenerateSection(G4LogicalVolume* lv) = 0;
    virtual void ProcessLogicalVolume(G4LogicalVolume* lv) = 0;  

    // methods
    //
    G4String  FindPositionName(G4ThreeVector position) const;
    G4String  FindRotationName(const G4RotationMatrix* rotation) const;

    void GeneratePositions(G4LogicalVolume* lv);
    void GenerateRotations(G4LogicalVolume* lv);
    void GenerateMaterials(G4LogicalVolume* lv);
    void GenerateSolids(G4LogicalVolume* lv);

    void OpenFile(G4String filePath);
    void CloseFile();
    void ClearVolumeNames();

    // static data members
    static const G4String fgkUndefinedFileName; //default value of file name
                                     
    // data members
    //
    TG4VXMLConvertor*  fConvertor;   //interface to XML convertor 
    StringSet          fVolumeNames; //set of volume names
    std::ofstream      fOutFile;     //output file
    G4String           fFileName;    //output file name
    G4int              fVerboseLevel;//level of verbosity

  private:
    // methods
    //
    void CutName(G4String& name) const;
    G4double Round(G4double number) const;
 
    G4String  AddPositionToMap(G4ThreeVector position);
    G4String  AddRotationToMap(const G4RotationMatrix* rotation);
    const G4Element*  AddElementToMap(const G4Element* element);
    const G4Material* AddMaterialToMap(const G4Material* material);
    
    void ProcessPositions(G4LogicalVolume* lv); 
    void ProcessRotations(G4LogicalVolume* lv); 
    void ProcessMaterials(G4LogicalVolume* lv); 
    void ProcessSolids(G4LogicalVolume* lv); 

    // data members
    //
    TG4XMLMessenger    fMessenger; //messenger
    ThreeVectorMap     fPositions; //map between positions and their XML names
    ThreeVectorMap     fRotations; //map between rotations and their XML names
    ElementMap         fElements;  //map of elements
    MaterialMap        fMaterials; //map of materials
};


// inline methods

inline void TG4VXMLGeometryGenerator::SetFileName(const G4String& fileName)
{ fFileName = fileName; }

inline void TG4VXMLGeometryGenerator::SetVerboseLevel(G4int verboseLevel)
{ fVerboseLevel = verboseLevel; }

inline G4String TG4VXMLGeometryGenerator::GetFileName() const
{ return fFileName; }

inline G4int TG4VXMLGeometryGenerator::GetVerboseLevel() const
{ return fVerboseLevel; }

#endif //TG4_V_XML_GEOMETRY_GENERATOR_H

