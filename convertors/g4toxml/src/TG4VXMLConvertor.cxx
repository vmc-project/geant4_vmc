// $Id: TG4VXMLConvertor.cxx,v 1.2 2004/04/26 17:05:04 brun Exp $
//
// Author: I. Hrivnacova, 27.07.2000 
//
// Class TG4VXMLConvertor 
// ----------------------
// See the class description in the header file.

#include <math.h>

#include "TG4VXMLConvertor.h"

//_____________________________________________________________________________
TG4VXMLConvertor::TG4VXMLConvertor() {
//
}

//_____________________________________________________________________________
TG4VXMLConvertor::~TG4VXMLConvertor() {
//
}

//_____________________________________________________________________________
G4double TG4VXMLConvertor::Round(G4double x) const
{
// Replacement for round(double) function from math.h
// which is not available for gcc 2.95.x
// ---

  G4double t;
  if (x >= 0.0) {
    t = ceil(x);
    if (t - x > 0.5) t -= 1.0;
    return t;
  } 
  else {
    t = ceil(-x);
    if (t + x > 0.5) t -= 1.0;
    return -t;
  }
}
