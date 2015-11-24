//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Garfield/src/Hit.cxx 
/// \brief Implementation of the Garfield::Hit class 
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>

#include "Hit.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(Garfield::Hit)
/// \endcond

namespace Garfield
{

//_____________________________________________________________________________
Hit::Hit() 
  : TObject(),
    fEdepAbs(0),
    fEdepGas(0),
    fTrackLengthAbs(0),
    fAvalancheSize(0),
    fGain(0)
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

  cout << "   Absorber: total energy [keV]: " << std::setw(7) << fEdepAbs * 1.0e06
       << "       total track length: " << std::setw(7) << fTrackLengthAbs <<  endl 
       << "        Gas: total energy [keV]: " << std::setw(7) << fEdepGas * 1.0e06 
       << endl
       << "             avalanche size: " << std::setw(7) << fAvalancheSize 
       << "       gain: " << std::setw(7) << fGain << endl;
}

//_____________________________________________________________________________
void Hit::Reset()
{
/// Reset accounted values.

  fEdepAbs = 0.;
  fEdepGas = 0.;
  fTrackLengthAbs = 0.;
  fAvalancheSize = 0.;
  fGain = 0.;
}

}
