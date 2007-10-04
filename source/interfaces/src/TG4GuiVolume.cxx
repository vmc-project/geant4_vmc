// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: interfaces
//
//==================================================================
//
//----------------TG4GuiVolume.cxx-------------------------------//
//----Creating link for Logical Volume Tree in AG4 Geometry----//
//
//===================================================================
//
// Author: D. Adamova
 
#include "TG4GuiVolume.h"
#include "TG4Globals.h"
#include <G4LogicalVolume.hh> 

ClassImp(TG4GuiVolume)

TG4GuiVolume::TG4GuiVolume(const char* name, G4LogicalVolume* lvolume)
  : TObject(),
    fLogicalVolume(0),
    fItem(0)
{
/// Constructor
    fItem   = 0;
    fLogicalVolume = lvolume; 
    
    G4String lName = fLogicalVolume->GetName();
    
    if ( lName != name ) { 
      TG4Globals::Exception(
       "TG4GuiVolume", "TG4GuiVolume",
       "A wrong name assigned to the guiVolume in the ctor");
    }   
}

const char* TG4GuiVolume::GetName() const
{
/// Returns the gui/logical volume name
   
  G4String lName = fLogicalVolume->GetName();
  return lName; 
}    

TGListTreeItem* TG4GuiVolume::GetItem() const
{
/// Returns ListTree item

    return fItem;
    
}

G4LogicalVolume* TG4GuiVolume::GetLogicalVolume() const
{
/// Returns logical volume

  return fLogicalVolume;

}

