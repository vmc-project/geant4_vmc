// $Id: TG4RootGeometryConvertor.h,v 1.4 2004/03/26 11:03:36 brun Exp $
//
// Author: I. Hrivnacova, 8.1.2003
//
// Class TG4RootGeometryConvertor
// ------------------------------
// Class for converting Root geometry in G4 geometry.

#ifndef TG4_ROOT_GEOMETRY_CONVERTOR_H
#define TG4_ROOT_GEOMETRY_CONVERTOR_H

#include <map>

#include <globals.hh>
#include <geomdefs.hh>

class TGeoVolume;
class TGeoPatternFinder;
class TGeoMaterial;
class TGeoMedium;

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class TG4MaterialConvertor;
class TG4ShapeConvertor;

class TG4RootGeometryConvertor
{
  public:
    typedef std::map<const TGeoVolume*, G4LogicalVolume*,  
                     std::less<const TGeoVolume*> > VolumesMap;
    typedef VolumesMap::const_iterator  VolumesMapIterator;	       

    typedef std::map<const G4LogicalVolume*, const TGeoMedium*,   
                     std::less<const G4LogicalVolume*> > MediumMap;
    typedef MediumMap::const_iterator  MediumMapIterator;	       

  public:
    TG4RootGeometryConvertor();
    // --> protected
    // TG4RootGeometryConvertor(const TG4RootGeometryConvertor& right);
    virtual ~TG4RootGeometryConvertor();

    // methods
    G4VPhysicalVolume* Convert(const TGeoVolume* world);
    
    // set methods
    void SetSeparator(char separator);

    // get methods
    char  GetSeparator() const;
    const TGeoMedium* GetMedium(const G4LogicalVolume* lv) const;   
    const G4Material* GetMaterial(const TGeoMaterial*) const;
   
  protected:  
    TG4RootGeometryConvertor(const TG4RootGeometryConvertor& right);

  private:  
    // methods
    void Append(G4String& string, G4int number) const;
    void SetUniqueName(G4LogicalVolume* lv, G4int number) const;
    EAxis GetAxis(const TGeoPatternFinder* finder) const;
    Int_t GetIAxis(EAxis axis) const;
    Bool_t IsDivided(const TGeoVolume* volume);
    G4LogicalVolume* CreateLV(const TGeoVolume* volume);
    void CreatePlacements(const TGeoVolume* volume, G4LogicalVolume* mLV);
    void CreateDivision(const TGeoVolume* volume, G4LogicalVolume* mLV);
    
    void SetUniqueNames();
    void ProcessDaughters(G4LogicalVolume* motherLV, const TGeoVolume* mother);
    void ProcessPositions();
    
    // static data members
    static const char  fgDefaultSeparator;

    // data members
    TG4MaterialConvertor* fMaterialConvertor;
    TG4ShapeConvertor*    fShapeConvertor;
    VolumesMap            fVolumesMap;
    MediumMap             fMediumMap;
    char                  fSeparator;
};

// inline functions

inline void TG4RootGeometryConvertor::SetSeparator(char separator)
{ fSeparator = separator; }

inline char TG4RootGeometryConvertor::GetSeparator() const
{ return fSeparator; }

#endif //TG4_ROOT_GEOMETRY_CONVERTOR_H

