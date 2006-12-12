// $Id: TG4DetConstruction.cxx,v 1.12 2006/04/12 10:37:23 brun Exp $
// Category: geometry
//
// Class TG4DetConstruction
// ---------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4DetConstruction.h"
#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"

#include <G4VPhysicalVolume.hh>

#include <TVirtualMCApplication.h>

//_____________________________________________________________________________
TG4DetConstruction::TG4DetConstruction()
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4DetConstruction::~TG4DetConstruction() 
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
G4VPhysicalVolume* TG4DetConstruction::Construct()
{ 
/// Construct geometry is delegated to TG4GeometryManager
// --

  // Build G4 geometry
  TG4GeometryManager::Instance()->ConstructGeometry();

  return TG4GeometryServices::Instance()->GetWorld();      
}

