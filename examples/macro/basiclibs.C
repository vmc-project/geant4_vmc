// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file basiclibs.C
/// \brief Macro for loading basic libraries used with all MCs

void basiclibs()
{
/// Macro function for loading basic libraries used with all MCs

  gSystem->Load("libRIO");
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libEG");
  gSystem->Load("libEGPythia6");
  gSystem->Load("libPythia6");  
  gSystem->Load("libPhysics");
  gSystem->Load("libSQL");  
  gSystem->Load("libNet");  
  gSystem->Load("libTree");  
}  
