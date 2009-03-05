// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex03DetectorConstruction.cxx 
/// \brief Implementation of the Ex03DetectorConstruction class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
/// Id: ExN03DetectorConstruction.cc,v 1.11 2002/01/09 17:24:12 ranjard Exp \n
///
/// 11/12/2008: 
/// - Updated materials definition using directly Root objects
/// - Added new materials according to:
///   Id: ExN03DetectorConstruction.cc,v 1.24 2008/08/12 20:00:03 gum Exp
///   GEANT4 tag Name: geant4-09-01-ref-09
/// - Changed cuts in SetCuts() according to Geant4 09-01-ref-09
///
/// \date 06/03/2002
/// \author I. Hrivnacova; IPN, Orsay
 
#include <Riostream.h>
#include <TGeoManager.h>
#include <TGeoElement.h>
#include <TGeoMaterial.h>
#include <TVirtualMC.h>
#include <TList.h>

#include "Ex03DetectorConstruction.h"

/// \cond CLASSIMP
ClassImp(Ex03DetectorConstruction)
/// \endcond

//_____________________________________________________________________________
Ex03DetectorConstruction::Ex03DetectorConstruction()
  : TObject(),
    fNbOfLayers(0),
    fWorldSizeX(0.),
    fWorldSizeYZ(0.),
    fCalorSizeYZ(0.),
    fCalorThickness(0.),
    fLayerThickness(0.),
    fAbsorberThickness(0.),
    fGapThickness(0.),
    fDefaultMaterial("Galactic"),
    fAbsorberMaterial("Lead"),
    fGapMaterial("liquidArgon")
{
/// Default constuctor

   // default parameter values of the calorimeter (in cm)
   fAbsorberThickness = 1.;
   fGapThickness      = 0.5;
   fNbOfLayers        = 10;
   fCalorSizeYZ       = 10.;
   
   ComputeCalorParameters();
}

//_____________________________________________________________________________
Ex03DetectorConstruction::~Ex03DetectorConstruction()
{
/// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
void Ex03DetectorConstruction::ComputeCalorParameters()
{
/// Compute derived parameters of the calorimeter

  fLayerThickness = fAbsorberThickness + fGapThickness;
  fCalorThickness = fNbOfLayers*fLayerThickness;
     
  fWorldSizeX  = 1.2*fCalorThickness; 
  fWorldSizeYZ = 1.2*fCalorSizeYZ;
}

//
// public methods
//

//_____________________________________________________________________________
void Ex03DetectorConstruction::ConstructMaterials()
{
/// Construct materials using TGeo modeller

  //
  // Tracking medias (defaut parameters)
  //

  // Create Root geometry manager 
  new TGeoManager("E03_geometry", "E03 VMC example geometry");


//--------- Material definition ---------

  TString name;      // Material name
  Double_t a;        // Mass of a mole in g/mole   
  Double_t z;        // Atomic number
  Double_t density;  // Material density in g/cm3
 
//
// define simple materials
//

  new TGeoMaterial("Aluminium", a=26.98, z=13., density=2.700); 

  new TGeoMaterial("liquidArgon", a=39.95, z=18., density=1.390);

  new TGeoMaterial("Lead", a=207.19, z=82., density=11.35); 

//
// define a material from elements.   case 1: chemical molecule
//

  // Elements
  
  TGeoElement* elH  = new TGeoElement("Hydrogen", "H", z= 1,  a= 1.01);
  TGeoElement* elC  = new TGeoElement("Carbon"  , "C", z= 6., a= 12.01);
  TGeoElement* elN  = new TGeoElement("Nitrogen", "N", z= 7., a= 14.01);
  TGeoElement* elO  = new TGeoElement("Oxygen"  , "O", z= 8., a= 16.00);
  TGeoElement* elSi = new TGeoElement("Silicon", "Si", z= 14., a= 28.09);


/*
  // define an Element from isotopes, by relative abundance 
  // (cannot be done with TGeo)

  G4Isotope* U5 = new G4Isotope("U235", iz=92, n=235, a=235.01*g/mole);
  G4Isotope* U8 = new G4Isotope("U238", iz=92, n=238, a=238.03*g/mole);

  G4Element* U  = new G4Element("enriched Uranium",symbol="U",ncomponents=2);
  U->AddIsotope(U5, abundance= 90.*perCent);
  U->AddIsotope(U8, abundance= 10.*perCent);
*/

  TGeoMixture* matH2O
    = new TGeoMixture("Water", 2, density=1.000);
  matH2O->AddElement(elH, 2);  
  matH2O->AddElement(elO, 1);  
  // overwrite computed meanExcitationEnergy with ICRU recommended value 
  // (cannot be done with TGeo)
  // H2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);

  TGeoMixture* matSci
    = new TGeoMixture("Scintillator", 2, density=1.032);
  matSci->AddElement(elC,  9); 
  matSci->AddElement(elH, 10); 

  TGeoMixture* matMyl
    = new TGeoMixture("Mylar", 3, density=1.397);
  matMyl->AddElement(elC, 10); 
  matMyl->AddElement(elH,  8); 
  matMyl->AddElement(elO,  4); 

  TGeoMixture* matSiO2
    = new TGeoMixture("quartz", 2, density=2.200);
  matSiO2->AddElement(elSi, 1); 
  matSiO2->AddElement(elO, 2); 

//
// define a material from elements.   case 2: mixture by fractional mass
//

  TGeoMixture* matAir
    = new TGeoMixture("Air", 2, density=1.29e-03);
  matAir->AddElement(elN, 0.7); 
  matAir->AddElement(elO, 0.3); 

//
// Define a material from elements and/or others materials (mixture of mixtures)
//

  TGeoMixture* matAerog
    = new TGeoMixture("Aerogel", 3, density=0.200);
  matAerog->AddElement(matSiO2, 0.625); 
  matAerog->AddElement(matH2O,  0.374); 
  matAerog->AddElement(elC,     0.001); 

//
// examples of gas in non STP conditions
//

  TGeoMixture* matCO2
    = new TGeoMixture("CarbonicGas", 2, density=1.842e-03);
  matCO2-> AddElement(elC, 1); 
  matCO2-> AddElement(elO, 2); 

  Double_t atmosphere = 6.32421e+08;
  Double_t pressure   = 50.*atmosphere;
  Double_t temperature = 325.;
  matCO2->SetPressure(pressure);
  matCO2->SetTemperature(temperature);
  matCO2->SetState(TGeoMaterial::kMatStateGas);
  

  TGeoMixture* matSteam
    = new TGeoMixture("WaterSteam", 1, density=0.3e-03);
  matSteam->AddElement(matH2O, 1.0);  

  pressure    = 2.*atmosphere;
  temperature = 500.;
  matSteam->SetPressure(pressure);
  matSteam->SetTemperature(temperature);
  matSteam->SetState(TGeoMaterial::kMatStateGas);

//
// examples of vacuum
//

  new TGeoMaterial("Galactic", a=1.e-16, z=1.e-16, density=1.e-16); 

  TGeoMixture* matBeam
    = new TGeoMixture("Beam", 1, density=1.e-5);
  matBeam->AddElement(matAir, 1.0);  

  pressure    = 2.*atmosphere;
  temperature = STP_temperature;
  matBeam->SetPressure(pressure);
  matBeam->SetTemperature(temperature);
  matBeam->SetState(TGeoMaterial::kMatStateGas);

  //
  // Tracking media
  //

  // Paremeter for tracking media  
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
  TList* materials = gGeoManager->GetListOfMaterials();
  TIter next(materials);
  while (TObject *obj = next()) {
    TGeoMaterial* material = (TGeoMaterial*)obj;    
    new TGeoMedium(material->GetName(), ++mediumId, material, param);
  }

}    

//_____________________________________________________________________________
void Ex03DetectorConstruction::ConstructGeometry()
{
/// Contruct volumes using TGeo modeller

  // Complete the Calor parameters definition 
  ComputeCalorParameters();
   
  Double_t* ubuf = 0;
  
  // Media Ids
  Int_t defaultMediumId = gGeoManager->GetMedium(fDefaultMaterial.Data())->GetId();
  Int_t absorberMediumId = gGeoManager->GetMedium(fAbsorberMaterial.Data())->GetId();
  Int_t gapMediumId = gGeoManager->GetMedium(fGapMaterial.Data())->GetId();

  //     
  // World
  //
  
  Double_t world[3];
  world[0] = fWorldSizeX/2.;
  world[1] = fWorldSizeYZ/2.;
  world[2] = fWorldSizeYZ/2.;
  TGeoVolume *top = gGeoManager->Volume("WRLD", "BOX", defaultMediumId, world, 3);
  gGeoManager->SetTopVolume(top);

  //                               
  // Calorimeter
  //  
  if (fCalorThickness > 0.)  { 
  
    Double_t calo[3];
    calo[0] = fCalorThickness/2.;
    calo[1] = fCalorSizeYZ/2.;
    calo[2] = fCalorSizeYZ/2.;
    gGeoManager->Volume("CALO", "BOX", defaultMediumId, calo, 3);

    Double_t posX =  0.;
    Double_t posY =  0.;
    Double_t posZ =  0.;
    gGeoManager->Node("CALO", 1 ,"WRLD", posX, posY, posZ, 0, kTRUE, ubuf);
  
    // Divide  calorimeter along X axis to place layers
    // 
    Double_t start =  - calo[0];
    Double_t width = fCalorThickness/fNbOfLayers;
    gGeoManager->Division("CELL", "CALO", 1, fNbOfLayers, start, width);

    //                                 
    // Layer
    //
    Double_t layer[3];
    layer[0] = fLayerThickness/2.;
    layer[1] = fCalorSizeYZ/2.;
    layer[2] = fCalorSizeYZ/2.;
    gGeoManager->Volume("LAYE", "BOX", defaultMediumId, layer, 3);

    posX =  0.;
    posY =  0.;
    posZ =  0.;
    gGeoManager->Node("LAYE", 1 ,"CELL", posX, posY, posZ, 0, kTRUE, ubuf);
  }  
  
  //                               
  // Absorber
  //  

  if (fAbsorberThickness > 0.) {

    Double_t abso[3];
    abso[0] = fAbsorberThickness/2;
    abso[1] = fCalorSizeYZ/2.;
    abso[2] = fCalorSizeYZ/2.;
    gGeoManager->Volume("ABSO", "BOX", absorberMediumId, abso, 3);

    Double_t posX = -fGapThickness/2.;
    Double_t posY =  0.;
    Double_t posZ =  0.;
    gGeoManager->Node("ABSO", 1 ,"LAYE", posX, posY, posZ, 0, kTRUE, ubuf);
  }  
  
  //                                 
  // Gap
  //
  
  if (fGapThickness > 0.) {     

    Double_t gap[3];
    gap[0] = fGapThickness/2;
    gap[1] = fCalorSizeYZ/2.;
    gap[2] = fCalorSizeYZ/2.;
    gGeoManager->Volume("GAPX", "BOX", gapMediumId, gap, 3);

    Double_t posX = fAbsorberThickness/2.;
    Double_t posY =  0.;
    Double_t posZ =  0.;
    gGeoManager->Node("GAPX", 1 ,"LAYE", posX, posY, posZ, 0, kTRUE, ubuf);
  } 

/*   
  //                                        
  // Visualization attributes
  //
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  logicCalor->SetVisAttributes(simpleBoxVisAtt);
*/

  // close geometry
  gGeoManager->CloseGeometry();
    
  // notify VMC about Root geometry
  gMC->SetRootGeometry();
  
  PrintCalorParameters();
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetCuts()
{
/// Set cuts for e-, gamma equivalent to 1mm cut in G4.

  Int_t mediumId = gMC->MediumId("Aluminium");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 10.e-06);
    gMC->Gstpar(mediumId, "BCUTE",  10.e-06);
    gMC->Gstpar(mediumId, "CUTELE", 597.e-06);
    gMC->Gstpar(mediumId, "DCUTE",  597.e-06);
  }

  mediumId = gMC->MediumId("liquidArgon");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 6.178e-06);
    gMC->Gstpar(mediumId, "BCUTE",  6.178e-06);
    gMC->Gstpar(mediumId, "CUTELE", 342.9e-06);
    gMC->Gstpar(mediumId, "DCUTE",  342.9e-06);
  }

  mediumId = gMC->MediumId("Lead");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 100.5e-06);
    gMC->Gstpar(mediumId, "BCUTE",  100.5e-06);
    gMC->Gstpar(mediumId, "CUTELE", 1.378e-03);
    gMC->Gstpar(mediumId, "DCUTE",  1.378e-03);
  }

  mediumId = gMC->MediumId("Water");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 2.902e-06);
    gMC->Gstpar(mediumId, "BCUTE",  2.902e-06);
    gMC->Gstpar(mediumId, "CUTELE", 347.2e-06);
    gMC->Gstpar(mediumId, "DCUTE",  347.2e-06);
  }

  mediumId = gMC->MediumId("Scintillator");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 2.369e-06);
    gMC->Gstpar(mediumId, "BCUTE",  2.369e-06);
    gMC->Gstpar(mediumId, "CUTELE", 355.8e-06);
    gMC->Gstpar(mediumId, "DCUTE",  355.8e-06);
  }

  mediumId = gMC->MediumId("Mylar");      
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 2.978e-06);
    gMC->Gstpar(mediumId, "BCUTE",  2.978e-06);
    gMC->Gstpar(mediumId, "CUTELE", 417.5e-06);
    gMC->Gstpar(mediumId, "DCUTE",  417.5e-06);
  }

  mediumId = gMC->MediumId("quartz"); 
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 5.516e-06);
    gMC->Gstpar(mediumId, "BCUTE",  5.516e-06);
    gMC->Gstpar(mediumId, "CUTELE", 534.1e-06);
    gMC->Gstpar(mediumId, "DCUTE",  534.1e-06);
  }

  mediumId = gMC->MediumId("Air");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 990.e-09);
    gMC->Gstpar(mediumId, "BCUTE",  990.e-09);
    gMC->Gstpar(mediumId, "CUTELE", 990.e-09);
    gMC->Gstpar(mediumId, "DCUTE",  990.e-09);
  }

  mediumId = gMC->MediumId("Aerogel");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 1.706e-06);
    gMC->Gstpar(mediumId, "BCUTE",  1.706e-06);
    gMC->Gstpar(mediumId, "CUTELE", 119.0e-06);
    gMC->Gstpar(mediumId, "DCUTE",  119.0e-06);
  }

  mediumId = gMC->MediumId("CarbonicGas");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 990.e-09);
    gMC->Gstpar(mediumId, "BCUTE",  990.e-09);
    gMC->Gstpar(mediumId, "CUTELE", 990.e-09);
    gMC->Gstpar(mediumId, "DCUTE",  990.e-09);
  }

  mediumId = gMC->MediumId("WaterSteam");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 990.e-09);
    gMC->Gstpar(mediumId, "BCUTE",  990.e-09);
    gMC->Gstpar(mediumId, "CUTELE", 990.e-09);
    gMC->Gstpar(mediumId, "DCUTE",  990.e-09);
  }

  mediumId = gMC->MediumId("Galactic");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 990.e-09);
    gMC->Gstpar(mediumId, "BCUTE",  990.e-09);
    gMC->Gstpar(mediumId, "CUTELE", 990.e-09);
    gMC->Gstpar(mediumId, "DCUTE",  990.e-09);
  }

  mediumId = gMC->MediumId("Beam");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 990.e-09);
    gMC->Gstpar(mediumId, "BCUTE",  990.e-09);
    gMC->Gstpar(mediumId, "CUTELE", 990.e-09);
    gMC->Gstpar(mediumId, "DCUTE",  990.e-09);
  }
}    

//_____________________________________________________________________________
void Ex03DetectorConstruction::PrintCalorParameters()
{
/// Print calorimeter parameters

  cout << "\n------------------------------------------------------------"
       << "\n---> The calorimeter is " << fNbOfLayers << " layers of: [ "
       << fAbsorberThickness << "cm of " << fAbsorberMaterial  
       << " + "
       << fGapThickness << "cm of " << fGapMaterial << " ] " 
       << "\n------------------------------------------------------------\n";
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetNbOfLayers(Int_t value)
{
/// Set the number of layers.
/// \param value  The new number of calorimeter layers

  fNbOfLayers = value;
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetDefaultMaterial(const TString& materialName)
{
/// Set default material
/// \param materialName  The new default material name.

  fDefaultMaterial = materialName;
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetAbsorberMaterial(const TString& materialName)
{
/// Set absorer material
/// \param materialName  The new absorber material name.

  fAbsorberMaterial = materialName;
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetGapMaterial(const TString& materialName)
{
/// Set gap material
/// \param materialName  The new gap material name.

  fGapMaterial = materialName;
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetCalorSizeYZ(Double_t value)
{
/// Change the transverse size and recompute the calorimeter parameters
/// \param value The new calorimeter tranverse size

  fCalorSizeYZ = value;
}  

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetAbsorberThickness(Double_t value)
{
/// Change the absorber thickness and recompute the calorimeter parameters
/// \param value The new absorber thickness

  fAbsorberThickness = value;
}  

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetGapThickness(Double_t value)
{
/// Change the gap thickness and recompute the calorimeter parameters
/// \param value The new gap thickness

  fGapThickness = value;
}  

/*
//_____________________________________________________________________________
void Ex03DetectorConstruction::UpdateGeometry()
{
// Not available in VMC
}
*/
