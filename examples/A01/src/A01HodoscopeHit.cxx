// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2012 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file A01HodoscopeHit.cxx 
/// \brief Implementation of the A01HodoscopeHit class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo \n
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>

#include "A01HodoscopeHit.h"

/// \cond CLASSIMP
ClassImp(A01HodoscopeHit)
/// \endcond

//_____________________________________________________________________________
A01HodoscopeHit::A01HodoscopeHit(Int_t id, Double_t t) 
  : TObject(),
    fID(id),
    fVolID(-1),
    fTime(t),
    fTransformation()
{
/// Standard constructor
/// \param id  Hit volume id
/// \param t   Hit time
}

//_____________________________________________________________________________
A01HodoscopeHit::A01HodoscopeHit() 
  : TObject(),
    fID(-1),
    fVolID(-1),
    fTime(0),
    fTransformation()
{
/// Default constructor
}

//_____________________________________________________________________________
A01HodoscopeHit::~A01HodoscopeHit() 
{
/// Destructor
}

//_____________________________________________________________________________
void A01HodoscopeHit::Print(Option_t* /*option*/) const
{
/// Print hit info

  cout << "  Hodoscope[" << fID << "] " << fTime*1.0e+09 << " [nsec]" << endl;
}

//_____________________________________________________________________________
void A01HodoscopeHit::Reset()
{
/// Reset all accounted values.

  fID = -1;
  fVolID = -1;
  fTime = 0.;
  fTransformation = TGeoHMatrix();
}

