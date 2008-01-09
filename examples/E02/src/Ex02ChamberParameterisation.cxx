// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex02ChamberParameterisation.cxx 
/// \brief Implementation of the Ex02ChamberParameterisation class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo \n
/// Id: Ex02ChamberParameterisation.cc,v 1.7 2002/01/09 17:24:09 ranjard Exp \n
/// GEANT4 tag Name: geant4-04-00-patch-02 
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include <TError.h>

#include "Ex02ChamberParameterisation.h"

/// \cond CLASSIMP
ClassImp(Ex02ChamberParameterisation)
/// \endcond

//_____________________________________________________________________________
Ex02ChamberParameterisation::Ex02ChamberParameterisation(
                                 Int_t    noChambers, 
                                 Double_t startZ,      
                                 Double_t spacingZ,   
                                 Double_t widthChamber, 
                                 Double_t lengthInitial, 
                                 Double_t lengthFinal )
  : TObject()				 
{
/// Standard constructor
/// \param noChambers    Number of chambers
/// \param startZ        The Z of the center of first chamber
/// \param spacingZ      The distance between the chambers' centers
/// \param widthChamber  The width of each tracker chamber
/// \param lengthInitial The first chamber length 
/// \param lengthFinal   The last chamber length 

   fNoChambers =  noChambers; 
   fStartZ     =  startZ; 
   fHalfWidth  =  widthChamber*0.5;
   fSpacing    =  spacingZ;
   fHalfLengthFirst = 0.5 * lengthInitial; 
   // fHalfLengthLast = lengthFinal;
   if( noChambers > 0 ){
      fHalfLengthIncr =  0.5 * (lengthFinal-lengthInitial)/noChambers;
      if (spacingZ < widthChamber) {
        Fatal("Ex02ChamberParameterisation",
              "Ex02ChamberParameterisation construction: Width>Spacing");
      }
   }
   
}

//_____________________________________________________________________________
Ex02ChamberParameterisation::~Ex02ChamberParameterisation()
{
/// Destructor
}


//_____________________________________________________________________________
void Ex02ChamberParameterisation::ComputeTransformation (
                                     Int_t copyNo, Double_t* position) const
{
/// Compute the chamber position
/// \param copyNo    The chamber copy number
/// \param position  The computed chamber position

  position[0] = 0.;
  position[1] = 0.;
  position[2] = fStartZ + (copyNo+1) * fSpacing;
}

//_____________________________________________________________________________
void Ex02ChamberParameterisation::ComputeDimensions(
                                      Int_t copyNo, Double_t* dimension) const
{

/// Compute the chamber dimensions
/// \param copyNo     The chamber copy number
/// \param dimension  The computed chamber dimensions

  Double_t  halfLength= fHalfLengthFirst + copyNo * fHalfLengthIncr;
  dimension[0] = halfLength;
  dimension[1] = halfLength;
  dimension[2] = fHalfWidth;
}
