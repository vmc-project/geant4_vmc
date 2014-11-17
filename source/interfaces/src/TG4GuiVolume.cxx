//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GuiVolume.cxx
/// \brief Implementation of the TG4GuiVolume class 
///
/// \author D. Adamova, NPI Rez
 
#include "TG4GuiVolume.h"
#include "TG4Globals.h"
#include <G4LogicalVolume.hh> 

/// \cond CLASSIMP
ClassImp(TG4GuiVolume)
/// \endcond

TG4GuiVolume::TG4GuiVolume(const char* name, G4LogicalVolume* lvolume)
  : TObject(),
    fLogicalVolume(0),
    fItem(0)
{
/// Standard constructor

    fItem   = 0;
    fLogicalVolume = lvolume; 
    
    G4String lName = fLogicalVolume->GetName();
    
    if ( lName != name ) { 
      TG4Globals::Exception(
       "TG4GuiVolume", "TG4GuiVolume",
       "A wrong name assigned to the guiVolume in the ctor");
    }   
}

TG4GuiVolume::~TG4GuiVolume()
{
/// Destructor 
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

