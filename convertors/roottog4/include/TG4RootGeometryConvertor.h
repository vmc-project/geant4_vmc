// $Id: $
//
// Author: I. Hrivnacova, 8.1.2003
//
// Class TG4RootGeometryConvertor
// ------------------------------
// Class for converting Root geometry in G4 geometry.

#ifndef TG4_ROOT_GEOMETRY_CONVERTOR_H
#define TG4_ROOT_GEOMETRY_CONVERTOR_H

#include "globals.hh"
#include "g4std/map"

class TGeoVolume;

class G4VPhysicalVolume;
class G4LogicalVolume;

class TG4MaterialConvertor;
class TG4ShapeConvertor;

class TG4RootGeometryConvertor
{
  public:
    typedef G4std::map<const TGeoVolume*, G4LogicalVolume*,  
                       G4std::less<const TGeoVolume*> > VolumesMap;
    typedef VolumesMap::const_iterator  VolumesMapIterator;	       

  public:
    TG4RootGeometryConvertor();
    // --> protected
    // TG4RootGeometryConvertor(const TG4RootGeometryConvertor& right);
    virtual ~TG4RootGeometryConvertor();

    // methods
    G4VPhysicalVolume* Convert(const TGeoVolume* world);
    
  protected:  
    TG4RootGeometryConvertor(const TG4RootGeometryConvertor& right);

  private:  
    // methods
    void Append(G4String& string, G4int number) const;
    void SetUniqueName(G4LogicalVolume* lv, G4int number) const;
    G4LogicalVolume* CreateLV(const TGeoVolume* volume);
    void ProcessDaughters(G4LogicalVolume* motherLV, const TGeoVolume* mother);
    void SetUniqueNames();
    void ProcessPositions();
    
    // static data members
    static const char  fgSeparator;

    // data members
    TG4MaterialConvertor* fMaterialConvertor;
    TG4ShapeConvertor*    fShapeConvertor;
    VolumesMap            fVolumesMap;
};

#endif //TG4_ROOT_GEOMETRY_CONVERTOR_H

