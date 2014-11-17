//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Ex02MagField.cxx
/// \brief Implementation of the Ex02MagField class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay


#include "Ex02MagField.h"

/// \cond CLASSIMP
ClassImp(Ex02MagField)
/// \endcond

//______________________________________________________________________________
Ex02MagField::Ex02MagField(Double_t Bx, Double_t By, Double_t Bz)
  : TVirtualMagField("Ex02 magnetic field")
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
Ex02MagField::Ex02MagField()
  : TVirtualMagField()
{
/// Default constructor
   fB[0] = 0.;
   fB[1] = 0.;
   fB[2] = 0.;
}

//______________________________________________________________________________
Ex02MagField::~Ex02MagField()
{
/// Destructor
}


//______________________________________________________________________________
void Ex02MagField::Field(const Double_t* /*x*/, Double_t* B) 
{
/// Fill in the field value B in the given position at x.
/// (In case of a uniform magnetic field the value B does not depend on 
/// the position x ) 
/// \param x   The position
/// \param B   he field value (in kiloGauss) 


  //cout << "+++ Getting field in " 
  //     << gMC->CurrentVolName() << " "  << gMC->CurrentMedium() << endl;
       
  B[0] = fB[0]; 
  B[1] = fB[1]; 
  B[2] = fB[2];
}
