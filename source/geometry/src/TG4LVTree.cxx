// $Id: TG4LVTree.cxx,v 1.2 2004/11/10 11:39:28 brun Exp $
// Category: geometry
//
// Class TG4LVTree
// ---------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4LVTree.h"
#include "TG4Globals.h"
#include "TG4LVStructure.h"
#include "TG4GeometryServices.h"
#ifdef G4VIS_USE
#include "TG4ColourStore.h"
#endif

#include <G4LogicalVolumeStore.hh>
#include <G4LogicalVolume.hh>
#ifdef G4VIS_USE
#include <G4Colour.hh>
#include <G4VisAttributes.hh>
#endif

TG4LVTree* TG4LVTree::fgInstance = 0;

//_____________________________________________________________________________
TG4LVTree* TG4LVTree::Instance() 
{
/// Static singleton access method.

  if (!fgInstance) new TG4LVTree();
  
  return fgInstance;
}  

//_____________________________________________________________________________
TG4LVTree::TG4LVTree()
  : fMessenger(this) 
{
// Protected singleton constructor.

  fgInstance = this;
}

//_____________________________________________________________________________
TG4LVTree::~TG4LVTree() 
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4LVTree::RegisterLogicalVolume(G4LogicalVolume* lv, const G4String& path,
                                      TG4LVStructure& lvStructure) const
{
/// Register the logical volume lv in the structure.
// ---        

  lvStructure.AddNewVolume(lv, path);
  
  // register daughters
  G4int nofDaughters = lv->GetNoDaughters();
  if (nofDaughters>0) {
    G4String previousName = "";
    for (G4int i=0; i<nofDaughters; i++) {
      G4LogicalVolume* lvd = lv->GetDaughter(i)->GetLogicalVolume();
      G4String currentName = lvd->GetName();
      if (currentName != lv->GetName() && currentName != previousName) { 
        G4String newPath = path + lv->GetName() +"/";
        RegisterLogicalVolume(lvd, newPath, lvStructure);
        previousName = currentName;
      }
    }
  }     
}          

//_____________________________________________________________________________
void TG4LVTree::Warn(const G4String& where, const G4String& lvName) const                               
{
/// Print warning "volume not found".
  
   TG4Globals::Warning(
     "TG4LVTree", where, TString(lvName) + " volume not found.");
}

//_____________________________________________________________________________
void TG4LVTree::Warn(const G4String& where) const
{                               
/// Print warning "volume not specified".
  
   TG4Globals::Warning(
     "TG4LVTree", where, "Volume not specified.");
}

//
// public methods
//

//_____________________________________________________________________________
void TG4LVTree::List(const G4String& lvName) const
{
/// List logical volumes tree (daughters) of the logical volume 
/// with specified lvName.
// ---- 

  G4LogicalVolume* lv 
    = TG4GeometryServices::Instance()->FindLogicalVolume(lvName);

  if (lv) {
    G4String path = "";
    TG4LVStructure lvStructure(path);
    RegisterLogicalVolume(lv, path, lvStructure);
    lvStructure.ListTree();
  }
  else 
    Warn("List", lvName);
}    

//_____________________________________________________________________________
void TG4LVTree::List(G4LogicalVolume* lv) const
{
/// List logical volumes tree of the given lv.
// ---- 

  if (!lv) {
    Warn("List");
    return; 
  }  
  
  List(lv->GetName());
}

//_____________________________________________________________________________
void TG4LVTree::ListLong(const G4String& lvName) const
{
/// List logical volumes tree (daughters) of the logical volume 
/// with specified lvName with numbers of daughters (physical volumes).
// ---- 

  G4LogicalVolume* lv 
    = TG4GeometryServices::Instance()->FindLogicalVolume(lvName);

  if (lv) {
    G4String path = "";
    TG4LVStructure lvStructure(path);
    RegisterLogicalVolume(lv, path, lvStructure);
    lvStructure.ListTreeLong();
  }
  else 
    Warn("ListLong", lvName);
}

//_____________________________________________________________________________
void TG4LVTree::ListLong(G4LogicalVolume* lv) const
{
/// List logical volumes tree with numbers of daughters 
/// (physical volumes) of the given lv.
// ---- 

  if (!lv) {
    Warn("ListLong");
    return; 
  }  
  
  ListLong(lv->GetName());
}

#ifdef G4VIS_USE

//_____________________________________________________________________________
void TG4LVTree::SetLVTreeVisibility(G4LogicalVolume* lv, 
                                    G4bool visibility) const
{ 
/// Set visibility to the logical volumes tree (daughters) of 
/// the logical volume lv.

  if (!lv) {
    Warn("SetLVTreeVisibility");
    return; 
  }  
  
  G4String path = "";
  TG4LVStructure lvStructure(path);
  RegisterLogicalVolume(lv, path, lvStructure);
  lvStructure.SetTreeVisibility(visibility);
}

//_____________________________________________________________________________
void TG4LVTree::SetVolumeVisibility(G4LogicalVolume* lv, 
                                    G4bool visibility) const
{ 
/// Set visibility to the specified logical volume.

  if (!lv) {
    Warn("SetVolumeVisibility");
    return; 
  }  
  
  const G4VisAttributes* kpVisAttributes = lv->GetVisAttributes ();
  G4VisAttributes* newVisAttributes; 
  if (kpVisAttributes) {
    G4Colour oldColour   = kpVisAttributes->GetColour();
    newVisAttributes = new G4VisAttributes(oldColour); 
  }  
  else
    newVisAttributes = new G4VisAttributes();
  delete kpVisAttributes;

  newVisAttributes->SetVisibility(visibility); 

  lv->SetVisAttributes(newVisAttributes);
}

//_____________________________________________________________________________
void TG4LVTree::SetLVTreeColour(G4LogicalVolume* lv, 
                                const G4String& colName) const
{ 
/// Set colour to the logical volumes tree (daughters) of 
/// the logical volume lv.

  if (!lv) {
    Warn("SetLVTreeColour");
    return; 
  }  
  
  G4String path = "";
  TG4LVStructure lvStructure(path);
  RegisterLogicalVolume(lv, path, lvStructure);
  lvStructure.SetTreeVisibility(true);
  lvStructure.SetTreeColour(colName);
}

//_____________________________________________________________________________
void TG4LVTree::SetVolumeColour(G4LogicalVolume* lv,
                                const G4String& colName) const
{
/// Set colour to the specified logical volume.

  if (!lv) {
    Warn("SetVolumeColour");
    return; 
  }  
  
  const G4VisAttributes* kpVisAttributes = lv->GetVisAttributes ();
  delete kpVisAttributes;

  G4VisAttributes* newVisAttributes = new G4VisAttributes(); 

  TG4ColourStore* pColours = TG4ColourStore::Instance();
  const G4Colour kColour = pColours->GetColour(colName);
  newVisAttributes->SetVisibility(true); 
  newVisAttributes->SetColour(kColour);

  lv->SetVisAttributes(newVisAttributes);
}

#endif //G4VIS_USE

