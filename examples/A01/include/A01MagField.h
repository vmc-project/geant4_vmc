#ifndef A01_MAG_FIELD_H
#define A01_MAG_FIELD_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2012 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file  A01MagField.h
/// \brief Definition of the A01MagField class 
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

class A01MagField : public TVirtualMagField
{
public:
   A01MagField(Double_t Bx, Double_t By, Double_t Bz); 
   A01MagField();
   virtual ~A01MagField();
   
   virtual void Field(const Double_t* /*x*/, Double_t* B);
   
private:
   A01MagField(const A01MagField&);
   A01MagField& operator=(const A01MagField&);
   
   Double_t  fB[3]; ///< Magnetic field vector

   ClassDef(A01MagField, 1)  // Uniform magnetic field        
};

#endif //A01_MAG_FIELD_H
