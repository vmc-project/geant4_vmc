#ifndef EX06_DETECTOR_CONSTRUCTION_OLD_H
#define EX06_DETECTOR_CONSTRUCTION_OLD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex06DetectorConstructionOld.h
/// \brief Definition of the Ex06DetectorConstructionOld class
///
/// Geant4 ExampleN06 adapted to Virtual Monte Carlo \n
/// Id: ExN06DetectorConstruction.hh,v 1.4 2003/01/23 15:34:23 maire Exp
/// GEANT4 tag Name: geant4-07-00-cand-01
///
/// \author I. Hrivnacova; IPN, Orsay

#include <map>

#include <Riostream.h>
#include <TObject.h>
#include <TString.h>

/// \ingroup E06
/// \brief The detector construction (via TGeo )
///
/// \date 16/05/2005
/// \author I. Hrivnacova; IPN, Orsay

class Ex06DetectorConstructionOld : public TObject
{
 public:
  Ex06DetectorConstructionOld();
  virtual ~Ex06DetectorConstructionOld();

 public:
  void ConstructMaterials();
  void ConstructGeometry();

 private:
  // data members
  Double_t fWorldSize;   ///< The world size
  Double_t fExpHallSize; ///< The experimental hall size
  Double_t fTankSize;    ///< The tank size
  Double_t fBubbleSize;  ///< The bubble size
  Int_t fImedAir;        ///< The Air medium Id
  Int_t fImedWater;      ///< The Water medium Id

  ClassDef(Ex06DetectorConstructionOld, 1) // Ex06DetectorConstructionOld
};

#endif // EX06_DETECTOR_CONSTRUCTION_OLD_H
