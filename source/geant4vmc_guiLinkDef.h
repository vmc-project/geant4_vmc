//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file geant4vmc_guiLinkDef.h
/// \brief The CINT link definitions for Geant4 VMC GUI classes (optional)
/// interfaced in Root

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

#endif
