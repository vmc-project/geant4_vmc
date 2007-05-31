// $Id: TG4GuiVolume.h,v 1.4 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup interfaces
/// \class TG4GuiVolume
/// \brief Creating link for logical volume tree in geometry
///
/// \author: D. Adamova, NPI Rez

#ifndef TG4_GUI_VOLUME_H
#define TG4_GUI_VOLUME_H
 
#include <TObject.h>
#include <TGListTree.h>
 
class G4LogicalVolume;


class TG4GuiVolume : public TObject 
{
public:
    TG4GuiVolume(const char* name, G4LogicalVolume* lvolume);
    virtual ~TG4GuiVolume(){;}

    G4LogicalVolume* GetLogicalVolume() const;
    TGListTreeItem* GetItem() const;
    const char* GetName() const;

    void  SetItem(TGListTreeItem* item);
    
private:      
    TG4GuiVolume(const TG4GuiVolume& gv) ;
    TG4GuiVolume & operator=(const TG4GuiVolume& gv) ;

    G4LogicalVolume*  fLogicalVolume;    // geant logical volume 
    TGListTreeItem*   fItem; // current item

    ClassDef(TG4GuiVolume,0)   
};

// inline methods

inline void TG4GuiVolume::SetItem(TGListTreeItem* item) {
  /// Set current item
  fItem = item;
}
 
#endif
