// $Id: TG4Polyhedra.cxx,v 1.2 2003/01/29 11:24:44 brun Exp $
// Category: geometry
//
// Author: I. Hrivnacova, 12.10.2000 
//
// Class TG4Polyhedra
// ------------------
// See the class description in the header file.

#include "TG4Polyhedra.h"

#include <globals.hh>

#include <stdlib.h>

//_____________________________________________________________________________
TG4Polyhedra::TG4Polyhedra(const G4Polyhedra& rhs) 
  : G4Polyhedra(rhs) {
//
}

//_____________________________________________________________________________
TG4Polyhedra::~TG4Polyhedra() {
//
}

// private methods

//_____________________________________________________________________________
void TG4Polyhedra::CheckOrigin() 
{
// Checks if polycone was created in a "historical way"
// and give exception otherwise.
// ---

  if (!original_parameters) {
    G4cerr << "    TG4Polyhedra::CheckOrigin: " << G4endl;
    G4cerr << "    Polyhedra has not defined original parameters.";
    G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
    exit(1);
  }  
}


// public methods

//_____________________________________________________________________________
G4int TG4Polyhedra::GetNofZPlanes()
{
// Returns nof z planes.
// ----
  
  CheckOrigin();

  return original_parameters->Num_z_planes;
}  


//_____________________________________________________________________________
G4double* TG4Polyhedra::GetRmin()
{
// Returns array of rmin parameters of the planes.
// ----
  
  CheckOrigin();

  return original_parameters->Rmin;
}  

//_____________________________________________________________________________
G4double* TG4Polyhedra::GetRmax()
{
// Returns array of rmax parameters of the planes.
// ----

  CheckOrigin();
  
  return original_parameters->Rmax;
}  

//_____________________________________________________________________________
G4double* TG4Polyhedra::GetZ()
{
// Returns array of z parameters of the planes.
// ----

  CheckOrigin();
  
  return original_parameters->Z_values;
}  
