// $Id: $
//
// Author: I. Hrivnacova, 8.1.2003
//
// Class TG4ShapeConvertor
// ------------------------------
// Class for converting Root shapes in G4 solids.

#ifndef TG4_SHAPE_CONVERTOR_H
#define TG4_SHAPE_CONVERTOR_H

class TGeoShape;
class TGeoBBox;
class TGeoTube;
class TGeoTubeSeg;
class TGeoCone;
class TGeoConeSeg;
class TGeoTrd1;
class TGeoTrd2;
class TGeoArb8;
class TGeoTrap;
class TGeoPara;
class TGeoPcon;
class TGeoPgon;
class TGeoEltu;
class TGeoSphere;
class TGeoCompositeShape;

class G4VSolid;

class TG4ShapeConvertor
{
  public:
    TG4ShapeConvertor();
    // --> protected
    // TG4ShapeConvertor(const TG4ShapeConvertor& right);
    virtual ~TG4ShapeConvertor();

    // methods
    G4VSolid* Convert(const TGeoShape* shape) const;
    
  protected:  
    TG4ShapeConvertor(const TG4ShapeConvertor& right);

  private:
    // methods
    G4VSolid* CreateBox(const TGeoBBox* box) const;
    G4VSolid* CreateTube(const TGeoTube* tube) const;
    G4VSolid* CreateTubs(const TGeoTubeSeg* tubs) const;
    G4VSolid* CreateCone(const TGeoCone* cone) const;
    G4VSolid* CreateCons(const TGeoConeSeg* cons) const;
    G4VSolid* CreateTrd(const TGeoTrd1* trd1) const;
    G4VSolid* CreateTrd(const TGeoTrd2* trd2) const;
    G4VSolid* CreateArb8(const TGeoArb8* arb8) const;
    G4VSolid* CreateTrap(const TGeoTrap* trap) const;
    G4VSolid* CreatePara(const TGeoPara* para) const;
    G4VSolid* CreatePolycone(const TGeoPcon* pcon) const;
    G4VSolid* CreatePolyhedra(const TGeoPgon* pgon) const;
    G4VSolid* CreateEllipticalTube(const TGeoEltu* eltu) const;
    G4VSolid* CreateSphere(const TGeoSphere* sphere) const;
    G4VSolid* CreateBooleanSolid(const TGeoCompositeShape* comp) const;
};

#endif //TG4_SHAPE_CONVERTOR_H

