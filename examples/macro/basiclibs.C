// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file basiclibs.C
/// \brief Macro for loading basic libraries used with all MCs

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TSystem.h>

#endif

void basiclibs()
{
/// Macro function for loading basic libraries used with all MCs

  gSystem->Load("libRIO");
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libPhysics");
  gSystem->Load("libTree");  
}  
