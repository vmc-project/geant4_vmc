// $Id: TG4VolumesFrames.h,v 1.4 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup interfaces
/// \class TG4VolumesFrames
/// \brief Frames for the display of volumes properties
///
/// \author: D. Adamova, NPI Rez

#ifndef TG4_VOLUMESFRAMES_H
#define TG4_VOLUMESFRAMES_H

#include <TObject.h>
#include <TGFrame.h>
#include <TGButton.h>

class TGLabel;
class TGTab;
class TGTextBuffer;
class TGTextEntry;
class TGComboBox;
class G4UserLimits;
class TG4MainFrame; 

class TG4VolumesFrames : public TObject {

public:   

    TG4VolumesFrames( TGTab* Tab, TG4MainFrame* actionFrame);
    virtual ~TG4VolumesFrames();
    
    void SetVolumesComboEntries();
    void DisplayVolumeCharacteristics();
    void DisplayUserLimits();
    void DisplayCuts();
    void DisplayControls();

private:
    TG4VolumesFrames(const TG4VolumesFrames& vf) ;
    TG4VolumesFrames& operator=(const TG4VolumesFrames& vf) ;

    TString GetLimitsDisplay(G4UserLimits* limits) const;
    TString GetCutsDisplay(G4UserLimits* limits) const;
    TString GetControlsDisplay(G4UserLimits* limits) const;
    
    TG4MainFrame*       fPanel;    //main frame
    TGCompositeFrame*   fCapFrame; // the top frame for volumes properties display
    TGCompositeFrame*   fVolSubframe1; // frame for the combo box
    TGCompositeFrame*   fVolSubframe2; //  frame for the text entries        
    TGGroupFrame*       fGrFrame; // frame for user's limits
    TGHorizontalFrame*  fGrHFrame; // frame to hold text buttons
    TGTextButton*       fbtsumm; // button to invoke user's limits display  
    TGTextButton*       fbtcuts; // button to invoke cuts display
    TGTextButton*       fbtcontrols; // button to invoke controls display
    TGLayoutHints*      fVolFrameLayout; // layout hints for SubFrames
    TGHorizontalFrame*  fHframe[3];     // horizontal frames for text entries
    TGLabel*            fLabel[3];      // labels for text entries
    TGTextBuffer*       fVolTextBuff[3]; // text buffs for vols propertie
    TGTextEntry*        fVolTextEntry[3]; // text entries for vols properties
    TGComboBox*         fVolumesCombo; // volumes  combo box
    TGLabel*            fComboLabel;   // label for combo box
    TGTextBuffer*       fDisplBuffLimits; //buffer containing text for user limits display
    TGTextBuffer*       fDisplBuffCuts; //buffer containing text for cuts display
    TGTextBuffer*       fDisplBuffControls; //buffer containing text for controls display
                               
    void AddLogicalVolumeName( const char* name, Int_t index) const;

    ClassDef(TG4VolumesFrames,0)
         // class for the composition of the volumes display frame    
  };
  
#endif
    
