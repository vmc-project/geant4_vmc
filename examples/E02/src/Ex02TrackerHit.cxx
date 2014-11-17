//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex02TrackerHit.cxx 
/// \brief Implementation of the Ex02TrackerHit class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo \n
/// Id: Ex02TrackerHit.cc,v 1.7 2002/01/09 17:24:10 ranjard Exp \n 
/// GEANT4 tag Name: geant4-04-00-patch-02 
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include <iostream>

#include "Ex02TrackerHit.h"

/// \cond CLASSIMP
ClassImp(Ex02TrackerHit)
/// \endcond

using namespace std;

//_____________________________________________________________________________
Ex02TrackerHit::Ex02TrackerHit() 
  : fTrackID(-1),
    fChamberNb(-1),
    fEdep(0.),
    fPos()
{
/// Default constructor
}

//_____________________________________________________________________________
Ex02TrackerHit::~Ex02TrackerHit() 
{
/// Destructor
}

/*
//_____________________________________________________________________________
void Ex02TrackerHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(0.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}
*/

//_____________________________________________________________________________
void Ex02TrackerHit::Print(const Option_t* /*opt*/) const
{
/// Printing

  cout << "  trackID: " << fTrackID 
       << "  chamberNb: " << fChamberNb
       << "  energy deposit (keV): " << fEdep * 1.0e06
       << "  position (cm): (" 
       << fPos[0] << ", " << fPos[1] << ", " << fPos[2] << ")"
       << endl;
}

