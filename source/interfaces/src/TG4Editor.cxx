// $Id: TG4Editor.cxx,v 1.4 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: interfaces
//
//========================================================
//
//---------------TG4Editor.cxx---------------------------//
//------- A supplementary service class for--------------//
//-----------AG4 Geometry Browser------------------------//
//
//=========================================================
//
// Author: D. Adamova

#include "TG4Editor.h"
#include "TG4Globals.h"

#include <TGButton.h>
#include <TGTextEdit.h>
#include <TGText.h>


ClassImp(TG4Editor)

TG4Editor::TG4Editor(const TGWindow* main, UInt_t w, UInt_t h) :
    TGTransientFrame(gClient->GetRoot(), main, w, h),
    fEdit(0),
    fOK(0),
    fL1(0),
    fL2(0)
{
   /// Create an editor 

   fEdit = new TGTextEdit(this, w, h, kSunkenFrame | kDoubleBorder);
   fL1 = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 3, 3, 3, 3);
   AddFrame(fEdit, fL1);

   fOK = new TGTextButton(this, "  &OK  ");
   fL2 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);
   AddFrame(fOK, fL2);

   SetTitle();

   MapSubwindows();

   Resize(GetDefaultSize());

   // position relative to the parent's window
   Window_t wdum;
   int ax, ay;
   gVirtualX->TranslateCoordinates(main->GetId(), GetParent()->GetId(),
                          (((TGFrame *) main)->GetWidth() - fWidth) >> 1,
                          ((TGFrame *) main)->GetHeight() - (fHeight >> 1),
                          ax, ay, wdum);
   Move(ax, ay);
   SetWMPosition(ax, ay);
}

TG4Editor::~TG4Editor()
{
   /// Delete editor accessories

   delete fEdit;
   delete fOK;
   delete fL1;
   delete fL2;
}

void TG4Editor::SetTitle()
{   
/// Set title in editor window.

   TGText* txt = GetEditor()->GetText();
   Bool_t untitled = !strlen(txt->GetFileName()) ? kTRUE : kFALSE;

   char title[256];
   if (untitled)
      sprintf(title, "Message Display");
   else
      sprintf(title, "Editor - %s", txt->GetFileName());

   SetWindowName(title);
   SetIconName(title);
}

void TG4Editor::Popup()
{
   /// Show editor.

   MapWindow();
   fClient->WaitFor(this);
}

void TG4Editor::LoadBuffer(const char* buffer) 
{
   /// Load a text buffer in the editor.

   fEdit->LoadBuffer(buffer);
}

void TG4Editor::CloseWindow()
{
   /// Called when closed via window manager action.

   delete this;
}

Bool_t TG4Editor::ProcessMessage(Long_t msg, Long_t, Long_t)
{
   /// Process Help Menu.
   

   switch (GET_MSG(msg)) {
      case kC_COMMAND:
         switch (GET_SUBMSG(msg)) {
            case kCM_BUTTON:
               // Only one button and one action...
               delete this;
               break;
            default:
               break;
         }
         break;
      case kC_TEXTVIEW:
         switch (GET_SUBMSG(msg)) {
            case kTXT_CLOSE:
               // close window
               delete this;
               break;
            case kTXT_OPEN:
               SetTitle();
               break;
            case kTXT_SAVE:
               SetTitle();
               break;
            default:
               break;
         }
      default:
         break;
   }

   return kTRUE;
}

