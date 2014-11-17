#ifndef TG4_VOLUMESFRAMES_H
#define TG4_VOLUMESFRAMES_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VolumesFrames.h
/// \brief Definition of the TG4VolumesFrames class 
///
/// \author D. Adamova, NPI Rez

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

/// \ingroup interfaces
/// \class TG4VolumesFrames
/// \brief Frames for the display of volumes properties
///
/// \author: D. Adamova, NPI Rez

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
    /// Not implemented
    TG4VolumesFrames(const TG4VolumesFrames& vf) ;
    /// Not implemented
    TG4VolumesFrames& operator=(const TG4VolumesFrames& vf) ;

    TString GetLimitsDisplay(G4UserLimits* limits) const;
    TString GetCutsDisplay(G4UserLimits* limits) const;
    TString GetControlsDisplay(G4UserLimits* limits) const;
    
    void AddLogicalVolumeName( const char* name, Int_t index) const;

    //
    // data members
    
    /// main frame
    TG4MainFrame*       fPanel;
    
    /// the top frame for volumes properties display    
    TGCompositeFrame*   fCapFrame;
    
    /// frame for the combo box    
    TGCompositeFrame*   fVolSubframe1;
    
    /// frame for the text entries            
    TGCompositeFrame*   fVolSubframe2;
    
    /// frame for user's limits    
    TGGroupFrame*       fGrFrame;
    
    /// frame to hold text buttons    
    TGHorizontalFrame*  fGrHFrame;
    
    /// button to invoke user's limits display      
    TGTextButton*       fbtsumm;
    
    /// button to invoke cuts display
    TGTextButton*       fbtcuts;
    
    /// button to invoke controls display    
    TGTextButton*       fbtcontrols;
    
    /// layout hints for SubFrames    
    TGLayoutHints*      fVolFrameLayout;
    
    /// horizontal frames for text entries
    TGHorizontalFrame*  fHframe[3];
    
    /// labels for text entries
    TGLabel*            fLabel[3];
    
    /// text buffs for vols propertie    
    TGTextBuffer*       fVolTextBuff[3];
    
    /// text entries for vols properties    
    TGTextEntry*        fVolTextEntry[3];
    
    /// volumes  combo box    
    TGComboBox*         fVolumesCombo;
    
    /// label for combo box
    TGLabel*            fComboLabel;
    
    /// buffer containing text for user limits display    
    TGTextBuffer*       fDisplBuffLimits;
    
    /// buffer containing text for cuts display
    TGTextBuffer*       fDisplBuffCuts;
    
    /// buffer containing text for controls display    
    TGTextBuffer*       fDisplBuffControls;
                               
    ClassDef(TG4VolumesFrames,0)
         // class for the composition of the volumes display frame    
  };
  
#endif
    
