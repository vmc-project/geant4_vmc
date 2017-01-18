//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file ExGarfield/src/DetectorConstruction.cxx 
/// \brief Implementation of the ExGarfield::DetectorConstruction class 
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
/// 
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay
 
#include <Riostream.h>
#include <TGeoManager.h>
#include <TGeoElement.h>
#include <TGeoMaterial.h>
#include <TGeoBBox.h>
#include <TGeoTube.h>
#include <TGeoVolume.h>
#include <TVirtualMC.h>
#include <TList.h>
#include <TThread.h>

#include "DetectorConstruction.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(VMC::ExGarfield::DetectorConstruction)
/// \endcond

namespace VMC
{
namespace ExGarfield
{

//_____________________________________________________________________________
DetectorConstruction::DetectorConstruction()
  : TObject()
{
/// Default constuctor
}

//_____________________________________________________________________________
DetectorConstruction::~DetectorConstruction()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void DetectorConstruction::Construct()
{
  /// Construct geometry using TGeo modeller

  // Create Root geometry manager 
  new TGeoManager("Garfield_geometry", "ExGarfield VMC example geometry");

  // Elements
  //
  Double_t z, a;
  TGeoElement* elH  = new TGeoElement("Hydrogen", "H",  z =  1., a =   1.01);
  TGeoElement* elC  = new TGeoElement("Carbon"  , "C",  z =  6., a =  12.01);
  TGeoElement* elN  = new TGeoElement("Nitrogen", "N",  z =  7., a =  14.01);
  TGeoElement* elO  = new TGeoElement("Oxygen"  , "O",  z =  8., a =  16.00);
  TGeoElement* elAr = new TGeoElement("Argon"   , "Ar", z = 18., a =  39.94);
  //TGeoElement* elW  = new TGeoElement("Tungsten", "W",  z = 74., a = 183.84);
  //TGeoElement* elPb = new TGeoElement("Lead",     "Pb", z = 82., a = 207.22);

  // Materials
  //

  // Al
  Double_t density;
  TGeoMaterial* matAl
    = new TGeoMaterial("Al", a = 26.98, z = 13., density = 2.699);

  // W
  TGeoMaterial* matW
    = new TGeoMaterial("Tungsten", a = 183.85, z = 74., density = 19.30);

  // Pb
  TGeoMaterial* matPb
    = new TGeoMaterial("Lead", a = 207.22, z = 82., density = 11.35);

  // Air
  TGeoMixture* matAir
    = new TGeoMixture("AirA", 4, density=1.205e-03);
  matAir->AddElement(elC,  0.0001); 
  matAir->AddElement(elN,  0.7553); 
  matAir->AddElement(elO,  0.2318); 
  matAir->AddElement(elAr, 0.0128);
  
  // ArCO2_70_30
  TGeoMixture* matArCO2
    = new TGeoMixture("ArCO2", 3, density=1.822e-03);
  matArCO2->AddElement(elAr, 0.7000); 
  matArCO2->AddElement(elC,  0.0819); 
  matArCO2->AddElement(elO,  0.2181);

  // Kapton
  TGeoMixture* matKapton
    = new TGeoMixture("Kapton", 4, density=1.413);
  matKapton->AddElement(elO, 0.2092); 
  matKapton->AddElement(elC, 0.6911); 
  matKapton->AddElement(elN, 0.0733); 
  matKapton->AddElement(elH, 0.0264); 

  // Tracking medias (defaut parameters)
  //

  // Paremeters for tracking media  
  Double_t param[20];
  param[0] = 0;     // isvol  - Not used
  param[1] = 2;     // ifield - User defined magnetic field
  param[2] = 10.;   // fieldm - Maximum field value (in kiloGauss)
  param[3] = -20.;  // tmaxfd - Maximum angle due to field deflection 
  param[4] = -0.01; // stemax - Maximum displacement for multiple scat 
  param[5] = -.3;   // deemax - Maximum fractional energy loss, DLS 
  param[6] = .001;  // epsil - Tracking precision
  param[7] = -.8;   // stmin
  for ( Int_t i=8; i<20; ++i) param[i] = 0.;

  Int_t mediumId = 0;
  TGeoMedium* medAl = new TGeoMedium("Al", ++mediumId, matAl, param);
  TGeoMedium* medW  = new TGeoMedium("W",  ++mediumId, matW,  param);
  TGeoMedium* medPb = new TGeoMedium("Pb", ++mediumId, matPb, param);
  TGeoMedium* medAir = new TGeoMedium("AirA", ++mediumId, matAir, param); 
  TGeoMedium* medArCO2 = new TGeoMedium("ArCO2", ++mediumId, matArCO2, param);

  TGeoMedium* defaultMedium = medAir;
  TGeoMedium* absorberMedium = medPb;
  TGeoMedium* gasMedium = medArCO2;
  TGeoMedium* cathodeMedium = medAl;
  TGeoMedium* wireMedium = medW;
 
  // Volumes
  //

  // Geometry parameters
  Double_t mm = 0.1;   // milimeter -> cm
  Double_t um = 1.e-4; // micrometer -> cm
  Double_t worldSizeXYZ = 1000 * mm;
  Double_t absorberThicknessZ = 10. * mm;
  Double_t absorberThicknessXY = 100. * mm;
  Double_t wireRadius = 0.025 * mm;
  Double_t tubeRadius = 15 * mm;
  Double_t tubeHalfLength = 100 * mm;
  Double_t tubeThickness = 500 * um;

  //
  // World
  //
  TGeoShape* worldS 
    = new TGeoBBox("World",             // its name
                   0.5 * worldSizeXYZ, 
                   0.5 * worldSizeXYZ, 
                   0.5 * worldSizeXYZ); // its size

  TGeoVolume* worldLV 
    = new TGeoVolume("World", worldS, defaultMedium); 

  gGeoManager->SetTopVolume(worldLV);

  //
  // Absorber
  //
  TGeoShape* absorberS 
    = new TGeoBBox(
           "Absorber",            // its name
            0.5 * absorberThicknessXY, 0.5 * absorberThicknessXY,
            0.5 * absorberThicknessZ); // its size

  TGeoVolume* absorberLV 
    = new TGeoVolume("Absorber", absorberS, absorberMedium); 

  Double_t xpos = 0;
  Double_t ypos = 0;
  Double_t zpos = absorberThicknessZ / 2;
  worldLV->AddNode(absorberLV, 1, new TGeoTranslation(xpos, ypos, zpos));

  //
  // Drift tube
  //
  TGeoShape* tubeS 
    = new TGeoTube("Tube",  0, tubeRadius, tubeHalfLength + tubeThickness);

  TGeoVolume* tubeLV 
    = new TGeoVolume( "Tube", tubeS, cathodeMedium); 

  TGeoRotation* rotY = new TGeoRotation();
  rotY->RotateY(-90.);
  ypos = -0.2*tubeRadius;
  zpos = absorberThicknessZ + tubeRadius;
  worldLV->AddNode(tubeLV, 1, new TGeoCombiTrans(xpos, ypos, zpos, rotY));

  //
  // Drift Tube Gas
  //
  TGeoShape* gasS 
    = new TGeoTube(
            "Gas", wireRadius, tubeRadius-tubeThickness, tubeHalfLength); 

  TGeoVolume* gasLV 
    = new TGeoVolume( "Gas", gasS, gasMedium);

  ypos = 0.;
  zpos = 0.;
  tubeLV->AddNode(gasLV, 1, new TGeoTranslation(xpos, ypos, zpos));

  //
  // Wire
  //
  TGeoShape* wireS 
    = new TGeoTube("Wire", 0, wireRadius, tubeHalfLength);

  TGeoVolume* wireLV 
    = new TGeoVolume("Wire", wireS, wireMedium); 

  ypos = 0.;
  zpos = 0.;
  tubeLV->AddNode(wireLV, 1, new TGeoTranslation(xpos, ypos, zpos));

  // close geometry
  gGeoManager->CloseGeometry();

  // write geometry
  gGeoManager->Export("geometry.root");
    
  // notify VMC about Root geometry
  gMC->SetRootGeometry();
}

}
}
