// $Id: geant4vmcLinkDef.h,v 1.2 2006/01/13 16:59:38 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 

#pragma link C++ class  TG4Editor+;
#pragma link C++ class  TG4MainFrame+;
#pragma link C++ class  TG4GeometryGUI+;
#pragma link C++ class  TG4GuiVolume+;
#pragma link C++ class  TG4MaterialsFrames+;
#pragma link C++ class  TG4VolumesFrames+;
#pragma link C++ class  TG4ListTreeFrame+;

#pragma link C++ class  TGeant4+;
#pragma link C++ class  TG4RunConfiguration+;
#pragma link C++ class  TG4PhysicsListOptions+;

#endif
