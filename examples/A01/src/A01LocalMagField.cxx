//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  A01LocalMagField.cxx
/// \brief Implementation of the A01LocalMagField class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TMath.h>

#include "A01LocalMagField.h"

/// \cond CLASSIMP
ClassImp(A01LocalMagField)
/// \endcond

//______________________________________________________________________________
A01LocalMagField::A01LocalMagField(Double_t Bx, Double_t By, Double_t Bz)
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
A01LocalMagField::A01LocalMagField()
  : TVirtualMagField()
{
/// Default constructor
   fB[0] = 0.;
   fB[1] = 0.;
   fB[2] = 0.;
}

//______________________________________________________________________________
A01LocalMagField::~A01LocalMagField()
{
/// Destructor
}


//______________________________________________________________________________
void A01LocalMagField::Field(const Double_t* /*x*/, Double_t* B) 
{
/// Fill in the field value B in the given position at x.
/// The local field is uniform in the associated volume.
/// \param x   The position
/// \param B   The field value (in kiloGauss) 

  B[0] = fB[0]; 
  B[1] = fB[1]; 
  B[2] = fB[2];
}
