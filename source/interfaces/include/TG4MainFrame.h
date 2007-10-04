// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup interfaces
/// \class TG4MainFrame
/// \brief Main Window for the Geant4 VMC Geometry Browser
///
/// \author: D. Adamova, NPI Rez

#ifndef TG4_MAIN_FRAME_H
#define TG4_MAIN_FRAME_H

#include <TGFrame.h>

class TG4ListTreeFrame;
class TG4VolumesFrames;
class TG4MaterialsFrames;
class TG4ListTreeFrame;
class TObject;
class TGTab;
class TGMenuBar;
class TGPopupMenu;


class TG4MainFrame : public TGMainFrame {

public:   
    
    TG4MainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    ~TG4MainFrame();
                
     TG4VolumesFrames* GetVolumesFrames() const;
     TG4MaterialsFrames* GetMaterialsFrames() const;
     TG4ListTreeFrame* GetListTreeFrame() const;

     void CloseWindow();                                                
     Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
//---------------------------------------------------------------------------

private:
    TG4MainFrame(const TG4MainFrame& mf);                     
    TG4MainFrame& operator=(const TG4MainFrame& mf);

    TGMenuBar*          fMenuBar;          // main menu bar   

    TGPopupMenu*        fPopupMenu;     // popup for window manipulations
    TGPopupMenu*        fPopupMenuTest; // popup for test messages
    TGPopupMenu*        fPopupMenuHelp; // popup for help messages

    TGLayoutHints*      fMenuBarItemLayout;// layout left
    TGLayoutHints*      fMenuBarHelpLayout;// layout right
    TGLayoutHints*      fMenuBarLayout;    // main bar layout 
    
    TGTab*      fTab;           // tab widget
    
    TG4VolumesFrames*   fvolumesFrames;   // service class for adding vols subframes 
    TG4MaterialsFrames* fmaterialsFrames; // service class for adding mats subframes 
    TG4ListTreeFrame*   flistTreeFrame;          // service class for volumes list tree                

    ClassDef(TG4MainFrame,0)  // the main frame for the TG4 Browser  
  };
  
//

#endif
