//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/src/DetectorConstruction.cxx
/// \brief Implementation of the DetectorConstruction class
///
/// \date 18/12/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "DetectorConstruction.h"

#include <Riostream.h>
#include <TGeoBBox.h>
#include <TGeoElement.h>
#include <TGeoManager.h>
#include <TGeoMaterial.h>
#include <TGeoVolume.h>
#include <TVirtualMC.h>

using namespace std;

/// \cond CLASSIMP
ClassImp(VMC::TR::DetectorConstruction)
  /// \endcond

  namespace VMC
{
  namespace TR
  {

  //_____________________________________________________________________________
  DetectorConstruction::DetectorConstruction() : TObject()
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
  void DetectorConstruction::ConstructGeometry()
  {
    // Geometry parameters
    //

    // Create Root geometry manager
    new TGeoManager("TR_geometry", "TR VMC example geometry");

    cout << "DetectorSimpleALICE setup" << endl;

    Double_t cm = 1.0;
    Double_t mm = 0.1;             // milimeter -> cm
    Double_t micrometer = 1.0e-04; // micrometer -> cm
    Double_t worldSizeZ = 400. * cm;
    Double_t worldSizeR = 20. * cm;

    // Radiator and detector parameters

    Double_t radThickness = 0.020 * mm;
    Double_t gasGap = 0.250 * mm;
    Double_t foilGasRatio = radThickness / (radThickness + gasGap);
    Int_t foilNumber = 220;

    Double_t absorberThickness = 38.3 * mm;
    Double_t absorberRadius = 100. * mm;

    Double_t windowThick = 51.0 * micrometer;
    Double_t electrodeThick = 10.0 * micrometer;
    Double_t gapThick = 10.0 * cm;
    Double_t detGap = 0.01 * mm;

    Double_t startZ = 100.0 * mm;

    // Materials
    //
    TString name;          // Material name
    Double_t a;            // Mass of a mole in g/mole
    Double_t z;            // Atomic number
    Double_t density;      // Material density in g/cm3
    Double_t fractionmass; // Fraction mass

    // Elements
    TGeoElement* elH = new TGeoElement("Hydrogen", "H", z = 1, a = 1.01);
    TGeoElement* elC = new TGeoElement("Carbon", "C", z = 6., a = 12.01);
    TGeoElement* elN = new TGeoElement("Nitrogen", "N", z = 7., a = 14.01);
    TGeoElement* elO = new TGeoElement("Oxygen", "O", z = 8., a = 16.00);
    TGeoElement* elAr = new TGeoElement("Argon", "Ar", z = 18., a = 39.94);

    // Dry Air (average composition with Ar), STP
    TGeoMixture* matAir = new TGeoMixture("Air", 3, density = 1.2928e-03);
    matAir->AddElement(elN, fractionmass = 0.7557);
    matAir->AddElement(elO, fractionmass = 0.2315);
    matAir->AddElement(elAr, fractionmass = 0.0128);

    // Mylar
    TGeoMixture* matMylar = new TGeoMixture("Mylar", 3, density = 1.39);
    matMylar->AddElement(elO, 2);
    matMylar->AddElement(elC, 5);
    matMylar->AddElement(elH, 4);

    // Xenon as detector gas, STP
    TGeoMaterial* matXe =
      new TGeoMaterial("Xenon", a = 131.29, z = 54., density = 5.858e-06);

    // Carbon dioxide, STP
    TGeoMixture* matCO2 = new TGeoMixture("CO2", 2, density = 1.977e-06);
    matCO2->AddElement(elC, 1);
    matCO2->AddElement(elO, 2);

    // 85% Xe + 15% CO2, STP
    TGeoMixture* matXe15CO2 = new TGeoMixture("Xe15CO2", 2, density = 4.9e-03);
    matXe15CO2->AddElement(matXe, fractionmass = 0.979);
    matXe15CO2->AddElement(matCO2, fractionmass = 0.021);

    Double_t foilDensity = matMylar->GetDensity();
    Double_t gasDensity = matAir->GetDensity();
    Double_t totDensity =
      foilDensity * foilGasRatio + gasDensity * (1.0 - foilGasRatio);

    Double_t fractionFoil = foilDensity * foilGasRatio / totDensity;
    Double_t fractionGas = gasDensity * (1.0 - foilGasRatio) / totDensity;

    TGeoMixture* matRadiator = new TGeoMixture("radiatorMat", 2, totDensity);
    matRadiator->AddElement(matMylar, fractionFoil);
    matRadiator->AddElement(matAir, fractionGas);

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
    for (Int_t i = 8; i < 20; ++i) param[i] = 0.;

    Int_t mediumId = 0;
    TGeoMedium* medAir = new TGeoMedium("Air", ++mediumId, matAir, param);
    TGeoMedium* medXe15CO2 =
      new TGeoMedium("Xe15CO2", ++mediumId, matXe15CO2, param);
    TGeoMedium* medRadiator =
      new TGeoMedium("Radiator", ++mediumId, matRadiator, param);

    TGeoMedium* worldMedium = medAir;
    TGeoMedium* absorberMedium = medXe15CO2;
    TGeoMedium* radiatorMedium = medRadiator;

    // Volumes
    //

    TGeoShape* solidWorld =
      new TGeoBBox("World", worldSizeR, worldSizeR, worldSizeZ / 2.);

    TGeoVolume* logicWorld = new TGeoVolume("World", solidWorld, worldMedium);

    gGeoManager->SetTopVolume(logicWorld);

    // TR radiator envelope

    Double_t radThick = foilNumber * (radThickness + gasGap) - gasGap + detGap;
    Double_t radZ = startZ + 0.5 * radThick;

    TGeoShape* solidRadiator = new TGeoBBox(
      "Radiator", 1.1 * absorberRadius, 1.1 * absorberRadius, 0.5 * radThick);

    TGeoVolume* logicRadiator =
      new TGeoVolume("Radiator", solidRadiator, radiatorMedium);

    logicWorld->AddNode(logicRadiator, 1, new TGeoTranslation(0, 0, radZ));

    // Print radiator description
    // These values should be set via /mcDet/setRadiator command
    //
    cout << "Radiator volume: " << logicRadiator->GetName() << endl;
    cout << "Foil material:   " << matMylar->GetName() << endl;
    cout << "Gas material:    " << matAir->GetName() << endl;
    cout << "Foil thickness:  " << radThickness << endl;
    cout << "Gas thickness:   " << gasGap << endl;
    cout << "Foild number:    " << foilNumber << endl;

    // create region for window inside windowR for

    // G4Region* radRegion = new G4Region("XTRradiator");
    // radRegion->AddRootLogicalVolume(logicRadiator);

    // Absorber

    TGeoShape* solidAbsorber = new TGeoBBox(
      "Absorber", absorberRadius, absorberRadius, absorberThickness / 2.);

    TGeoVolume* logicAbsorber =
      new TGeoVolume("Absorber", solidAbsorber, absorberMedium);

    Double_t windowZ = startZ + radThick + windowThick / 2. + 15.0 * mm;
    Double_t gapZ = windowZ + windowThick / 2. + gapThick / 2. + 0.01 * mm;
    Double_t electrodeZ =
      gapZ + gapThick / 2. + electrodeThick / 2. + 0.01 * mm;
    Double_t absorberZ =
      electrodeZ + electrodeThick / 2. + absorberThickness / 2. + 0.01 * mm;

    logicWorld->AddNode(
      logicAbsorber, 1, new TGeoTranslation(0., 0., absorberZ));

    // G4Region* regGasDet = new G4Region("XTRdEdxDetector");
    // regGasDet->AddRootLogicalVolume(logicAbsorber);

    // close geometry
    gGeoManager->CloseGeometry();

    // notify VMC about Root geometry
    gMC->SetRootGeometry();

    // Sensitive Detectors: Absorber

    // SensitiveDetector* sd = new SensitiveDetector("SensitiveDetector");
    // G4SDManager::GetSDMpointer()->AddNewDetector(sd );
    // logicAbsorber->SetSensitiveDetector(sd);

    // Print geometry parameters

    cout << "\n The  WORLD   is made of " << worldSizeZ / mm << "mm of "
         << worldMedium->GetName();
    cout << ", the transverse size (R) of the world is " << worldSizeR / mm
         << " mm. " << endl;
    cout << " The ABSORBER is made of " << absorberThickness / mm << "mm of "
         << absorberMedium->GetName();
    cout << ", the transverse size (R) is " << absorberRadius / mm << " mm. "
         << endl;
    cout << " Z position of the (middle of the) absorber " << absorberZ / mm
         << "  mm." << endl;

    cout << "radZ = " << radZ / mm << " mm" << endl;
    cout << "startZ = " << startZ / mm << " mm" << endl;

    cout << "fRadThick = " << radThick / mm << " mm" << endl;
    cout << "fFoilNumber = " << foilNumber << endl;
    cout << "fRadiatorMat = " << radiatorMedium->GetMaterial()->GetName()
         << endl;
    cout << "WorldMaterial = " << worldMedium->GetMaterial()->GetName() << endl;
    cout << endl;
  }

  } // namespace TR
}
