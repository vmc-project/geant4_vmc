//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file DetectorConstruction.cxx 
/// \brief Implementation of the DetectorConstruction class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
/// 
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay
 
#include <Riostream.h>
#include <TGeoManager.h>
#include <TGeoElement.h>
#include <TGeoMaterial.h>
#include <TGeoBBox.h>
#include <TGeoVolume.h>
#include <TVirtualMC.h>
#include <TList.h>
#include <TThread.h>

#include "DetectorConstruction.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(Gflash::DetectorConstruction)
/// \endcond

namespace Gflash
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
  new TGeoManager("Gflash_geometry", "Gflash VMC example geometry");

  // Elements
  //
  Double_t z, a;
  TGeoElement* elN  = new TGeoElement("Nitrogen", "N", z=  7., a=  14.01);
  TGeoElement* elO  = new TGeoElement("Oxygen"  , "O", z=  8., a=  16.00);
  TGeoElement* elW  = new TGeoElement("Tungsten", "W", z= 74., a= 183.84);
  TGeoElement* elPb = new TGeoElement("Lead",    "Pb", z= 82., a= 207.2);

  // Materials
  //
  // G4_Air
  Double_t density;
  TGeoMixture* matAir
    = new TGeoMixture("AirA", 2, density=1.29e-03);
  matAir->AddElement(elN, 0.7); 
  matAir->AddElement(elO, 0.3); 

  // G4_Air
  TGeoMixture* matAir2
    = new TGeoMixture("AirB", 2, density=1.29e-03);
  matAir2->AddElement(elN, 0.7); 
  matAir2->AddElement(elO, 0.3); 

  // G4_PbWO4
  TGeoMixture* matPbWO4
    = new TGeoMixture("PbWO4", 3, density=8.28);
  matPbWO4->AddElement(elO,  4);
  matPbWO4->AddElement(elW,  1);
  matPbWO4->AddElement(elPb, 1);

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
  TGeoMedium* medAir = new TGeoMedium("AirA", ++mediumId, matAir, param); 
  TGeoMedium* medAir2 = new TGeoMedium("AirB", ++mediumId, matAir2, param); 
  TGeoMedium* medPbWO4 = new TGeoMedium("PbWO4", ++mediumId, matPbWO4, param); 
 
  // Volumes
  //

  // The Experimental Hall 
  Double_t experimentalHall_x=1000.;
  Double_t experimentalHall_y=1000.;
  Double_t experimentalHall_z=1000.;

  TGeoShape* experimentalHall_box 
    = new TGeoBBox("expHall_box", 
                   experimentalHall_x,        // x size
                   experimentalHall_y,        // y size
                   experimentalHall_z);       // z size

  TGeoVolume* experimentalHall_log 
    = new TGeoVolume("ExpHall_log", experimentalHall_box, medAir);
  gGeoManager->SetTopVolume(experimentalHall_log);

  //------------------------------ 
  // Calorimeter segments
  //------------------------------
  // Simplified `CMS-like` PbWO4 crystal calorimeter  
  
  Int_t nbOfCrystals = 10;  // this are the crystals PER ROW in this example 
                            // cube of 10 x 10 crystals 
                            // don't change it @the moment, since 
                            // the readout in event action assumes this 
                            // dimensions and is not automatically adapted
                            // in this version of the example :-( 
  Double_t crystalWidth = 3.;
  Double_t crystalLenght= 24.;
  Double_t calo_xside=(crystalWidth*nbOfCrystals)+1.;
  Double_t calo_yside=(crystalWidth*nbOfCrystals)+1.;
  Double_t calo_zside=crystalLenght;
  
  TGeoShape* calo_box
    = new TGeoBBox("CMS_calorimeter",  // its name
                   calo_xside/2.,      // size
                   calo_yside/2.,
                   calo_zside/2.);
  TGeoVolume* calo_log 
    = new TGeoVolume("Calo_log", calo_box, medAir2);
  
  Double_t Xpos = 0.0;
  Double_t Ypos = 0.0;
  Double_t Zpos = 100.0;

     // TGeoRotation* rot = new TGeoRotation();
     // rot->RotateY(-phi);
        // !!! Different meaning of rotation in Root than in Geant4

  experimentalHall_log
    ->AddNode(calo_log, 1, new TGeoTranslation(Xpos,Ypos,Zpos));

  TGeoShape*  crystal_box 
    = new TGeoBBox("Crystal",
                   crystalWidth/2,
                   crystalWidth/2,
                   crystalLenght/2); 
  TGeoVolume* crystal_log 
    = new TGeoVolume("Crystal_log", crystal_box, medPbWO4);

  for (Int_t i=0; i<nbOfCrystals; i++) {
    for (Int_t j=0; j<nbOfCrystals; j++) {  
      Int_t n = i*nbOfCrystals + j;

      Double_t crystalPos_x = (i*crystalWidth) - 13.5;
      Double_t crystalPos_y = (j*crystalWidth) - 13.5;
      Double_t crystalPos_z = 0;
      calo_log
        ->AddNode(crystal_log, n, 
                  new TGeoTranslation(crystalPos_x, crystalPos_y, crystalPos_z));
    }
  }

  // close geometry
  gGeoManager->CloseGeometry();
    
  // notify VMC about Root geometry
  gMC->SetRootGeometry();
  
  cout << "There are " << nbOfCrystals 
       << " crystals per row in the calorimeter, so in total "
       << nbOfCrystals*nbOfCrystals << " crystals" << endl;  
  cout << "The have width of " << crystalWidth 
       << " cm and a lenght of " <<  crystalLenght
       << " cm. The Material is PbWO4" << endl;
}    

/*
//_____________________________________________________________________________
void DetectorConstruction::ConstructSDandField()
{
}
*/

}
