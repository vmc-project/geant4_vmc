#ifndef TG4_MATERIALSFRAMES_H
#define TG4_MATERIALSFRAMES_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4MaterialsFrames.h
/// \brief Definition of the TG4MaterialsFrames class 
///
/// \author D. Adamova, NPI Rez

#include <TObject.h>
#include <TGFrame.h>

class TGLabel;
class TGTab;
class TGTextBuffer;
class TGTextEntry;
class TGComboBox;

/// \ingroup interfaces
/// \brief Frames for the the display of materials properties
///
/// \author: D. Adamova, NPI Rez

class TG4MaterialsFrames : public TObject {

public:   

    TG4MaterialsFrames(  TGTab* tab, TGMainFrame* ActionFrame);
    virtual ~TG4MaterialsFrames();
    
    void DisplayMaterialCharacteristics( int qmat);
    void SetMaterialsComboEntries() ;

private:
    /// Not implemented
    TG4MaterialsFrames(const TG4MaterialsFrames& mf) ;
    /// Not implemented
    TG4MaterialsFrames& operator=(const TG4MaterialsFrames& mf) ;

    // methods
    void AddMaterialName( const char* name, Int_t index) const;
    
    // data members
    TGCompositeFrame*   fCapFrame;       ///< the top frame for materials display
    TGCompositeFrame*   fMatSubframe1;   ///< frame for the combo box
    TGCompositeFrame*   fMatSubframe2;   ///< frame for the text entries
    TGLayoutHints*      fMatFrameLayout; ///< layout hints for SubFrames

    TGHorizontalFrame*  fHframe[8];      ///< horizontal frames for text entries
    TGLabel*            fLabel[8];       ///< labels for text entries 
    TGTextBuffer*       fMatTextBuff[8]; ///<text buffs for mat properties
    TGTextEntry*        fMatTextEntry[8];///<text entries for mat properties
    TGComboBox*         fMaterialsCombo; ///< materials  combo box
    TGLabel*            fComboLabel;     ///< label for the combo box

    ClassDef(TG4MaterialsFrames,0) 
         // class for the composition of the materials display frame   
  };
  
#endif
    
    
     
    
    
 
