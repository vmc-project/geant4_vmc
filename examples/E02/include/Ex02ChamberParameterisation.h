#ifndef Ex02ChamberParameterisation_H
#define Ex02ChamberParameterisation_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex02ChamberParameterisation.h
/// \brief Definition of the Ex02ChamberParameterisation class
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo \n
/// Id: Ex02ChamberParameterisation.hh,v 1.6 2002/01/09 17:24:08 ranjard Exp \n
/// GEANT4 tag Name: geant4-04-00-patch-02
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>

/// \ingroup E02
/// \brief The chamber parameterisation
///
/// A parameterisation that describes a series of boxes along Z
/// The boxes have equal width, & their lengths are a linear equation.
//  They are spaced an equal distance apart, starting from given location.
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex02ChamberParameterisation : public TObject
{
  public:
    Ex02ChamberParameterisation(Int_t    noChambers,
                                Double_t startZ,
                                Double_t spacingZ,
                                Double_t widthChamber,
                                Double_t lengthInitial,
                                Double_t lengthFinal );

    virtual ~Ex02ChamberParameterisation();

    // methods
    void ComputeTransformation (Int_t copyNo, Double_t* position) const;
    void ComputeDimensions (Int_t copyNo, Double_t* dimension) const;


  private:
    Int_t     fNoChambers;      ///< Number of chambers
    Double_t  fStartZ;          ///< The Z of the center of first chamber
    Double_t  fHalfWidth;       ///< The half-width of each tracker chamber
    Double_t  fSpacing;         ///< The distance between the chambers' centers
    Double_t  fHalfLengthFirst; ///< The first half-length
    Double_t  fHalfLengthIncr;  ///< The Increment for the half-length

  ClassDef(Ex02ChamberParameterisation,1) //Ex02ChamberParameterisation
};

#endif


