//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Gflash/src/Hit.cxx
/// \brief Implementation of the Gflash::Hit class
///
/// Geant4 gflash example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>

#include "Hit.h"

/// \cond CLASSIMP
ClassImp(VMC::Gflash::Hit)
/// \endcond

using namespace std;

namespace VMC
{
namespace Gflash
{

//_____________________________________________________________________________
Hit::Hit()
  : TObject(),
    fEdep(0),
    fPos(),
    fCrystalNumber(0)
{
/// Default constructor
}

//_____________________________________________________________________________
Hit::~Hit()
{
/// Destructor
}

//_____________________________________________________________________________
void Hit::Print(Option_t* /*option*/) const
{
/// Print hit info

  cout << "In crystal: " << fCrystalNumber << ":" << endl
       << "   energy deposit (keV): " << fEdep * 1.0e06 << endl;
}

//_____________________________________________________________________________
void Hit::Reset()
{
/// Reset accounted values.

  fEdep = 0.;
  fPos = TVector3();
  fCrystalNumber = 0;
}

}
}
