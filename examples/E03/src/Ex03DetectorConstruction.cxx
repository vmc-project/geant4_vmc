// $Id: Ex03DetectorConstruction.cxx,v 1.1 2003/03/17 14:56:51 brun Exp $
//
// Geant4 ExampleN03 adapted to Virtual Monte Carlo 
//
// Id: ExN03DetectorConstruction.cc,v 1.11 2002/01/09 17:24:12 ranjard Exp 
// GEANT4 tag $Name:  $
//
// by Ivana Hrivnacova, 6.3.2003
 
#include <Riostream.h>
#include <TVirtualMC.h>

#include "Ex03DetectorConstruction.h"

ClassImp(Ex03DetectorConstruction)

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
    fMediaIds(),
    fDefaultMaterial("Vacuum"),
    fAbsorberMaterial("Lead"),
    fGapMaterial("LiquidArgon")
{
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
// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
Int_t  Ex03DetectorConstruction::GetMediumId(const TString& mediumName) const
{
  NameMapIterator it = fMediaIds.find(mediumName.Data());
  return (*it).second;
}  

//_____________________________________________________________________________
TString  Ex03DetectorConstruction::GetMediumName(Int_t mediumId) const
{
  NameMapIterator it;
  
  for (it = fMediaIds.begin(); it != fMediaIds.end(); it++) 
    if ((*it).second == mediumId) return (*it).first;

  return TString();
}  

//_____________________________________________________________________________
void Ex03DetectorConstruction::ComputeCalorParameters()
{
// Compute derived parameters of the calorimeter
// ---

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
  //
  // Tracking medias (defaut parameters)
  //

  Int_t ifield = 2;          // User defined magnetic field
  Double_t fieldm = 10.;     // Maximum field value (in kiloGauss)
  Double_t epsil  = .001;    // Tracking precision, 
  Double_t stemax = -0.01;   // Maximum displacement for multiple scat 
  Double_t tmaxfd = -20.;    // Maximum angle due to field deflection 
  Double_t deemax = -.3;     // Maximum fractional energy loss, DLS 
  Double_t stmin  = -.8;

//--------- Material definition ---------

  TString name;
  Double_t a;
  Double_t z;
  Double_t density;
  Double_t radl;
  Double_t absl;
  Float_t* ubuf = 0;
 
//
// define simple materials
//

  name = "Al";
  a = 26.98;
  z = 13.;
  density = 2.700;
  radl = 8.893;
  absl = 0.1;   
  Int_t imat = 1;  
  gMC->Material(imat, name.Data(), a, z, density, radl, absl, ubuf, 0);  
  Int_t mediumId = 0;
  gMC->Medium(mediumId, name.Data(),  imat, 0, ifield, fieldm, tmaxfd, stemax, 
              deemax, epsil, stmin, ubuf, 0);
  fMediaIds[name] = mediumId;

  name = "LiquidArgon";
  a = 39.95;
  z = 18.;
  density = 1.390;
  radl = 14.064;
  absl = 0.1;  
  imat = 2;  
  gMC->Material(imat, name.Data(), a, z, density, radl, absl, ubuf, 0);  
  gMC->Medium(mediumId, name.Data(),  imat, 0, ifield, fieldm, tmaxfd, stemax,
              deemax, epsil, stmin, ubuf, 0);
  fMediaIds[name] = mediumId;

  name = "Lead";
  a = 207.19;
  z = 82.;
  density = 11.35;
  radl = 0.5612;
  absl = 0.1;  
  imat = 3;  
  gMC->Material(imat,name.Data() , a, z, density, radl, absl, ubuf, 0);  
  gMC->Medium(mediumId, name.Data(), imat, 0, ifield, fieldm, tmaxfd, stemax,
              deemax, epsil, stmin, ubuf, 0);
  fMediaIds[name] = mediumId;

//
// define a material from elements.   case 1: chemical molecule
//

  name = "Water";
  Double_t aw2[2] = { 1.01, 16.00};
  Double_t zw2[2] = { 1.0,   8.0};
  Double_t ww2[2] = { 2., 1.};
  density = 1.000;  
  imat = 4;
  gMC->Mixture(imat, name.Data(), aw2, zw2, density, -2, ww2); 
  gMC->Medium(mediumId, name.Data(), imat, 0, ifield, fieldm, tmaxfd, stemax,
              deemax, epsil, stmin, ubuf, 0);
  fMediaIds[name] = mediumId;

  name = "Scintillator";
  Double_t as2[2] = { 1.01, 12.01};
  Double_t zs2[2] = { 1.0,   6.0};
  Double_t ws2[2] = { 10., 9.};
  density = 1.032;  
  imat = 5;
  gMC->Mixture(imat, name.Data(), as2, zs2, density, -2, ws2); 
  gMC->Medium(mediumId, name.Data(), imat, 0, ifield, fieldm, tmaxfd, stemax,
              deemax, epsil, stmin, ubuf, 0);
  fMediaIds[name] = mediumId;

  name = "Quartz";
  Double_t aq2[2] = { 28.09, 16.00};
  Double_t zq2[2] = { 14.0,   8.0};
  Double_t wq2[2] = { 1., 2.};
  density = 2.200;  
  imat = 6;
  gMC->Mixture(imat, name.Data(), aq2, zq2, density, -2, wq2); 
  gMC->Medium(mediumId, name.Data(), imat, 0, ifield, fieldm, tmaxfd, stemax,
              deemax, epsil, stmin, ubuf, 0);
  fMediaIds[name] = mediumId;

//
// define a material from elements.   case 2: mixture by fractional mass
//

  name = "Air";
  Double_t aa2[2] = { 14.01, 16.00};
  Double_t za2[2] = {  7.0,   8.0};
  Double_t wa2[2] = {  0.7,   0.3};
  density = 1.29e-03;  
  imat = 7;
  gMC->Mixture(imat, name.Data(), aa2, za2, density, 2, wa2); 
  gMC->Medium(mediumId, name.Data(), imat, 0, ifield, fieldm, tmaxfd, stemax,
              deemax, epsil, stmin, ubuf, 0);
  fMediaIds[name] = mediumId;

//
// !! NOT SUPPORTED BY VMC
//
// define a material from elements and/or others materials (mixture of mixtures)
//
//density = 0.200*g/cm3;
//G4Material* Aerog = new G4Material(name="Aerogel", density, ncomponents=3);
//Aerog->AddMaterial(SiO2, fractionmass=62.5*perCent);
//Aerog->AddMaterial(H2O , fractionmass=37.4*perCent);
//Aerog->AddElement (C   , fractionmass= 0.1*perCent);

//
// !! NOT SUPPORTED BY VMC
//
// examples of gas in non STP conditions
//
//
//density     = 27.*mg/cm3;
//pressure    = 50.*atmosphere;
//temperature = 325.*kelvin;
//G4Material* CO2 = new G4Material(name="CarbonicGas", density, ncomponents=2,
//                                     kStateGas,temperature,pressure);
//CO2->AddElement(C, natoms=1);
//CO2->AddElement(O, natoms=2);
// 
//
// !! NOT SUPPORTED BY VMC
//
//density     = 0.3*mg/cm3;
//pressure    = 2.*atmosphere;
//temperature = 500.*kelvin;
//G4Material* steam = new G4Material(name="WaterSteam", density, ncomponents=1,
//                                      kStateGas,temperature,pressure);
//steam->AddMaterial(H2O, fractionmass=1.);

  name = "Vacuum";
  a = 1.e-16;
  z = 1.e-16;
  density = 1.e-16;
  radl = 1.e16; 
  absl = 1.e16;  //??
  imat = 8;  
  gMC->Material(imat, name.Data(), a, z, density, radl, absl, ubuf, 0);  
  gMC->Medium(mediumId, name.Data(), imat, 0, ifield, fieldm, tmaxfd, stemax,
              deemax, epsil, stmin, ubuf, 0);
  fMediaIds[name] = mediumId;
}    

//_____________________________________________________________________________
void Ex03DetectorConstruction::ConstructGeometry()
{
  // Complete the Calor parameters definition 
  ComputeCalorParameters();
   
  //     
  // World
  //
  
  Double_t world[3];
  world[0] = fWorldSizeX/2.;
  world[1] = fWorldSizeYZ/2.;
  world[2] = fWorldSizeYZ/2.;
  gMC->Gsvolu("WRLD", "BOX", GetMediumId("Air"), world, 3);

  //                               
  // Calorimeter
  //  
  if (fCalorThickness > 0.)  { 
  
    Double_t calo[3];
    calo[0] = fCalorThickness/2.;
    calo[1] = fCalorSizeYZ/2.;
    calo[2] = fCalorSizeYZ/2.;
    gMC->Gsvolu("CALO", "BOX", GetMediumId(fDefaultMaterial.Data()), calo, 3);

    Double_t posX =  0.;
    Double_t posY =  0.;
    Double_t posZ =  0.;
    gMC->Gspos("CALO", 1 ,"WRLD", posX, posY, posZ, 0, "ONLY");
  
    // Divide  calorimeter along X axis to place layers
    // 
    gMC->Gsdvn("CELL", "CALO", fNbOfLayers, 1);

    //                                 
    // Layer
    //
    Double_t layer[3];
    layer[0] = fLayerThickness/2.;
    layer[1] = fCalorSizeYZ/2.;
    layer[2] = fCalorSizeYZ/2.;
    gMC->Gsvolu("LAYE", "BOX", GetMediumId(fDefaultMaterial.Data()), layer, 3);

    posX =  0.;
    posY =  0.;
    posZ =  0.;
    gMC->Gspos("LAYE", 1 ,"CELL", posX, posY, posZ, 0, "ONLY");
  }  
  
  //                               
  // Absorber
  //  

  if (fAbsorberThickness > 0.) {

    Double_t abso[3];
    abso[0] = fAbsorberThickness/2;
    abso[1] = fCalorSizeYZ/2.;
    abso[2] = fCalorSizeYZ/2.;
    gMC->Gsvolu("ABSO", "BOX", GetMediumId(fAbsorberMaterial.Data()), abso, 3);

    Double_t posX = -fGapThickness/2.;
    Double_t posY =  0.;
    Double_t posZ =  0.;
    gMC->Gspos("ABSO", 1 ,"LAYE", posX, posY, posZ, 0, "ONLY");
  }  
  
  //                                 
  // Gap
  //
  
  if (fGapThickness > 0.) {     

    Double_t gap[3];
    gap[0] = fGapThickness/2;
    gap[1] = fCalorSizeYZ/2.;
    gap[2] = fCalorSizeYZ/2.;
    gMC->Gsvolu("GAPX", "BOX", GetMediumId(fGapMaterial.Data()), gap, 3);

    Double_t posX = fAbsorberThickness/2.;
    Double_t posY =  0.;
    Double_t posZ =  0.;
    gMC->Gspos("GAPX", 1 ,"LAYE", posX, posY, posZ, 0, "ONLY");
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
  
  PrintCalorParameters();
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetCuts()
{
// Sets cuts for e-, gamma equivalent to 1mm cut in G4.
// ---

  Int_t mediumId = GetMediumId("Al");
  gMC->Gstpar(mediumId, "CUTGAM", 10.e-06);
  gMC->Gstpar(mediumId, "BCUTE",  10.e-06);
  gMC->Gstpar(mediumId, "CUTELE", 597.e-06);
  gMC->Gstpar(mediumId, "DCUTE",  597.e-06);

  mediumId = GetMediumId("LiquidArgon");
  gMC->Gstpar(mediumId, "CUTGAM", 10.e-06);
  gMC->Gstpar(mediumId, "BCUTE",  10.e-06);
  gMC->Gstpar(mediumId, "CUTELE", 343.e-06);
  gMC->Gstpar(mediumId, "DCUTE",  343.e-06);

  mediumId = GetMediumId("Lead");
  gMC->Gstpar(mediumId, "CUTGAM", 101.e-06);
  gMC->Gstpar(mediumId, "BCUTE",  101.e-06);
  gMC->Gstpar(mediumId, "CUTELE", 1.38e-03);
  gMC->Gstpar(mediumId, "DCUTE",  1.38e-03);

  mediumId = GetMediumId("Water");
  gMC->Gstpar(mediumId, "CUTGAM", 10.e-06);
  gMC->Gstpar(mediumId, "BCUTE",  10.e-06);
  gMC->Gstpar(mediumId, "CUTELE", 347.e-03);
  gMC->Gstpar(mediumId, "DCUTE",  347.e-03);

  mediumId = GetMediumId("Scintillator");
  gMC->Gstpar(mediumId, "CUTGAM", 10.e-06);
  gMC->Gstpar(mediumId, "BCUTE",  10.e-06);
  gMC->Gstpar(mediumId, "CUTELE", 356.e-03);
  gMC->Gstpar(mediumId, "DCUTE",  356.e-03);

  mediumId = GetMediumId("Quartz");
  gMC->Gstpar(mediumId, "CUTGAM", 10.e-06);
  gMC->Gstpar(mediumId, "BCUTE",  10.e-06);
  gMC->Gstpar(mediumId, "CUTELE", 534.e-03);
  gMC->Gstpar(mediumId, "DCUTE",  534.e-03);

  mediumId = GetMediumId("Air");
  gMC->Gstpar(mediumId, "CUTGAM", 10.e-06);
  gMC->Gstpar(mediumId, "BCUTE",  10.e-06);
  gMC->Gstpar(mediumId, "CUTELE", 10.e-06);
  gMC->Gstpar(mediumId, "DCUTE",  10.e-06);

  mediumId = GetMediumId("Vacuum");
  gMC->Gstpar(mediumId, "CUTGAM", 10.e-06);
  gMC->Gstpar(mediumId, "BCUTE",  10.e-06);
  gMC->Gstpar(mediumId, "CUTELE", 10.e-06);
  gMC->Gstpar(mediumId, "DCUTE",  10.e-06);
}    

//_____________________________________________________________________________
void Ex03DetectorConstruction::PrintCalorParameters()
{
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
// Set number of layers.
// --

  fNbOfLayers = value;
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetDefaultMaterial(const TString& materialName)
{
// Set default material
// --

  fDefaultMaterial = materialName;
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetAbsorberMaterial(const TString& materialName)
{
// Set absorber material
// --

  fAbsorberMaterial = materialName;
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetGapMaterial(const TString& materialName)
{
// Set gap material
// --

  fGapMaterial = materialName;
}

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetCalorSizeYZ(Double_t value)
{
// Change the transverse size and recompute the calorimeter parameters
// ---

  fCalorSizeYZ = value;
}  

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetAbsorberThickness(Double_t value)
{
// Change Absorber thickness and recompute the calorimeter parameters
// ---

  fAbsorberThickness = value;
}  

//_____________________________________________________________________________
void Ex03DetectorConstruction::SetGapThickness(Double_t value)
{
// Change Gap thickness and recompute the calorimeter parameters
// ---

  fGapThickness = value;
}  

/*
//_____________________________________________________________________________
void Ex03DetectorConstruction::UpdateGeometry()
{
// Not available in VMC
}
*/
