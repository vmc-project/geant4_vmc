// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file g3libs.C
/// \brief Macro for loading Geant3 libraries

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>

#include <TSystem.h>
#include <TString.h>

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

void g3libs()
{
/// Macro function for loading Geant3 libraries

  cout << "Loading Geant3 libraries ..." << endl;

  gSystem->Load("libEG");
  gSystem->Load("libEGPythia6");
  gSystem->Load("libPythia6");  

  if (isLibrary("libdummies.so"))
     gSystem->Load("libdummies.so");
                   // libdummies.so needed from geant3_+vmc version 0.5

  gSystem->Load("libgeant321.so");

  cout << "Loading Geant3 libraries ... finished" << endl;
}

