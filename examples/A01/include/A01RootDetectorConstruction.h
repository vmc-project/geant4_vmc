#ifndef A01_ROOT_DETECTOR_CONSTRUCTION_H
#define A01_ROOT_DETECTOR_CONSTRUCTION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  A01RootDetectorConstruction.h
/// \brief Definition of the A01RootDetectorConstruction class
///
/// Geant4 example A01 adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TString.h>

/// \ingroup A01
/// \brief The detector construction (via TGeo )
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

class A01RootDetectorConstruction : public TObject
{
 public:
  A01RootDetectorConstruction(const TString& geometryFileName);
  virtual ~A01RootDetectorConstruction();

 public:
  void ConstructGeometry();

  void SetUseLocalMagField(Bool_t localMagField);

 private:
  // data members

  TString fGeometryFileName; ///< The root geometry file name

  /// Option to use local magnetic field (working only with Geant4 !)
  Bool_t fUseLocalMagField;

  ClassDef(A01RootDetectorConstruction, 1) // A01RootDetectorConstruction
};

// inline functions

/// Set the option to use local magnetic field (working only with Geant4 !)
/// \param localMagField  The new value of the option
inline void A01RootDetectorConstruction::SetUseLocalMagField(
  Bool_t localMagField)
{
  fUseLocalMagField = localMagField;
}

#endif // A01_DETECTOR_CONSTRUCTION_H
