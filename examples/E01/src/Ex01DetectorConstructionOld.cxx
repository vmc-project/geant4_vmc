// $Id: Ex01DetectorConstructionOld.cxx,v 1.3 2006/12/15 09:30:53 brun Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
//
// Id: Ex01DetectorConstructionOld.cc,v 1.11 2002/01/09 17:24:09 ranjard Exp 
// GEANT4 tag Name: geant4-04-00-patch-02 
//
// by Ivana Hrivnacova, 21.4.2002
 
#include <iostream>

#include <TVirtualMC.h>

#include "Ex01DetectorConstructionOld.h"

ClassImp(Ex01DetectorConstructionOld)

using namespace std;

//_____________________________________________________________________________
Ex01DetectorConstructionOld::Ex01DetectorConstructionOld()
  : TObject(),
    fImedAr(0),
    fImedAl(0),
    fImedPb(0)
{
//
}

//_____________________________________________________________________________
Ex01DetectorConstructionOld::~Ex01DetectorConstructionOld()
{
//
}

//_____________________________________________________________________________
void Ex01DetectorConstructionOld::ConstructMaterials()
{
  //
  // Materials
  //

  Double_t a;
  Double_t z;
  Double_t density;
  Double_t radl;
  Double_t absl;
  Float_t* ubuf = 0;

  a = 39.95;
  z = 18.;
  density = 1.782e-03;
  radl = 0.1;
  absl = 0.1;
  Int_t imatAr;
  gMC->Material(imatAr, "ArgonGas", a, z, density, radl, absl, ubuf, 0);

  a = 26.98;
  z = 13.;
  density = 2.7;
  radl = 0.1;
  absl = 0.1;
  Int_t imatAl;
  gMC->Material(imatAl, "Aluminium", a, z, density, radl, absl, ubuf, 0);
  
  a = 207.19;
  z = 82.;
  density = 11.35;
  radl = 0.1;
  absl = 0.1;
  Int_t imatLead;  
  gMC->Material(imatLead, "Lead", a, z, density, radl, absl, ubuf, 0);  

  //
  // Tracking medias
  //

  Int_t ifield = 0;          // No magnetic field
  Double_t fieldm = 0.;      //
  Double_t epsil  = .001;    // Tracking precision, 
  Double_t stemax = -0.01;   // Maximum displacement for multiple scat 
  Double_t tmaxfd = -20.;    // Maximum angle due to field deflection 
  Double_t deemax = -.3;     // Maximum fractional energy loss, DLS 
  Double_t stmin  = -.8;
  gMC->Medium(fImedAr, "ArgonGas",  imatAr, 0, ifield, fieldm, tmaxfd, stemax, deemax, epsil, stmin, ubuf, 0); 
  gMC->Medium(fImedAl, "Aluminium", imatAl, 0, ifield, fieldm, tmaxfd, stemax, deemax, epsil, stmin, ubuf, 0); 
  gMC->Medium(fImedPb, "Lead", imatLead, 0, ifield, fieldm, tmaxfd, stemax, deemax, epsil, stmin, ubuf, 0); 
}    

//_____________________________________________________________________________
void Ex01DetectorConstructionOld::ConstructVolumes()
{

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along x axis

  Double_t expHall[3];
  expHall[0] = 300.;
  expHall[1] = 100.;
  expHall[2] = 100.;
  gMC->Gsvolu("EXPH","BOX", fImedAr, expHall, 3);
 
  //------------------------------ a tracker tube

  Double_t trackerTube[3];
  trackerTube[0] = 0.;
  trackerTube[1] = 60.;
  trackerTube[2] = 50.;
  gMC->Gsvolu("TRTU","TUBE", fImedAl, trackerTube, 3);

  Double_t posX = -100.;
  Double_t posY =  0.;
  Double_t posZ =  0.;
  gMC->Gspos("TRTU", 1 ,"EXPH", posX, posY, posZ, 0, "ONLY");
  
  //------------------------------ a calorimeter block

  Double_t calBox[3];
  calBox[0] = 100.;
  calBox[1] = 50.;
  calBox[2] = 50.;
  gMC->Gsvolu("CALB","BOX", fImedPb, calBox, 3);

  posX = 100.;
  posY = 0.;
  posZ = 0.;
  gMC->Gspos("CALB", 1 ,"EXPH", posX, posY, posZ, 0, "ONLY");

  //------------------------------ calorimeter layers
 
  Double_t layerBox[3];
  layerBox[0] = 1.;
  layerBox[1] = 40.;
  layerBox[2] = 40.;
  gMC->Gsvolu("LAYB","BOX", fImedAl, layerBox, 3);

  for (Int_t i=0; i<19; i++) {
    posX = (i-9) * 10.;
    posY = 0.;
    posZ = 0.;
    gMC->Gspos("LAYB", i ,"CALB", posX, posY, posZ, 0, "ONLY");
  }  
}
