// $Id: TG4LVTree.h,v 1.1 2002/06/20 11:55:24 hristov Exp $
// Category: geometry
//
// Author: I. Hrivnacova
//
// Class TG4LVTree
// ---------------------------
// Class provides methods for browsing volumes trees, 
// and setting their visualization attributes.

#ifndef TG4_LV_TREE_H
#define TG4_LV_TREE_H

#include "TG4LVTreeMessenger.h"

#include <globals.hh>

class TG4LVStructure;

class G4LogicalVolume;
#ifdef G4VIS_USE
class G4Colour;
#endif

class TG4LVTree
{
  public:
    // --> protected
    // TG4LVTree();
    // TG4LVTree(const TG4LVTree& right);
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

  protected:
    TG4LVTree(); 
    TG4LVTree(const TG4LVTree& right);

    // operators
    TG4LVTree& operator=(const TG4LVTree &right);

  private:
    // methods
    void RegisterLogicalVolume(G4LogicalVolume* lv, const G4String& path, 
                               TG4LVStructure& lvStructure) const;
    void Warn(const G4String& where, const G4String& lvName) const;			       
    void Warn(const G4String& where) const;			       

    // static data members
    static TG4LVTree* fgInstance;

    // data members
    TG4LVTreeMessenger  fMessenger; //messenger     
};

// inline methods

#endif //TG4_LV_TREE_H

