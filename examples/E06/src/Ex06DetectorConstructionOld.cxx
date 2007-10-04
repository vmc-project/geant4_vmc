// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

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

#include "Ex06DetectorConstructionOld.h"

ClassImp(Ex06DetectorConstructionOld)

//_____________________________________________________________________________
Ex06DetectorConstructionOld::Ex06DetectorConstructionOld()
  : TObject()
{
    fExpHallSize = 1000;  // 10*m
    fTankSize    = 500;   // 5*m
    fBubbleSize  = 50;    // 0.5*m
}

//_____________________________________________________________________________
Ex06DetectorConstructionOld::~Ex06DetectorConstructionOld()
{
// Destructor
}


//
// public methods
//

//_____________________________________________________________________________
void Ex06DetectorConstructionOld::ConstructMaterials()
{
   
  Double_t a1[2] = { 14.01, 16.00};
  Double_t z1[2] = {  7.0,   8.0};
  Double_t w1[2] = {  0.7,   0.3};
  Double_t density = 1.29e-03;  
  Int_t imatAir;
  gMC->Mixture(imatAir, "Air", a1, z1, density, 2, w1); 

  Double_t a2[2] = {  1.01, 16.00};
  Double_t z2[2] = {  1.0,   8.0};
  Double_t w2[2] = {  2.0,   1.0};
  density = 1.0;  
  Int_t imatWater;
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
void Ex06DetectorConstructionOld::ConstructGeometry()
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
    
