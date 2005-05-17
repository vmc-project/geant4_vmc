// $Id: $
//
// Geant4 ExampleN06 adapted to Virtual Monte Carlo 
//
// Id: ExN06DetectorConstruction.cc,v 1.14 2004/03/17 22:41:12 gum Exp 
// GEANT4 tag Name: geant4-07-00-cand-01 
//
// by Ivana Hrivnacova, 16.5.2005
 
#include <Riostream.h>
#include <TVirtualMC.h>
#include <TGeoManager.h>

#include "Ex06DetectorConstruction.h"

ClassImp(Ex06DetectorConstruction)

//_____________________________________________________________________________
Ex06DetectorConstruction::Ex06DetectorConstruction()
  : TObject()
{
    fExpHallSize = 1000;  // 10*m
    fTankSize    = 500;   // 5*m
    fBubbleSize  = 50;    // 0.5*m
}

//_____________________________________________________________________________
Ex06DetectorConstruction::~Ex06DetectorConstruction()
{
// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
void Ex06DetectorConstruction::ConstructMaterialsViaTGeo()
{
  // Create Root geometry manager 
  new TGeoManager("TGeo", "Root geometry manager");
   
  Double_t a1[2] = { 14.01, 16.00};
  Double_t z1[2] = {  7.0,   8.0};
  Double_t w1[2] = {  0.7,   0.3};
  Double_t density = 1.29e-03;  
  Int_t imatAir = 1;
  gGeoManager->Mixture("Air", a1, z1, density, 2, w1, imatAir); 

  Double_t a2[2] = {  1.01, 16.00};
  Double_t z2[2] = {  1.0,   8.0};
  Double_t w2[2] = {  2.0,   1.0};
  // convert numbers of elements to relative wights
  Double_t amol = 0;
  Int_t i;
  for (i=0;i<2;i++)  amol += a2[i]*w2[i];
  for (i=0;i<2;i++)  w2[i] *= a2[i]/amol;

  density = 1.0;  
  Int_t imatWater = 2;
  gGeoManager->Mixture("Water", a2, z2, density, 2, w2, imatWater); 

  //
  // Tracking medias
  //

  Int_t ifield = 0;         // No magnetic field 
  Double_t fieldm = 0.;     //
  Double_t epsil  = .001;    // Tracking precision, 
  Double_t stemax = -0.01;   // Maximum displacement for multiple scat 
  Double_t tmaxfd = -20.;    // Maximum angle due to field deflection 
  Double_t deemax = -.3;     // Maximum fractional energy loss, DLS 
  Double_t stmin  = -.8;
  fImedAir = 1;
  fImedWater = 2;
  gGeoManager->Medium("Air", fImedAir, imatAir, 0, ifield, fieldm, tmaxfd, 
                      stemax, deemax, epsil, stmin); 
  gGeoManager->Medium("Water", fImedWater, imatWater, 0, ifield, fieldm, tmaxfd,
                      stemax, deemax, epsil, stmin); 
}    

//_____________________________________________________________________________
void Ex06DetectorConstruction::ConstructGeometryViaTGeo()
{

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
void Ex06DetectorConstruction::ConstructMaterialsViaVMC()
{
   
  Double_t a1[2] = { 14.01, 16.00};
  Double_t z1[2] = {  7.0,   8.0};
  Double_t w1[2] = {  0.7,   0.3};
  Double_t density = 1.29e-03;  
  Int_t imatAir = 1;
  gMC->Mixture(imatAir, "Air", a1, z1, density, 2, w1); 

  Double_t a2[2] = {  1.01, 16.00};
  Double_t z2[2] = {  1.0,   8.0};
  Double_t w2[2] = {  2.0,   1.0};
  density = 1.0;  
  Int_t imatWater = 2;
  gMC->Mixture(imatWater, "Water", a2, z2, density, -2, w2); 

  //
  // Tracking medias
  //

  Int_t ifield = 0;           // No magnetic field 
  Double_t fieldm = 0.;       //
  Double_t epsil  = .001;    // Tracking precision, 
  Double_t stemax = -0.01;   // Maximum displacement for multiple scat 
  Double_t tmaxfd = -20.;    // Maximum angle due to field deflection 
  Double_t deemax = -.3;     // Maximum fractional energy loss, DLS 
  Double_t stmin  = -.8;
  Float_t* ubuf = 0;
  gMC->Medium(fImedAir, "Air", imatAir, 0, ifield, fieldm, tmaxfd, 
                      stemax, deemax, epsil, stmin, ubuf, 0); 
  gMC->Medium(fImedWater, "Water", imatWater, 0, ifield, fieldm, tmaxfd,
                      stemax, deemax, epsil, stmin, ubuf, 0); 
}


//_____________________________________________________________________________
void Ex06DetectorConstruction::ConstructGeometryViaVMC()
{

// The experimental Hall
//
  Double_t expHall[3];
  expHall[0] = fExpHallSize;
  expHall[1] = fExpHallSize;
  expHall[2] = fExpHallSize;
  gMC->Gsvolu("WRLD","BOX", fImedAir, expHall, 3);
   
// The Water Tank
//	
  Double_t waterTank[3];
  waterTank[0] = fTankSize;
  waterTank[1] = fTankSize;
  waterTank[2] = fTankSize;
  gMC->Gsvolu("TANK","BOX", fImedWater, waterTank, 3);
   
  Double_t posX =  0.;
  Double_t posY =  0.;
  Double_t posZ =  0.;
  gMC->Gspos("TANK", 1 ,"WRLD", posX, posY, posZ, 0, "ONLY");
  
// The Air Bubble 
//	
  Double_t bubbleAir[3];
  bubbleAir[0] = fBubbleSize;
  bubbleAir[1] = fBubbleSize;
  bubbleAir[2] = fBubbleSize;
  gMC->Gsvolu("BUBL","BOX", fImedAir, bubbleAir, 3);
   
  posX =  0.;
  posY =  250.;
  posZ =  0.;
  gMC->Gspos("BUBL", 1 ,"TANK", posX, posY, posZ, 0, "ONLY");
}

//
// public methods
//

//_____________________________________________________________________________
void Ex06DetectorConstruction::ConstructGeometry()
{
// Construct geometry via TGeo, if possible,
// or via VMC otherwise

  if (gMC->IsRootGeometrySupported()) {
    cout << "Geometry will be defined via TGeo" << endl;
    ConstructMaterialsViaTGeo();  
    ConstructGeometryViaTGeo(); 
    fIsTGeoGeometry = kTRUE;
  }
  else {   
    cout << "Geometry will be defined via VMC" << endl;
    ConstructMaterialsViaVMC();  
    ConstructGeometryViaVMC(); 
    fIsTGeoGeometry = kFALSE;
  }
}
    
//_____________________________________________________________________________
void Ex06DetectorConstruction::ConstructOpGeometry()
{
// ------------ Generate & Add Material Properties Table ------------
//

  if (fIsTGeoGeometry) {
    // If geometry is constructed via TGeo,
    // the materials and tracking media indices
    // can be changed by MC !!!
    fImedAir = gGeoManager->GetMedium("Air")->GetId();
    fImedWater = gGeoManager->GetMedium("Water")->GetId();
  }  

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

  cout << "gMC->SetCerenkov " << endl;
  gMC->SetCerenkov(fImedWater, nEntries, photonEnergy,
                   absorption1, efficiency1, refractiveIndex1); 
                   
  cout << "gMC->SetCerenkov done " << endl;
  gMC->SetMaterialProperty(fImedWater, 
                           "FASTCOMPONENT", nEntries, photonEnergy, scintilFast);
  gMC->SetMaterialProperty(fImedWater, 
                           "SLOWCOMPONENT", nEntries, photonEnergy, scintilSlow);

  gMC->SetMaterialProperty(fImedWater, "SCINTILLATIONYIELD", 50.e03);  // 50./MeV
  gMC->SetMaterialProperty(fImedWater, "RESOLUTIONSCALE",  1.0);
  gMC->SetMaterialProperty(fImedWater, "FASTTIMECONSTANT",  1.0e-09);  // 1.*ns
  gMC->SetMaterialProperty(fImedWater, "SLOWTIMECONSTANT", 10.0e-09); // 10.*ns
  gMC->SetMaterialProperty(fImedWater, "YIELDRATIO", 0.8);

//
// Air
//
  Double_t refractiveIndex2[nEntries] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };

  // Added (for Geant3)
  Double_t absorption2[nEntries] =
           { DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, 
             DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, 
             DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, 
             DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX,
	     DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX }; 
	     
  // Added (for Geant3)
  Double_t efficiency2[nEntries] =
            { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00 };

  gMC->SetCerenkov(fImedAir, nEntries, photonEnergy,
                   absorption2, efficiency2, refractiveIndex2); 

//	------------- Surfaces --------------
//
// Water Tank
//
  gMC->DefineOpSurface("WaterSurface", 
                        kUnified, kDielectric_dielectric, kGround, 0.);
			          // CHECK default value of sigma alpha
  gMC->SetBorderSurface("WaterSurface", 
                        "TANK", 1, "WRLD", 0, "WaterSurface");				  
  
// Air Bubble
//
  gMC->DefineOpSurface("AirSurface", 
                        kGlisur, kDielectric_dielectric, kPolished, 0.);
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

