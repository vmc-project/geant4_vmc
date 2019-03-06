//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01RootDetectorConstruction.cxx
/// \brief Implementation of the A01RootDetectorConstruction class
///
/// Geant4 ExampleA01 adapted to Virtual Monte Carlo \n
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>
#include <TGeoManager.h>
#include <TVirtualMC.h>
#include <TList.h>

#include "A01RootDetectorConstruction.h"
#include "A01LocalMagField.h"

/// \cond CLASSIMP
ClassImp(A01RootDetectorConstruction)
/// \endcond

//_____________________________________________________________________________
A01RootDetectorConstruction::A01RootDetectorConstruction(
                                    const TString& geometryFileName)
  : TObject(),
    fGeometryFileName(geometryFileName),
    fUseLocalMagField(false)
{
/// Standard constuctor
/// \param geometryFileName The root geometry file name
}

//_____________________________________________________________________________
A01RootDetectorConstruction::~A01RootDetectorConstruction()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void A01RootDetectorConstruction::ConstructGeometry()
{
/// Contruct volumes using TGeo modeller

  TGeoManager::Import(fGeometryFileName.Data());

  // Update media parameters (needed for Geant3)
  TList* media = gGeoManager->GetListOfMedia();
  TIter next(media);
  while (TObject *obj = next()) {
    TGeoMedium* medium = (TGeoMedium*)obj;
    medium->SetParam(1,2);    // ifield - User defined magnetic field
    medium->SetParam(2,10);   // fieldm - Maximum field value (in kiloGauss)
    medium->SetParam(6,.001); // epsil - Tracking precision
  }

  // Create local magnetic field
  if ( fUseLocalMagField ) {
    // Constant magnetic field (in kiloGauss)
    // field value: 1.0*tesla (= 10.0 kiloGauss) in y
    TVirtualMagField* magField = new A01LocalMagField(0, 10.0, 0);

    // set the field to "magneticLogical" volume
    TGeoVolume* magneticVolume = gGeoManager->FindVolumeFast("magneticLogical");
    if ( magneticVolume ) {
      magneticVolume->SetField(magField);
    }
    else {
      std::cerr << "The Tube volume was not found in geometry." << std::endl;
      std::cerr << "Local magnetic field has not been set." << std::endl;
      delete magField;
    }
  }
  // notify VMC about Root geometry
  gMC->SetRootGeometry();
}
