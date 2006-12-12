// $Id: TG4PostDetConstruction.cxx,v 1.12 2006/04/12 10:37:23 brun Exp $
// Category: geometry
//
// Class TG4PostDetConstruction
// ---------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4PostDetConstruction.h"
#include "TG4GeometryManager.h"
#include "TG4Globals.h"

#include <TVirtualMCApplication.h>

//_____________________________________________________________________________
TG4PostDetConstruction::TG4PostDetConstruction()
  : TVirtualUserPostDetConstruction()
{
/// Standard constructor
//
}

//_____________________________________________________________________________
TG4PostDetConstruction::~TG4PostDetConstruction() 
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4PostDetConstruction::Initialize(TG4RootDetectorConstruction* /*dc*/)
{ 
/// Construct geometry using the VMC application.
// --

  G4cout << "TG4PostDetConstruction::Initialize" << G4endl;
  TG4GeometryManager::Instance()->ConstructGeometry();  
}

