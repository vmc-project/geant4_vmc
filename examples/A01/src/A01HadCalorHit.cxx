// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2012 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file A01HadCalorHit.cxx 
/// \brief Implementation of the A01HadCalorHit class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo \n
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>

#include "A01HadCalorHit.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(A01HadCalorHit)
/// \endcond

//_____________________________________________________________________________
A01HadCalorHit::A01HadCalorHit(Int_t icol, Int_t irow) 
  : TObject(),
    fColumnID(icol),
    fRowID(irow),
    fEdep(0.),
    fTransformation()
{
/// Standard constructor
/// \param icol  Volume cell column number
/// \param irow  Volume cell row number
}

//_____________________________________________________________________________
A01HadCalorHit::A01HadCalorHit() 
  : TObject(),
    fColumnID(-1),
    fRowID(-1),
    fEdep(0.),
    fTransformation()
{
/// Default constructor
}

//_____________________________________________________________________________
A01HadCalorHit::~A01HadCalorHit() 
{
/// Destructor
}

//_____________________________________________________________________________
void A01HadCalorHit::Print(Option_t* /*option*/) const
{
/// Print hit info

  cout << "  Cell[" << fRowID << ", " << fColumnID << "] " << fEdep*1.0e03 
       << " (MeV) " << endl;
}

//_____________________________________________________________________________
void A01HadCalorHit::Reset()
{
/// Reset all accounted values.

  fColumnID = -1;
  fRowID = -1;
  fEdep = 0.;
  fTransformation = TGeoHMatrix();
}

