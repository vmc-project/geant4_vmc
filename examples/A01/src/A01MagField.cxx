//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  A01MagField.cxx
/// \brief Implementation of the A01MagField class
///
/// Geant4 example A01 adapted to Virtual Monte Carlo \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TMath.h>

#include "A01MagField.h"

/// \cond CLASSIMP
ClassImp(A01MagField)
  /// \endcond

  //______________________________________________________________________________
  A01MagField::A01MagField(Double_t Bx, Double_t By, Double_t Bz)
  : TVirtualMagField("A01 magnetic field")
{
  /// Standard constructor
  /// \param Bx   The x component of the field value (in kiloGauss)
  /// \param By   The y component of the field value (in kiloGauss)
  /// \param Bz   The z component of the field value (in kiloGauss)

  fB[0] = Bx;
  fB[1] = By;
  fB[2] = Bz;
}

//______________________________________________________________________________
A01MagField::A01MagField() : TVirtualMagField()
{
  /// Default constructor
  fB[0] = 0.;
  fB[1] = 0.;
  fB[2] = 0.;
}

//______________________________________________________________________________
A01MagField::~A01MagField()
{
  /// Destructor
}

//______________________________________________________________________________
void A01MagField::Field(const Double_t* x, Double_t* B)
{
  /// Fill in the field value B in the given position at x.
  /// \param x   The position
  /// \param B   The field value (in kiloGauss)

  // G4VSolid* magneticSolid = new
  // G4Tubs("magneticTubs",0.,1.*m,1.*m,0.,360.*deg); G4VSolid* magneticSolid =
  // new G4Tubs("magneticTubs",0.,1.*m,1.*m,0.,360.*deg); G4RotationMatrix*
  // fieldRot = new G4RotationMatrix(); fieldRot->rotateX(90.*deg); new
  // G4PVPlacement(fieldRot,G4ThreeVector(),magneticLogical,
  //                  "magneticPhysical",worldLogical,0,0);

  if (TMath::Sqrt(x[0] * x[0] + x[2] * x[2]) <= 100.00 &&
      TMath::Abs(x[1]) <= 100.00) {
    B[0] = fB[0];
    B[1] = fB[1];
    B[2] = fB[2];
  }
  else {
    B[0] = 0.;
    B[1] = 0.;
    B[2] = 0.;
  }
}
