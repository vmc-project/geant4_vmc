#ifndef EX02_MAG_FIELD_H
#define EX02_MAG_FIELD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Ex02MagField.h
/// \brief Definition of the Ex02MagField class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo
///
/// \date 18/09/2009
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMagField.h>

/// \ingroup E02
/// \brief Definition of a uniform magnetic field
///
/// This class demonstrates definition of a user magnetic field
/// via the TVirtualMagField interface.
/// In case of a uniform magnetic field, it is also possible to use
/// directly the TGeoUniformMagField class in the VMC application.
///
/// \date 
/// \author I. Hrivnacova; IPN, Orsay

class Ex02MagField : public TVirtualMagField
{
public:
   Ex02MagField(Double_t Bx, Double_t By, Double_t Bz); 
   Ex02MagField();
   virtual ~Ex02MagField();
   
   virtual void Field(const Double_t* x, Double_t* B);
   
private:
   Ex02MagField(const Ex02MagField&);
   Ex02MagField& operator=(const Ex02MagField&);
   
   Double_t  fB[3]; ///< Magnetic field vector

   ClassDef(Ex02MagField, 1)  // Uniform magnetic field        
};

#endif //EX02_MAG_FIELD_H
