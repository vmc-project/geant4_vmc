// $Id: TG4XMLGeometryGenerator.h,v 1.1 2003/07/22 06:46:58 brun Exp $
// Category: geometry
//
// Author: I. Hrivnacova, 27.07.2000 
//
// Class TG4XMLGeometryGenerator
// -----------------------------
// Singleton class for generation of geometry data files in XML,
// the XML format is independent from G4 geometry
// object model. 

#ifndef TG4_XML_GEOMETRY_GENERATOR_H
#define TG4_XML_GEOMETRY_GENERATOR_H

#include <globals.hh>
#include <fstream>
#include <set>

#include "TG4XMLMessenger.h"

class TG4VXMLConvertor;

class G4LogicalVolume;

class TG4XMLGeometryGenerator
{
  public:
    typedef std::set <G4String, std::less<G4String> > StringSet; 

  public:
    TG4XMLGeometryGenerator();
    // --> protected
    // TG4XMLGeometryGenerator(const TG4XMLGeometryGenerator& right);
    virtual ~TG4XMLGeometryGenerator();

    // static access method
    static TG4XMLGeometryGenerator* Instance();

    // methods
    void GenerateXMLGeometry();
    void GenerateXMLGeometry(const G4String& lvName);
    void GenerateMaterials(const G4String& version, const G4String& date,
            const G4String& author,  const G4String dtdVersion,
	    G4LogicalVolume* lv);
    void GenerateSection(const G4String& versionDTD, const G4String& name, 
            const G4String& version, const G4String& date, 
	    const G4String& author, const G4String& topVolume,
	    G4LogicalVolume* lv);
    void OpenFile(G4String filePath);
    void CloseFile();
    void SetVerboseLevel(G4int verboseLevel);

  protected:
    TG4XMLGeometryGenerator(const TG4XMLGeometryGenerator& right);

    // operators
    TG4XMLGeometryGenerator& operator=(const TG4XMLGeometryGenerator& right);

  private:
    // methods
    void CutName(G4String& name) const;
    void GenerateXMLGeometry(G4LogicalVolume* lv);
    void ProcessLogicalVolume(G4LogicalVolume* lv); 
    void ProcessMaterials(G4LogicalVolume* lv); 
    void ProcessSolids(G4LogicalVolume* lv); 
    void ProcessRotations(G4LogicalVolume* lv); 
    void ClearMaterialNames();
    void ClearVolumeNames();

    // static data members
    static TG4XMLGeometryGenerator*  fgInstance;     //this instance

    // data members
    TG4XMLMessenger    fMessenger; //messenger
    TG4VXMLConvertor*  fConvertor; //interface to XML convertor 
    std::ofstream      fOutFile;   //output file
    StringSet          fMaterialNames;   //set of names of materials 
    StringSet          fVolumeNames;     //set of names of solids
    G4int              fVerboseLevel;    //level of verbosity
};


// inline methods
inline TG4XMLGeometryGenerator* TG4XMLGeometryGenerator::Instance()
{ return fgInstance; }

inline void TG4XMLGeometryGenerator::SetVerboseLevel(G4int verboseLevel)
{ fVerboseLevel = verboseLevel; }

#endif //TG4_XML_GEOMETRY_GENERATOR_H

