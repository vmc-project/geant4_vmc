// $Id: TG4Polyhedra.cxx,v 1.1 2003/07/22 06:46:58 brun Exp $
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
