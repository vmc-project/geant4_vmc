// $Id: $
//
// Author: I. Hrivnacova, 31.03.2004 
//
// Class TG4GDMLGeometryGenerator
// ------------------------------
// Class for generation of geometry data files in XML,
// in the GDML format.

#ifndef TG4_GDML_GEOMETRY_GENERATOR_H
#define TG4_GDML_GEOMETRY_GENERATOR_H

//#include <globals.hh>
//#include <fstream>
//#include <set>

#include "TG4VXMLGeometryGenerator.h"

//class TG4VXMLConvertor;

class G4LogicalVolume;

class TG4GDMLGeometryGenerator : public TG4VXMLGeometryGenerator
{
  public:
    TG4GDMLGeometryGenerator();
    // --> protected
    // TG4GDMLGeometryGenerator(const TG4GDMLGeometryGenerator& right);
    virtual ~TG4GDMLGeometryGenerator();

  protected:
    TG4GDMLGeometryGenerator(const TG4GDMLGeometryGenerator& right);

    // operators
    TG4GDMLGeometryGenerator& operator=(const TG4GDMLGeometryGenerator& right);

    // methods 
    // (specific to XML definition)
    //
    virtual void GenerateXMLGeometry(G4LogicalVolume* lv);
    virtual void GenerateSection(G4LogicalVolume* lv);
    virtual void ProcessLogicalVolume(G4LogicalVolume* lv); 
};

#endif //TG4_GDML_GEOMETRY_GENERATOR_H

