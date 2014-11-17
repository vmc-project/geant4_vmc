#ifndef TG4_GEOMETRY_GUI_H
#define TG4_GEOMETRY_GUI_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GeometryGUI.h
/// \brief Definition of the TG4GeometryGUI class 
///
/// \author D. Adamova, NPI Rez

#include <TObject.h>

class TG4MainFrame;
class G4LogicalVolume;
class TGListTreeItem;
class G4LogicalVolumeStore;  

/// \ingroup interfaces
/// \brief Geant4 VMC Geometry Browser
///
/// \author: D. Adamova, NPI Rez

class TG4GeometryGUI : public TObject
{
public:
    TG4GeometryGUI();
    virtual ~TG4GeometryGUI();
    
    void  ReadGeometryTree();
    void  RegisterLogicalVolume(G4LogicalVolume* lv, TGListTreeItem* itemv);
    void  ReadMaterials() const; 

 private:
    /// Not implemented
    TG4GeometryGUI(const TG4GeometryGUI& gg) ;
    /// Not implemented
    TG4GeometryGUI& operator=(const TG4GeometryGUI& gg) ;

    TG4MainFrame* fPanel; ///< the main  panel
  

    ClassDef(TG4GeometryGUI,1) // GUI for Geant4 geometry  
};

#endif

