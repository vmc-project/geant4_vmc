// $Id: TG4Polycone.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: geometry
//
// Author: I. Hrivnacova, 12.10.2000 
//
// Class TG4Polycone
// -----------------
// See the class description in the header file.

#include "TG4Polycone.h"

#include <globals.hh>

#include <stdlib.h>

//_____________________________________________________________________________
TG4Polycone::TG4Polycone(const G4Polycone& rhs) 
  : G4Polycone(rhs) {
//
}

//_____________________________________________________________________________
TG4Polycone::~TG4Polycone() {
//
}

// private methods

//_____________________________________________________________________________
void TG4Polycone::CheckOrigin() 
{
// Checks if polycone was created in a "historical way"
// and give exception otherwise.
// ---

  if (!original_parameters) {
    G4cerr << "    TG4Polycone::CheckOrigin: " << G4endl;
    G4cerr << "    Polycone has not defined original parameters.";
    G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
    exit(1);
  }  
}


// public methods

//_____________________________________________________________________________
G4int TG4Polycone::GetNofZPlanes()
{
// Returns nof z planes.
// ----
  
  CheckOrigin();

  return original_parameters->Num_z_planes;
}  

//_____________________________________________________________________________
G4double* TG4Polycone::GetRmin()
{
// Returns array of rmin parameters of the planes.
// ----
  
  CheckOrigin();

  return original_parameters->Rmin;
}  

//_____________________________________________________________________________
G4double* TG4Polycone::GetRmax()
{
// Returns array of rmax parameters of the planes.
// ----

  CheckOrigin();
  
  return original_parameters->Rmax;
}  

//_____________________________________________________________________________
G4double* TG4Polycone::GetZ()
{
// Returns array of z parameters of the planes.
// ----

  CheckOrigin();
  
  return original_parameters->Z_values;
}  
