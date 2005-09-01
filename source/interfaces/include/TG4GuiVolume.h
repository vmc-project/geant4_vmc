// $Id: TG4GuiVolume.h,v 1.2 2004/11/10 11:39:28 brun Exp $
/// \ingroup interfaces
//
/// \class TG4GuiVolume
/// \brief Creating link for logical volume tree in geometry
///
/// Author: D. Adamova

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

//--------------------------------------------------------------------
protected:
    TG4GuiVolume(const TG4GuiVolume& gv) ;

    // operators
    TG4GuiVolume & operator=(const TG4GuiVolume& gv) ;
    
//---------------------------------------------------------------------    
    
private:      
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
