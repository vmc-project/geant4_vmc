// $Id: TG4Editor.h,v 1.4 2006/09/13 06:26:11 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup interfaces
/// \class TG4Editor
/// \brief A supplementary service class for Geant4 VMC Geometry Browser
///
/// \author: D. Adamova, NPI Rez

#ifndef TG4_EDITOR_H
#define TG4_EDITOR_H

#include <TGFrame.h> 

class TGTextEdit;
class TGTextButton;
class TGLayoutHints;

class TG4Editor : public TGTransientFrame {
 
public:
   TG4Editor(const TGWindow* main, UInt_t w, UInt_t h);
   virtual ~TG4Editor();

   void   LoadBuffer(const char* buffer);
   void   SetTitle();
   void   Popup();
   void   CloseWindow();
   Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
   
   TGTextEdit* GetEditor() const;

private:
   TG4Editor(const TG4Editor& ge); 
   TG4Editor& operator=(const TG4Editor& ge) ;

   TGTextEdit*       fEdit;   // text edit widget
   TGTextButton*     fOK;     // OK button
   TGLayoutHints*    fL1;     // layout of TGTextEdit
   TGLayoutHints*    fL2;     // layout of OK button
   
   ClassDef(TG4Editor,0)   // service Editor window for GUI
};

// inline methods

inline TGTextEdit* TG4Editor::GetEditor() const { 
  /// Return text editor widget
  return fEdit; 
}   

#endif
