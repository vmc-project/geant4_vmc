#ifndef GFLASH_DETECTOR_CONSTRUCTION_H
#define GFLASH_DETECTOR_CONSTRUCTION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Gflash/include/DetectorConstruction.h
/// \brief Definition of the Gflash::DetectorConstruction class
///
/// Geant4 gflash example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include <map>

#include <Riostream.h>
#include <TObject.h>
#include <TString.h>

namespace VMC
{
namespace Gflash
{

/// \ingroup Gflash
/// \brief The detector construction (via TGeo )
///
/// Geant4 gflash example adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

class DetectorConstruction : public TObject
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    void Construct();
    // void ConstructSDandField();

    // const G4VPhysicalVolume* GetCristal(int num__crystal)
    // {return fCrystal_phys[num__crystal];};

  ClassDef(DetectorConstruction,1) //DetectorConstruction
};

}
}

#endif //GFLASH_DETECTOR_CONSTRUCTION_H
