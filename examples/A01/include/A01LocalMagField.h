#ifndef A01_LOCAL_MAG_FIELD_H
#define A01_LOCAL_MAG_FIELD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  A01LocalMagField.h
/// \brief Definition of the A01LocalMagField class
///
/// Geant4 ExampleA01 adapted to Virtual Monte Carlo
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMagField.h>

/// \ingroup A01
/// \brief Definition of a uniform magnetic field within a given region
///
/// \date
/// \author I. Hrivnacova; IPN, Orsay

class A01LocalMagField : public TVirtualMagField
{
 public:
  A01LocalMagField(Double_t Bx, Double_t By, Double_t Bz);
  A01LocalMagField();
  virtual ~A01LocalMagField();

  virtual void Field(const Double_t* /*x*/, Double_t* B);

 private:
  A01LocalMagField(const A01LocalMagField&);
  A01LocalMagField& operator=(const A01LocalMagField&);

  Double_t fB[3]; ///< Magnetic field vector

  ClassDef(A01LocalMagField, 1) // Uniform magnetic field
};

#endif // A01_LOCAL_MAG_FIELD_H
