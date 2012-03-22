#ifndef TG4_LV_TREE_H
#define TG4_LV_TREE_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4LVTree.h
/// \brief Definition of the TG4LVTree class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4LVTreeMessenger.h"

#include <globals.hh>

class TG4LVStructure;

class G4LogicalVolume;
#ifdef G4VIS_USE
class G4Colour;
#endif

/// \ingroup geometry
/// \brief A simple logical volumes tree browser
///
/// Class provides methods for browsing volumes trees, 
/// and setting their visualization attributes.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4LVTree
{
  public:
    virtual ~TG4LVTree();

    // static methods
    static TG4LVTree* Instance();

    // methods
    void List(const G4String& lvName) const;
    void List(G4LogicalVolume* lv) const;
    void ListLong(const G4String& lvName) const;
    void ListLong(G4LogicalVolume* lv) const;

#ifdef G4VIS_USE
    void SetLVTreeVisibility(G4LogicalVolume* lv, G4bool visibility) const;
    void SetVolumeVisibility(G4LogicalVolume* lv, G4bool visibility) const;
    void SetLVTreeColour(G4LogicalVolume* lv, const G4String& colName) const;   
    void SetVolumeColour(G4LogicalVolume* lv, const G4String& colName) const;     
#endif

  private:
    /// Not implemented
    TG4LVTree(); 
    /// Not implemented
    TG4LVTree(const TG4LVTree& right);
    /// Not implemented
    TG4LVTree& operator=(const TG4LVTree &right);

    // methods
    void RegisterLogicalVolume(G4LogicalVolume* lv, const G4String& path, 
                               TG4LVStructure& lvStructure) const;
    void Warn(const G4String& where, const G4String& lvName) const;                               
    void Warn(const G4String& where) const;                               

    // static data members
    static __thread TG4LVTree* fgInstance; ///<  this instance

    // data members
    TG4LVTreeMessenger  fMessenger; ///<  messenger     
};

#endif //TG4_LV_TREE_H

