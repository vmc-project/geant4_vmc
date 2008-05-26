#ifndef TG4_MAIN_FRAME_H
#define TG4_MAIN_FRAME_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4MainFrame.h
/// \brief Definition of the TG4MainFrame class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TGFrame.h>

class TG4ListTreeFrame;
class TG4VolumesFrames;
class TG4MaterialsFrames;
class TG4ListTreeFrame;
class TObject;
class TGTab;
class TGMenuBar;
class TGPopupMenu;

/// \ingroup interfaces
/// \brief Main Window for the Geant4 VMC Geometry Browser
///
/// \author: D. Adamova, NPI Rez

class TG4MainFrame : public TGMainFrame {

public:   
    
    TG4MainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    ~TG4MainFrame();
                
     TG4VolumesFrames* GetVolumesFrames() const;
     TG4MaterialsFrames* GetMaterialsFrames() const;
     TG4ListTreeFrame* GetListTreeFrame() const;

     void CloseWindow();                                                
     Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

private:
    /// Not implemented
    TG4MainFrame(const TG4MainFrame& mf);                     
    /// Not implemented
    TG4MainFrame& operator=(const TG4MainFrame& mf);

    //
    // data members

    /// main menu bar 
    TGMenuBar*          fMenuBar;

    /// popup for window manipulations
    TGPopupMenu*        fPopupMenu;
    
    /// popup for test messages
    TGPopupMenu*        fPopupMenuTest;
    
    /// popup for help messages
    TGPopupMenu*        fPopupMenuHelp;

    /// layout left
    TGLayoutHints*      fMenuBarItemLayout;

    /// layout right
    TGLayoutHints*      fMenuBarHelpLayout;

    /// main bar layout 
    TGLayoutHints*      fMenuBarLayout;

    /// tab widget    
    TGTab*              fTab;
    
    /// service class for adding vols subframes 
    TG4VolumesFrames*   fvolumesFrames;
    
    /// service class for adding mats subframes 
    TG4MaterialsFrames* fmaterialsFrames;
    
    /// service class for volumes list tree                    
    TG4ListTreeFrame*   flistTreeFrame;

    ClassDef(TG4MainFrame,0) // the main frame for the TG4 Browser  
  };
  
//

#endif
