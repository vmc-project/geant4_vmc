// $Id: TG4LVTree.h,v 1.3 2005/09/01 10:04:32 brun Exp $
/// \ingroup geometry
//
/// \class TG4LVTree
/// \brief A simple logical volumes tree browser
///
/// Class provides methods for browsing volumes trees, 
/// and setting their visualization attributes.
///
/// Author: I. Hrivnacova

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
    TG4LVTree(); 
    TG4LVTree(const TG4LVTree& right);
    TG4LVTree& operator=(const TG4LVTree &right);

    // methods
    void RegisterLogicalVolume(G4LogicalVolume* lv, const G4String& path, 
                               TG4LVStructure& lvStructure) const;
    void Warn(const G4String& where, const G4String& lvName) const;                               
    void Warn(const G4String& where) const;                               

    // static data members
    static TG4LVTree* fgInstance;   // this instance

    // data members
    TG4LVTreeMessenger  fMessenger; // messenger     
};

// inline methods

#endif //TG4_LV_TREE_H

