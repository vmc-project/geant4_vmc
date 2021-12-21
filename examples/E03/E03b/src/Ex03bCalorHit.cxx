//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03bCalorHit.cxx
/// \brief Implementation of the Ex03bCalorHit class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
/// Id: ExN03CalorHit.cc,v 1.5 2002/01/09 17:24:12 ranjard Exp \n
/// GEANT4 tag Name: geant4-05-00
///
/// \date 06/03/2002
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>

#include "Ex03bCalorHit.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(Ex03bCalorHit)
  /// \endcond

//_____________________________________________________________________________
Ex03bCalorHit::Ex03bCalorHit()
: TObject(),
  fEdep(0.),
  fTrackLength(0.)
{
  /// Default constructor
}

//_____________________________________________________________________________
Ex03bCalorHit::~Ex03bCalorHit()
{
  /// Destructor
}

//_____________________________________________________________________________
void Ex03bCalorHit::Print(Option_t* /*option*/) const
{
  /// Print hit info

  cout << "Energy deposit (keV): " << fEdep * 1.0e06 << endl
       << "   track length (cm): " << fTrackLength << endl;
}

//_____________________________________________________________________________
void Ex03bCalorHit::Reset()
{
  /// Reset all accounted values.

  fEdep = 0.;
  fTrackLength = 0.;
}
