#ifndef EX01_DETECTOR_CONSTRUCTION_OLD_H
#define EX01_DETECTOR_CONSTRUCTION_OLD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex01DetectorConstructionOld.h
/// \brief Definition of the Ex01DetectorConstructionOld class
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo \n
/// Id: ExN02DetectorConstruction.hh,v 1.7 2002/01/09 17:24:09 ranjard Exp \n
/// GEANT4 tag Name: geant4-04-00-patch-02
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TString.h>

/// \ingroup E01
/// \brief The old (deprecated) detector construction class
///
/// Geometry definition via VMC (for testing purposes only).
/// The recomended way of geometry definition is using TGeo
/// geometry modeller.
///
/// \date 21/04/2002
/// Separated from Ex01MCAopplication, 08/12/2006
///
/// \author I. Hrivnacova; IPN, Orsay

class Ex01DetectorConstructionOld : public TObject
{
 public:
  Ex01DetectorConstructionOld();
  virtual ~Ex01DetectorConstructionOld();

  // methods
  void ConstructMaterials();
  void ConstructVolumes();

 private:
  // data members
  Int_t fImedAr; ///< The Argon medium Id
  Int_t fImedAl; ///< The Aluminium medium Id
  Int_t fImedPb; ///< The Lead medium Id

  ClassDef(Ex01DetectorConstructionOld, 1) // Ex01DetectorConstructionOld
};

#endif // EX01_DETECTOR_CONSTRUCTION_OLD_H
