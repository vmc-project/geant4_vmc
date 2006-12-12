// $Id: Ex03RunConfiguration1.cxx,v 1.3 2006/04/12 10:39:33 brun Exp $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration1
// --------------------------
// See the class description in the header file.

#include "Ex03RunConfiguration1.h"
#include "ExN03DetectorConstruction.hh"

#include <LHEP_BERT.hh>

//_____________________________________________________________________________
Ex03RunConfiguration1::Ex03RunConfiguration1()
  : TG4RunConfiguration("geomGeant4") {
//
}

//_____________________________________________________________________________
Ex03RunConfiguration1::~Ex03RunConfiguration1(){
//
}

//
// protected methods
//

//_____________________________________________________________________________
G4VUserDetectorConstruction*  Ex03RunConfiguration1::CreateDetectorConstruction()
{
// Create detector construction

  return new ExN03DetectorConstruction();
}   
