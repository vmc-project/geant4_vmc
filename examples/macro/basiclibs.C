//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file basiclibs.C
/// \brief Macro for loading basic libraries used with all MCs

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TSystem.h>

#endif

Bool_t isLibrary(const char* libName)
{
/// Helper function which testes the existence of the given library
/// \param libName  The library name

  if (TString(gSystem->DynamicPathName(libName, kTRUE)) != TString(""))
    return kTRUE;
  else
    return kFALSE;
}

void basiclibs()
{
/// Macro function for loading basic libraries used with all MCs

  gSystem->Load("libRIO");
  gSystem->Load("libGeom");
  gSystem->Load("libPhysics");
  gSystem->Load("libTree");

  if ( isLibrary("libVMC") ) {
    // VMC library in ROOT (optional)
    gSystem->Load("libVMC");
  }
}
