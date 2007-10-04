// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 ExampleN03 adapted to Virtual Monte Carlo 
//
// Id: ExN03CalorHit.cc,v 1.5 2002/01/09 17:24:12 ranjard Exp 
// GEANT4 tag Name: geant4-05-00 
//
// by Ivana Hrivnacova, 6.3.2003

#include <Riostream.h>

#include "Ex03CalorHit.h"

ClassImp(Ex03CalorHit)

//_____________________________________________________________________________
Ex03CalorHit::Ex03CalorHit() 
  : TObject(),
    fEdepAbs(0.),
    fTrackLengthAbs(0.),
    fEdepGap(0.),
    fTrackLengthGap(0.)
{}

//_____________________________________________________________________________
Ex03CalorHit::~Ex03CalorHit() 
{}

//_____________________________________________________________________________
void Ex03CalorHit::Print(Option_t* /*option*/) const
{
// Prints hit info
// ---

  cout << "In absorber: " << endl
       << "   energy deposit (keV): " << fEdepAbs * 1.0e06 << endl
       << "   track length (cm): " << fTrackLengthAbs  << endl
       << "In gap: " << endl
       << "   energy deposit (keV): " << fEdepGap * 1.0e06 << endl
       << "   track length (cm): " << fTrackLengthGap  << endl;
}

//_____________________________________________________________________________
void Ex03CalorHit::Reset()
{
// Reset all accounted values.
// ---

  fEdepAbs = 0.;
  fTrackLengthAbs = 0.;
  fEdepGap = 0.;
  fTrackLengthGap = 0.;
}

