#ifndef TG4_EDITOR_H
#define TG4_EDITOR_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4Editor.h
/// \brief Definition of the TG4Editor class 
///
/// \author D. Adamova, NPI Rez

#include <TGFrame.h> 

class TGTextEdit;
class TGTextButton;
class TGLayoutHints;

/// \ingroup interfaces
/// \class TG4Editor
/// \brief A supplementary service class for Geant4 VMC Geometry Browser
///
/// \author: D. Adamova, NPI Rez

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
   /// Not implemented
   TG4Editor(const TG4Editor& ge); 
   /// Not implemented
   TG4Editor& operator=(const TG4Editor& ge) ;

   TGTextEdit*       fEdit;   ///< text edit widget
   TGTextButton*     fOK;     ///< OK button
   TGLayoutHints*    fL1;     ///< layout of TGTextEdit
   TGLayoutHints*    fL2;     ///< layout of OK button
   
   ClassDef(TG4Editor,0) // service Editor window for GUI
};

// inline methods

inline TGTextEdit* TG4Editor::GetEditor() const { 
  /// Return text editor widget
  return fEdit; 
}   

#endif
