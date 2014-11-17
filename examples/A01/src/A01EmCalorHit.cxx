//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01EmCalorHit.cxx 
/// \brief Implementation of the A01EmCalorHit class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo \n
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>

#include "A01EmCalorHit.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(A01EmCalorHit)
/// \endcond

//_____________________________________________________________________________
A01EmCalorHit::A01EmCalorHit(Int_t z) 
  : TObject(),
    fCellID(z),
    fVolID(-1),
    fEdep(0.),
    fTransformation()
{
/// Standard constructor
/// \param z   The cell ID
}

//_____________________________________________________________________________
A01EmCalorHit::A01EmCalorHit() 
  : TObject(),
    fCellID(-1),
    fVolID(-1),
    fEdep(0.),
    fTransformation()
{
/// Default constructor
}

//_____________________________________________________________________________
A01EmCalorHit::~A01EmCalorHit() 
{
/// Destructor
}

//_____________________________________________________________________________
void A01EmCalorHit::Print(Option_t* /*option*/) const
{
/// Print hit info

  cout << "  Cell[" << fCellID << "] " << fEdep*1.0e03 << " (MeV)" << endl;
}

//_____________________________________________________________________________
void A01EmCalorHit::Reset()
{
/// Reset all accounted values.

  fCellID = -1;
  fVolID = -1;
  fEdep = 0.;
  fTransformation = TGeoHMatrix();
}

