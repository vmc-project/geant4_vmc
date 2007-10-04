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
/// \class TG4ListTreeFrame
/// \brief Frame for the ListTree container
///
/// \author: D. Adamova, NPI Rez

#ifndef TG4_LISTTREE_FRAME_H
#define TG4_LISTTREE_FRAME_H

#include <TGFrame.h>
#include <TObject.h>

class TGListTreeItem;
class TGListTree;
class TGCanvas;
class TGTab;
class TGPicture;

class TG4ListTreeFrame : public TObject {
public:   

    TG4ListTreeFrame( TGTab* tab, TGMainFrame* actionFrame);
    virtual ~TG4ListTreeFrame();

    Bool_t ProcessSubMessage(Long_t msg, Long_t parm1);  
    void DrawSelectedVolume(TGListTreeItem* item);
    TGListTreeItem*
        AddItem(TObject* obj, TGListTreeItem* parent,const char* name,
                const TGPicture* open, const TGPicture* closed);    
    void SendCloseMessage();                

private:
    TG4ListTreeFrame& operator=(const TG4ListTreeFrame& ltf);
    TG4ListTreeFrame(const TG4ListTreeFrame& ltf);                     

    TGCanvas*   fCanvasWindow;  // Canvas window for the list tree
    TGListTree* fVolumesListTree;  // volumes list tree 
                        
    ClassDef(TG4ListTreeFrame,0)  // the frame for the volumes list tree 
  };
  
//

#endif
