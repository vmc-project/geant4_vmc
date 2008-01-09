#ifndef TG4_GUI_VOLUME_H
#define TG4_GUI_VOLUME_H
 
// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4GuiVolume.h
/// \brief Definition of the TG4GuiVolume class 
///
/// \author D. Adamova, NPI Rez

#include <TObject.h>
#include <TGListTree.h>
 
class G4LogicalVolume;

/// \ingroup interfaces
/// \brief Creating link for logical volume tree in geometry
///
/// \author: D. Adamova, NPI Rez

class TG4GuiVolume : public TObject 
{
public:
    TG4GuiVolume(const char* name, G4LogicalVolume* lvolume);
    virtual ~TG4GuiVolume();

    G4LogicalVolume* GetLogicalVolume() const;
    TGListTreeItem* GetItem() const;
    const char* GetName() const;

    void  SetItem(TGListTreeItem* item);
    
private:      
    /// Not implemented
    TG4GuiVolume(const TG4GuiVolume& gv) ;
    /// Not implemented
    TG4GuiVolume & operator=(const TG4GuiVolume& gv) ;

    G4LogicalVolume*  fLogicalVolume; ///< geant logical volume 
    TGListTreeItem*   fItem; ///< current item

    ClassDef(TG4GuiVolume,0)   
};

// inline methods

inline void TG4GuiVolume::SetItem(TGListTreeItem* item) {
  /// Set current item
  fItem = item;
}
 
#endif
