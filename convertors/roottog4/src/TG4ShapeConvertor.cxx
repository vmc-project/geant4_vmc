// $Id: TG4ShapeConvertor.cxx,v 1.1 2003/07/22 06:46:58 brun Exp $
//
// Author: I. Hrivnacova, 8.1.2003 
//
// Class TG4ShapeConvertor
// -----------------------
// See the class description in the header file.

#include <TGeoShape.h>
#include <TGeoBBox.h>
#include <TGeoTube.h>
#include <TGeoCone.h>
#include <TGeoTrd1.h>
#include <TGeoTrd2.h>
#include <TGeoArb8.h>
#include <TGeoPara.h>
#include <TGeoPcon.h>
#include <TGeoPgon.h>
#include <TGeoEltu.h>
#include <TGeoSphere.h>
#include <TGeoCompositeShape.h>

#include <globals.hh>
#include <G4VSolid.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Cons.hh>
#include <G4Trd.hh>
#include <G4Trap.hh>
#include <G4Para.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>
#include <G4EllipticalTube.hh>
#include <G4Sphere.hh>
#include <G4BooleanSolid.hh>
#include <G4ThreeVector.hh>

#include "TG4ShapeConvertor.h"
#include "TG4TGeoUnits.h"

//_____________________________________________________________________________
TG4ShapeConvertor::TG4ShapeConvertor() {
//
}

//_____________________________________________________________________________
TG4ShapeConvertor::TG4ShapeConvertor(const TG4ShapeConvertor& right) 
{
// 
  G4Exception("G4RootGeometryConvertor: Copy constructor not implemented");
}

//_____________________________________________________________________________
TG4ShapeConvertor::~TG4ShapeConvertor() {
//
}

//
// private methods
//

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateBox(const TGeoBBox* box) const
{
// Converts box
// ---

  // get values
  G4String name(box->GetName());
  G4double hx = box->GetDX();
  G4double hy = box->GetDY();
  G4double hz = box->GetDZ();

  // convert units
  hx *= TG4TGeoUnits::Length();
  hy *= TG4TGeoUnits::Length();
  hz *= TG4TGeoUnits::Length();

  return new G4Box(name, hx, hy, hz);
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateTube(const TGeoTube* tube) const
{
// Converts tube
// ---

  // get values
  G4String name(tube->GetName());
  G4double rmin = tube->GetRmin();
  G4double rmax = tube->GetRmax();
  G4double dz   = tube->GetDz();
  G4double sphi = 0.*deg;
  G4double dphi = 360.*deg;

  // convert units
  rmin *= TG4TGeoUnits::Length();
  rmax *= TG4TGeoUnits::Length();
  dz   *= TG4TGeoUnits::Length();
  
  return new G4Tubs(name, rmin, rmax, dz, sphi, dphi);;
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateTubs(const TGeoTubeSeg* tubs) const
{
// Converts tubs
// ---

  // get values
  G4String name(tubs->GetName());
  G4double rmin = tubs->GetRmin();
  G4double rmax = tubs->GetRmax();
  G4double dz   = tubs->GetDz();
  G4double sphi = tubs->GetPhi1();
  G4double dphi = tubs->GetPhi2() - sphi;

   // convert units
  rmin *= TG4TGeoUnits::Length();
  rmax *= TG4TGeoUnits::Length();
  dz   *= TG4TGeoUnits::Length();
  sphi *= TG4TGeoUnits::Angle();
  dphi *= TG4TGeoUnits::Angle();
 
  if (dphi < 0) dphi += 360.*deg;

  return new G4Tubs(name, rmin, rmax, dz, sphi, dphi);;
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateCone(const TGeoCone* cone) const
{
// Converts cone
// ---

  // get values
  G4String name(cone->GetName());
  G4double dz    = cone->GetDz();
  G4double rmin1 = cone->GetRmin1(); 
  G4double rmax1 = cone->GetRmax1(); 
  G4double rmin2 = cone->GetRmin2(); 
  G4double rmax2 = cone->GetRmax2(); 
  G4double sphi = 0.*deg;
  G4double dphi = 360.*deg;

  // convert units
  dz    *= TG4TGeoUnits::Length();
  rmin1 *= TG4TGeoUnits::Length();
  rmax1 *= TG4TGeoUnits::Length();
  rmin2 *= TG4TGeoUnits::Length();
  rmax2 *= TG4TGeoUnits::Length();

  return new G4Cons(name, rmin1, rmax1, rmin2, rmax2, dz, sphi, dphi);
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateCons(const TGeoConeSeg* cons) const
{
// Converts cons
// ---

  // get values
  G4String name(cons->GetName());
  G4double dz    = cons->GetDz();
  G4double rmin1 = cons->GetRmin1(); 
  G4double rmax1 = cons->GetRmax1(); 
  G4double rmin2 = cons->GetRmin2(); 
  G4double rmax2 = cons->GetRmax2(); 
  G4double sphi = cons->GetPhi1();
  G4double dphi = cons->GetPhi2() - sphi;

  // convert units
  dz    *= TG4TGeoUnits::Length();
  rmin1 *= TG4TGeoUnits::Length();
  rmax1 *= TG4TGeoUnits::Length();
  rmin2 *= TG4TGeoUnits::Length();
  rmax2 *= TG4TGeoUnits::Length();
  sphi  *= TG4TGeoUnits::Angle();
  dphi  *= TG4TGeoUnits::Angle();
 
  if (dphi < 0) dphi += 360.*deg;

  return new G4Cons(name, rmin1, rmax1, rmin2, rmax2, dz, sphi, dphi);
 }

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateTrd(const TGeoTrd1* trd1) const
{
// Converts trd1
// ---

  // get values
  G4String name(trd1->GetName());
  G4double dx1 = trd1->GetDx1();
  G4double dx2 = trd1->GetDx2();
  G4double dy1 = trd1->GetDy();
  G4double dy2 = dy1;
  G4double dz  = trd1->GetDz();

  // convert units
  dx1 *= TG4TGeoUnits::Length();
  dx2 *= TG4TGeoUnits::Length();
  dy1 *= TG4TGeoUnits::Length();
  dy2 *= TG4TGeoUnits::Length();
  dz  *= TG4TGeoUnits::Length();
  
  return new G4Trd(name, dx1, dx2, dy1, dy2, dz);
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateTrd(const TGeoTrd2* trd2) const
{
// Converts trd2
// ---

  // get values
  G4String name(trd2->GetName());
  G4double dx1 = trd2->GetDx1();
  G4double dx2 = trd2->GetDx2();
  G4double dy1 = trd2->GetDy1();
  G4double dy2 = trd2->GetDy2();
  G4double dz  = trd2->GetDz();

  // convert units
  dx1 *= TG4TGeoUnits::Length();
  dx2 *= TG4TGeoUnits::Length();
  dy1 *= TG4TGeoUnits::Length();
  dy2 *= TG4TGeoUnits::Length();
  dz  *= TG4TGeoUnits::Length();
  
  return new G4Trd(name, dx1, dx2, dy1, dy2, dz);
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateArb8(const TGeoArb8* arb8) const
{
// Converts arb8
// ---

  // get values
  G4String name(arb8->GetName());
  G4double  dz = arb8->GetDz();
  G4double* xy = (const_cast<TGeoArb8*>(arb8))->GetVertices();
  
  G4ThreeVector pt[8];
  for (G4int i=0; i<8; i++) {
     if (i<4)
       pt[i] = G4ThreeVector(xy[2*i], xy[2*i+1], -dz);
     else      
       pt[i] = G4ThreeVector(xy[2*i], xy[2*i+1],  dz);
 
     // convert units
     pt[i] *= TG4TGeoUnits::Length();
     
     // G4cout << i << "th vertex  " << pt[i] << G4endl;
  }
  
  return new G4Trap(name, pt);
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateTrap(const TGeoTrap* trap) const
{
// Converts trap
// ---

  // get values
  G4String name(trap->GetName());
  G4double dz    = trap->GetDz();
  G4double theta = trap->GetTheta();
  G4double phi   = trap->GetPhi();
  G4double dy1   = trap->GetH1();
  G4double dx1   = trap->GetBl1();
  G4double dx2   = trap->GetTl1();
  G4double alph1 = trap->GetAlpha1();
  G4double dy2   = trap->GetH2();
  G4double dx3   = trap->GetBl2();
  G4double dx4   = trap->GetTl2();
  G4double alph2 = trap->GetAlpha2();

  // convert units
  dz    *= TG4TGeoUnits::Length();
  theta *= TG4TGeoUnits::Angle();
  phi   *= TG4TGeoUnits::Angle();
  dy1   *= TG4TGeoUnits::Length();
  dx1   *= TG4TGeoUnits::Length();
  dx2   *= TG4TGeoUnits::Length();
  alph1 *= TG4TGeoUnits::Angle();
  dy2   *= TG4TGeoUnits::Length();
  dx3   *= TG4TGeoUnits::Length();
  dx4   *= TG4TGeoUnits::Length();
  alph2 *= TG4TGeoUnits::Angle();

  return new G4Trap(name, dz, theta, phi, 
                    dy1, dx1, dx2, alph1, dy2, dx3, dx4, alph2);
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreatePara(const TGeoPara* para) const
{
// Converts para
// ---

  // get values
  G4String name(para->GetName());
  G4double dx = para->GetX();
  G4double dy = para->GetY();
  G4double dz = para->GetZ();
  G4double alpha = para->GetAlpha();
  G4double theta = para->GetTheta();
  G4double phi   = para->GetPhi();

  // convert units
  dx    *= TG4TGeoUnits::Length();
  dy    *= TG4TGeoUnits::Length();
  dz    *= TG4TGeoUnits::Length();
  alpha *= TG4TGeoUnits::Angle();
  theta *= TG4TGeoUnits::Angle();
  phi   *= TG4TGeoUnits::Angle();

  return new G4Para(name, dx, dy, dz, alpha, theta, phi);
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreatePolycone(const TGeoPcon* pcon) const
{
// Converts pcon
// ---

  // get values
  G4String name(pcon->GetName());
  G4double  sphi = pcon->GetPhi1();
  G4double  dphi = pcon->GetDphi();
  G4int     nz   = pcon->GetNz();
  G4double* dz   = pcon->GetZ();
  G4double* rmin = pcon->GetRmin();
  G4double* rmax = pcon->GetRmax();
  
  // convert units
  for (G4int i=0; i<nz; i++) {
    dz[i]   *= TG4TGeoUnits::Length();
    rmin[i] *= TG4TGeoUnits::Length();
    rmax[i] *= TG4TGeoUnits::Length();
  }  
  sphi  *= TG4TGeoUnits::Angle();
  dphi  *= TG4TGeoUnits::Angle();

  G4Polycone* g4Polycone
    = new G4Polycone(name, sphi, dphi, nz, dz, rmin, rmax);
    
  // put back original parameters
  for (G4int j=0; j<nz; j++) {
    dz[j]   /= TG4TGeoUnits::Length();
    rmin[j] /= TG4TGeoUnits::Length();
    rmax[j] /= TG4TGeoUnits::Length();
  }  

  return g4Polycone;
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreatePolyhedra(const TGeoPgon* pgon) const
{
// Converts pgon
// ---

  // get values
  G4String name(pgon->GetName());
  G4double  sphi = pgon->GetPhi1();
  G4double  dphi = pgon->GetDphi();
  G4int     nz   = pgon->GetNz();
  G4int     ns   = pgon->GetNedges();
  G4double* dz   = pgon->GetZ();
  G4double* rmin = pgon->GetRmin();
  G4double* rmax = pgon->GetRmax();
  
  // convert units
  for (G4int i=0; i<nz; i++) {
    dz[i]   *= TG4TGeoUnits::Length();
    rmin[i] *= TG4TGeoUnits::Length();
    rmax[i] *= TG4TGeoUnits::Length();
  }  
  sphi  *= TG4TGeoUnits::Angle();
  dphi  *= TG4TGeoUnits::Angle();

  G4Polyhedra* g4Polyhedra
    = new G4Polyhedra(name, sphi, dphi, ns, nz, dz, rmin, rmax);
    
  // put back original parameters
  for (G4int j=0; j<nz; j++) {
    dz[j]   /= TG4TGeoUnits::Length();
    rmin[j] /= TG4TGeoUnits::Length();
    rmax[j] /= TG4TGeoUnits::Length();
  }  

  return g4Polyhedra;   
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateEllipticalTube(const TGeoEltu* eltu) const
{
// Converts eltu
// ---

  // get values
  G4String name(eltu->GetName());
  G4double dx = eltu->GetA();
  G4double dy = eltu->GetB();
  G4double dz = eltu->GetDz();

  // convert units
  dx *= TG4TGeoUnits::Length();
  dy *= TG4TGeoUnits::Length();
  dz *= TG4TGeoUnits::Length();
  
  return new G4EllipticalTube(name, dx, dx, dz);
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateSphere(const TGeoSphere* sphere) const
{
// Converts sphere
// ---

  // get values
  G4String name(sphere->GetName());
  G4double rmin = sphere->GetRmin();
  G4double rmax = sphere->GetRmax();
  G4double the1 = sphere->GetTheta1();
  G4double the2 = sphere->GetTheta2();
  G4double dthe = the2 - the1;
  G4double phi1 = sphere->GetPhi1();
  G4double phi2 = sphere->GetPhi2();
  G4double dphi = phi2 - phi1;
 
  // convert units
  rmin *= TG4TGeoUnits::Length();
  rmax *= TG4TGeoUnits::Length();
  the1 *= TG4TGeoUnits::Angle();
  the2 *= TG4TGeoUnits::Angle();
  dthe *= TG4TGeoUnits::Angle();
  phi1 *= TG4TGeoUnits::Angle();
  phi2 *= TG4TGeoUnits::Angle();
  dphi *= TG4TGeoUnits::Angle();

  return new G4Sphere(name, rmin, rmax, phi1, dphi, the1, dthe);
}

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::CreateBooleanSolid(
                                   const TGeoCompositeShape* comp) const
{
  G4Exception("G4ShapeConvertor: Not yet implemented");
  return 0;
}

//
// public methods
//

//_____________________________________________________________________________
G4VSolid* TG4ShapeConvertor::Convert(const TGeoShape* shape) const
{
// Converts TGeo shape into G4 solid
// ---

  const TGeoBBox* box = dynamic_cast<const TGeoBBox*>(shape);
  if (box && TString(shape->ClassName()) == TString("TGeoBBox")) { 
    return CreateBox(box); 
  }

  const TGeoTubeSeg* tubs = dynamic_cast<const TGeoTubeSeg*>(shape);
  if (tubs) {
    return CreateTubs(tubs);
  }  

  const TGeoTube* tube = dynamic_cast<const TGeoTube*>(shape);
  if (tube) {
    return CreateTube(tube);
  }  

  const TGeoConeSeg* cons = dynamic_cast<const TGeoConeSeg*>(shape);
  if (cons) {
    return CreateCons(cons);
  }  

  const TGeoCone* cone = dynamic_cast<const TGeoCone*>(shape);
  if (cone) {
    return CreateCone(cone);
  }  

  const TGeoTrd1* trd1 = dynamic_cast<const TGeoTrd1*>(shape);
  if (trd1) {
    return CreateTrd(trd1);
  }  

  const TGeoTrd2* trd2 = dynamic_cast<const TGeoTrd2*>(shape);
  if (trd2) {
    return CreateTrd(trd2);
  }  

  const TGeoTrap* trap = dynamic_cast<const TGeoTrap*>(shape);
  if (trap) {
    return CreateTrap(trap);
  }  

  const TGeoArb8* arb8 = dynamic_cast<const TGeoArb8*>(shape);
  if (arb8) {
    return CreateArb8(arb8);
  }  

  const TGeoPara* para = dynamic_cast<const TGeoPara*>(shape);
  if (para) {
    return CreatePara(para);
  }  

  const TGeoPgon* pgon = dynamic_cast<const TGeoPgon*>(shape);
  if (pgon) {
    return CreatePolyhedra(pgon);
  }  

  const TGeoPcon* pcon = dynamic_cast<const TGeoPcon*>(shape);
  if (pcon) {
    return CreatePolycone(pcon);
  }  

  const TGeoEltu* eltu = dynamic_cast<const TGeoEltu*>(shape);
  if (eltu) {
    return CreateEllipticalTube(eltu);
  }  

  const TGeoSphere* sphere = dynamic_cast<const TGeoSphere*>(shape);
  if (sphere) {
    return CreateSphere(sphere);
  }  

  const TGeoCompositeShape* comp 
    = dynamic_cast<const TGeoCompositeShape*>(shape);
  if (comp) {
    return CreateBooleanSolid(comp);
  }  

  G4Exception("TG4ShapeConvertor::Convert: unknown shape type.");
  return 0;
}

