// $Id: $
//
// Author: I. Hrivnacova, 8.1.2003
//
// Class TG4MaterialConvertor
// ------------------------------
// Class for converting Root materials into G4 materials.

#ifndef TG4_MATERIAL_CONVERTOR_H
#define TG4_MATERIAL_CONVERTOR_H

#include "g4std/map"
#include <globals.hh>

class TGeoMaterial;
class TGeoMixture;
class TList;

class G4Material;

class TG4MaterialConvertor
{
  public:
    typedef G4std::map<const TGeoMaterial*, G4Material*,  
                       G4std::less<const TGeoMaterial*> > MaterialsMap;
    typedef MaterialsMap::const_iterator  MaterialsMapIterator;	       

  public:
    TG4MaterialConvertor();
    // --> protected
    // TG4MaterialConvertor(const TG4MaterialConvertor& right);
    virtual ~TG4MaterialConvertor();

    // methods
    void Convert(const TList* materials);
    
    // get methods
    G4Material* GetMaterial(TGeoMaterial* material) const;
    
  protected:  
    TG4MaterialConvertor(const TG4MaterialConvertor& right);

  private:  
    // methods
    G4Material* CreateMaterial(const TGeoMaterial* material);
    G4Material* CreateMixture(const TGeoMixture* mixture);

    // static data members
    static const G4double fgkG3MinimumDensity;

    // data members
    MaterialsMap  fMaterialsMap;
};

#endif //TG4_ROOT_GEOMETRY_CONVERTOR_H

