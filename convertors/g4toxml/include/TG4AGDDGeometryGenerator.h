// $Id: TG4AGDDGeometryGenerator.h,v 1.1 2004/04/26 17:05:04 brun Exp $
//
// Author: I. Hrivnacova, 27.07.2000 
// (redesigned  31.03.2004) 
//
// Class TG4AGDDGeometryGenerator
// ------------------------------
// Class for generation of geometry data files in XML,
// in the AGDD XML format.

#ifndef TG4_AGDD_GEOMETRY_GENERATOR_H
#define TG4_AGDD_GEOMETRY_GENERATOR_H

#include "TG4VXMLGeometryGenerator.h"

class G4LogicalVolume;

class TG4AGDDGeometryGenerator : public TG4VXMLGeometryGenerator
{
  public:
    TG4AGDDGeometryGenerator();
    // --> protected
    // TG4AGDDGeometryGenerator(const TG4AGDDGeometryGenerator& right);
    virtual ~TG4AGDDGeometryGenerator();

  protected:
    TG4AGDDGeometryGenerator(const TG4AGDDGeometryGenerator& right);

    // operators
    TG4AGDDGeometryGenerator& operator=(const TG4AGDDGeometryGenerator& right);

    // methods
    // (specific to XML definition)
    //
    virtual void GenerateXMLGeometry(G4LogicalVolume* lv);
    virtual void GenerateSection(G4LogicalVolume* lv);
    virtual void ProcessTopVolume(G4LogicalVolume* lv); 
    virtual void ProcessLogicalVolume(G4LogicalVolume* lv); 
};

#endif //TG4_AGDD_GEOMETRY_GENERATOR_H

