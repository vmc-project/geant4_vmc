// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex06DetectorConstruction.cxx 
/// \brief Implementation of the Ex06DetectorConstruction class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
/// Id: ExN06DetectorConstruction.cc,v 1.14 2004/03/17 22:41:12 gum Exp 
/// GEANT4 tag Name: geant4-07-00-cand-01 
///
/// \date 16/05/2005
/// \author I. Hrivnacova; IPN, Orsay
 
#include <Riostream.h>
#include <TVirtualMC.h>
#include <TGeoManager.h>

#include "Ex06DetectorConstruction.h"

/// \cond CLASSIMP
ClassImp(Ex06DetectorConstruction)
/// \endcond

//_____________________________________________________________________________
Ex06DetectorConstruction::Ex06DetectorConstruction()
  : TObject(),
    fExpHallSize(1000),  // 10*m
    fTankSize(500),      // 5*m
    fBubbleSize(50),     // 0.5*m
    fImedAir(-1),
    fImedWater(-1)
{
/// Default constuctor
}

//_____________________________________________________________________________
Ex06DetectorConstruction::~Ex06DetectorConstruction()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void Ex06DetectorConstruction::ConstructMaterials()
{
/// Construct materials using TGeo modeller

  // Create Root geometry manager 
  new TGeoManager("E06_geometry", "E06 VMC example geometry");
   
  Double_t a;        // Mass of a mole in g/mole   
  Double_t z;        // Atomic number
  Double_t density;  // Material density in g/cm3
 
  // Elements
  
  TGeoElement* elH  = new TGeoElement("Hydrogen", "H", z= 1,  a= 1.01);
  TGeoElement* elN  = new TGeoElement("Nitrogen", "N", z= 7., a= 14.01);
  TGeoElement* elO  = new TGeoElement("Oxygen"  , "O", z= 8., a= 16.00);
  
  // Materials

  TGeoMixture* matAir
    = new TGeoMixture("Air", 2, density = 1.29e-03);
  matAir->AddElement(elN, 0.7); 
  matAir->AddElement(elO, 0.3); 

  TGeoMixture* matH2O
    = new TGeoMixture("Water", 2, density = 1.000);
  matH2O->AddElement(elH, 2);  
  matH2O->AddElement(elO, 1);  

  // Tracking media

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

  fImedAir = 1;
  new TGeoMedium("Air", fImedAir, matAir, param);
  
  fImedWater = 2;
  new TGeoMedium("Water", fImedWater, matH2O, param);
}    

//_____________________________________________________________________________
void Ex06DetectorConstruction::ConstructGeometry()
{
/// Contruct volumes using TGeo modeller

// The experimental Hall
//
  Double_t* ubuf = 0;

  Double_t expHall[3];
  expHall[0] = fExpHallSize;
  expHall[1] = fExpHallSize;
  expHall[2] = fExpHallSize;
  TGeoVolume* expHallV 
    = gGeoManager->Volume("WRLD","BOX", fImedAir, expHall, 3);
  gGeoManager->SetTopVolume(expHallV);
   
// The Water Tank
//	
  Double_t waterTank[3];
  waterTank[0] = fTankSize;
  waterTank[1] = fTankSize;
  waterTank[2] = fTankSize;
  gGeoManager->Volume("TANK","BOX", fImedWater, waterTank, 3);
   
  Double_t posX =  0.;
  Double_t posY =  0.;
  Double_t posZ =  0.;
  gGeoManager->Node("TANK", 1 ,"WRLD", posX, posY, posZ, 0, kTRUE, ubuf);
  
// The Air Bubble 
//	
  Double_t bubbleAir[3];
  bubbleAir[0] = fBubbleSize;
  bubbleAir[1] = fBubbleSize;
  bubbleAir[2] = fBubbleSize;
  gGeoManager->Volume("BUBL","BOX", fImedAir, bubbleAir, 3);
   
  posX =  0.;
  posY =  250.;
  posZ =  0.;
  gGeoManager->Node("BUBL", 1 ,"TANK", posX, posY, posZ, 0, kTRUE, ubuf);

  // close geometry
  gGeoManager->CloseGeometry();

  // notify VMC about Root geometry
  gMC->SetRootGeometry();
}
    
//_____________________________________________________________________________
void Ex06DetectorConstruction::ConstructOpGeometry()
{
/// Define material optical properties

// ------------ Generate & Add Material Properties Table ------------
//

  fImedAir = gMC->MediumId("Air");
  fImedWater = gMC->MediumId("Water");

  const Int_t nEntries = 32;

  Double_t photonEnergy[nEntries] =
            { 2.034e-09, 2.068e-09, 2.103e-09, 2.139e-09,
              2.177e-09, 2.216e-09, 2.256e-09, 2.298e-09,
              2.341e-09, 2.386e-09, 2.433e-09, 2.481e-09,
              2.532e-09, 2.585e-09, 2.640e-09, 2.697e-09,
              2.757e-09, 2.820e-09, 2.885e-09, 2.954e-09,
              3.026e-09, 3.102e-09, 3.181e-09, 3.265e-09,
              3.353e-09, 3.446e-09, 3.545e-09, 3.649e-09,
              3.760e-09, 3.877e-09, 4.002e-09, 4.136e-09 }; 
//
// Water
//	      
  Double_t refractiveIndex1[nEntries] =
            { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
              1.346,  1.3465, 1.347,  1.3475, 1.348,
              1.3485, 1.3492, 1.35,   1.3505, 1.351,
              1.3518, 1.3522, 1.3530, 1.3535, 1.354,
              1.3545, 1.355,  1.3555, 1.356,  1.3568,
              1.3572, 1.358,  1.3585, 1.359,  1.3595,
              1.36,   1.3608};

  Double_t absorption1[nEntries] =
           {3.448*100.,  4.082*100.,  6.329*100.,  9.174*100., 12.346*100., 13.889*100.,
           15.152*100., 17.241*100., 18.868*100., 20.000*100., 26.316*100., 35.714*100.,
           45.455*100., 47.619*100., 52.632*100., 52.632*100., 55.556*100., 52.632*100.,
           52.632*100., 47.619*100., 45.455*100., 41.667*100., 37.037*100., 33.333*100.,
           30.000*100., 28.500*100., 27.000*100., 24.500*100., 22.000*100., 19.500*100.,
           17.500*100., 14.500*100. };


  // Added (for Geant3)
  Double_t efficiency1[nEntries] =
            { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00 };

  Double_t scintilFast[nEntries] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };
  Double_t scintilSlow[nEntries] =
            { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
              7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
              3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
              4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
              7.00, 6.00, 5.00, 4.00 };

  gMC->SetCerenkov(fImedWater, nEntries, photonEnergy,
                   absorption1, efficiency1, refractiveIndex1); 
                   
  gMC->SetMaterialProperty(fImedWater, 
                           "FASTCOMPONENT", nEntries, photonEnergy, scintilFast);
  gMC->SetMaterialProperty(fImedWater, 
                           "SLOWCOMPONENT", nEntries, photonEnergy, scintilSlow);

  gMC->SetMaterialProperty(fImedWater, "SCINTILLATIONYIELD", 50.e03);  // 50./MeV
  gMC->SetMaterialProperty(fImedWater, "RESOLUTIONSCALE",  1.0);
  gMC->SetMaterialProperty(fImedWater, "FASTTIMECONSTANT",  1.0e-09);  // 1.*ns
  gMC->SetMaterialProperty(fImedWater, "SLOWTIMECONSTANT", 10.0e-09); // 10.*ns
  gMC->SetMaterialProperty(fImedWater, "YIELDRATIO", 0.8);

  const Int_t nEntriesWater = 60;

  Double_t energyWater[nEntriesWater] = {
     1.56962e-09, 1.58974e-09, 1.61039e-09, 1.63157e-09,
     1.65333e-09, 1.67567e-09, 1.69863e-09, 1.72222e-09,
     1.74647e-09, 1.77142e-09, 1.79710e-09, 1.82352e-09,
     1.85074e-09, 1.87878e-09, 1.90769e-09, 1.93749e-09,
     1.96825e-09, 1.99999e-09, 2.03278e-09, 2.06666e-09,
     2.10169e-09, 2.13793e-09, 2.17543e-09, 2.21428e-09,
     2.25454e-09, 2.29629e-09, 2.33962e-09, 2.38461e-09,
     2.43137e-09, 2.47999e-09, 2.53061e-09, 2.58333e-09,
     2.63829e-09, 2.69565e-09, 2.75555e-09, 2.81817e-09,
     2.88371e-09, 2.95237e-09, 3.02438e-09, 3.09999e-09,
     3.17948e-09, 3.26315e-09, 3.35134e-09, 3.44444e-09,
     3.54285e-09, 3.64705e-09, 3.75757e-09, 3.87499e-09,
     3.99999e-09, 4.13332e-09, 4.27585e-09, 4.42856e-09,
     4.59258e-09, 4.76922e-09, 4.95999e-09, 5.16665e-09,
     5.39129e-09, 5.63635e-09, 5.90475e-09, 6.19998e-09
  };

  //assume 100 times larger than the rayleigh scattering for now.
  Double_t mieWater[nEntriesWater] = {
     167024.4*100, 158726.7*100, 150742  *100,
     143062.5*100, 135680.2*100, 128587.4*100,
     121776.3*100, 115239.5*100, 108969.5*100,
     102958.8*100, 97200.35*100, 91686.86*100,
     86411.33*100, 81366.79*100, 76546.42*100,
     71943.46*100, 67551.29*100, 63363.36*100,
     59373.25*100, 55574.61*100, 51961.24*100,
     48527.00*100, 45265.87*100, 42171.94*100,
     39239.39*100, 36462.50*100, 33835.68*100,
     31353.41*100, 29010.30*100, 26801.03*100,
     24720.42*100, 22763.36*100, 20924.88*100,
     19200.07*100, 17584.16*100, 16072.45*100,
     14660.38*100, 13343.46*100, 12117.33*100,
     10977.70*100, 9920.416*100, 8941.407*100,
     8036.711*100, 7202.470*100, 6434.927*100,
     5730.429*100, 5085.425*100, 4496.467*100,
     3960.210*100, 3473.413*100, 3032.937*100,
     2635.746*100, 2278.907*100, 1959.588*100,
     1675.064*100, 1422.710*100, 1200.004*100,
     1004.528*100, 833.9666*100, 686.1063*100
  };

  // gforward, gbackward, forward backward ratio
  Double_t mieWaterConst[3] = { 0.99, 0.99, 0.8 };

  gMC->SetMaterialProperty(fImedWater, 
                           "MIEHG", nEntriesWater, energyWater, mieWater);
  gMC->SetMaterialProperty(fImedWater, "MIEHG_FORWARD", mieWaterConst[0]);
  gMC->SetMaterialProperty(fImedWater, "MIEHG_BACKWARD", mieWaterConst[1]);
  gMC->SetMaterialProperty(fImedWater, "MIEHG_FORWARD_RATIO", mieWaterConst[2]);

  // Set the Birks Constant for the Water scintillator
  gMC->SetMaterialProperty(fImedWater, "BIRKS_CONSTANT", 0.126e+02); // mm/MeV


//	------------- Surfaces --------------
//
// Water Tank
//
  gMC->DefineOpSurface("WaterSurface", 
                        kUnified, kDielectric_dielectric, kGround, 1.0);
			          // CHECK default value of sigma alpha
  gMC->SetBorderSurface("WaterSurface", 
                       "TANK", 1, "WRLD", 1, "WaterSurface");
                                  // the world copyNo is in VMC always 1 
                       				  
  
// Air Bubble
//
  gMC->DefineOpSurface("AirSurface", 
                        kGlisur, kDielectric_dielectric, kPolished, 1.0);
			          // CHECK default value of sigma alpha
  gMC->SetSkinSurface("AirSurface", "BUBL", "AirSurface");				  
  

//
// Generate & Add Material Properties Table attached to the optical surfaces
//
  const Int_t num = 2;
  Double_t ephoton[num] = { 2.038e-09, 4.144e-09 };

  // OpticalWaterSurface 
  Double_t refractiveIndex[num] = { 1.35, 1.40 };
  Double_t specularLobe[num]    = { 0.3, 0.3 };
  Double_t specularSpike[num]   = { 0.2, 0.2 };
  Double_t backscatter[num]     = { 0.2, 0.2 };
  gMC->SetMaterialProperty("WaterSurface", 
                           "RINDEX", num, ephoton, refractiveIndex);  
  gMC->SetMaterialProperty("WaterSurface", 
                           "SPECULARLOBECONSTANT", num, ephoton, specularLobe);  
  gMC->SetMaterialProperty("WaterSurface", 
                           "SPECULARSPIKECONSTANT", num, ephoton, specularSpike);  
  gMC->SetMaterialProperty("WaterSurface", 
                           "BACKSCATTERCONSTANT", num, ephoton, backscatter);  


  //OpticalAirSurface
  Double_t reflectivity[num] = { 0.3, 0.5 };
  Double_t efficiency[num]   = { 0.8, 1.0 };
  gMC->SetMaterialProperty("AirSurface", 
                           "REFLECTIVITY", num, ephoton, reflectivity);  
  gMC->SetMaterialProperty("AirSurface", 
                           "EFFICIENCY", num, ephoton, efficiency);  
}

