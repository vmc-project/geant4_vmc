// $Id$
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration1
// --------------------------
// See the class description in the header file.

#include "Ex03RunConfiguration1.h"
#include "ExN03DetectorConstruction.hh"

//_____________________________________________________________________________
Ex03RunConfiguration1::Ex03RunConfiguration1(const TString& physicsList,
                                             const TString& specialProcess)
  : TG4RunConfiguration("geomGeant4", physicsList, specialProcess) {
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
