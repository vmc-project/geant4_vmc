#ifndef EX05_DETECTOR_CONSTRUCTION_H
#define EX05_DETECTOR_CONSTRUCTION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Ex05DetectorConstruction.h
/// \brief Definition of the Ex05DetectorConstruction class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include <map>

#include <Riostream.h>
#include <TObject.h>
#include <TString.h>

/// \ingroup E05
/// \brief The detector construction (via TGeo )
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

class Ex05DetectorConstruction : public TObject
{
  public:  
    Ex05DetectorConstruction();
    virtual ~Ex05DetectorConstruction();

  public:
    void Construct();
    // void ConstructSDandField();

    // const G4VPhysicalVolume* GetCristal(int num__crystal)
    // {return fCrystal_phys[num__crystal];};

  ClassDef(Ex05DetectorConstruction,1) //Ex05DetectorConstruction
};

#endif //EX05_DETECTOR_CONSTRUCTION_H
