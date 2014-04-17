// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2012 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file A01DriftChamberHit.cxx 
/// \brief Implementation of the A01DriftChamberHit class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>

#include "A01DriftChamberHit.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(A01DriftChamberHit)
/// \endcond

//_____________________________________________________________________________
A01DriftChamberHit::A01DriftChamberHit(Int_t z) 
  : TObject(),
    fLayerID(z),
    fTime(0.),
    fLocalPos(),
    fWorldPos()
{
/// Standard constructor
/// \param z  The layer id
}

//_____________________________________________________________________________
A01DriftChamberHit::A01DriftChamberHit() 
  : TObject(),
    fLayerID(-1),
    fTime(0.),
    fLocalPos(),
    fWorldPos()
{
/// Default constructor
}

//_____________________________________________________________________________
A01DriftChamberHit::~A01DriftChamberHit() 
{
/// Destructor
}

//_____________________________________________________________________________
void A01DriftChamberHit::Print(Option_t* /*option*/) const
{
/// Print hit info

  cout << "  Layer[" << fLayerID << "] : time " << fTime*1e+09 
       << " (nsec) --- local (x,y) " << fLocalPos.X()*10. << ", " << fLocalPos.Y()*10 
       << " (mm)" << endl;
}

//_____________________________________________________________________________
void A01DriftChamberHit::Reset()
{
/// Reset all accounted values.

  fLayerID = -1;
  fTime = 0.;
  fLocalPos = TVector3();
  fWorldPos = TVector3();
}

