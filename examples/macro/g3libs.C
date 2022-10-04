//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file g3libs.C
/// \brief Macro for loading Geant3 libraries

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>

#include <TSystem.h>
#include <TString.h>

#endif

namespace g3libUtilities {
  Bool_t isLibrary(const char* libName)
  {
  /// Helper function which testes the existence of the given library
  /// \param libName  The library name

    if (TString(gSystem->DynamicPathName(libName, kTRUE)) != TString(""))
      return kTRUE;
    else
      return kFALSE;
  }
}

void g3libs()
{
/// Macro function for loading Geant3 libraries

  // Load libraries required by Geant3
  gSystem->Load("libPythia6");
  gSystem->Load("libEG");
  gSystem->Load("libEGPythia6");

  // VMC library (optional)
  if ( g3libUtilities::isLibrary("libVMCLibrary") ) {
    cout << "Loading VMC library ..." << endl;
    gSystem->Load("libVMCLibrary");
  }

  if (g3libUtilities::isLibrary("libdummies"))
     gSystem->Load("libdummies");
                   // libdummies.so needed from geant3_+vmc version 0.5

  // Load  geant3 library
  gSystem->Load("libgeant321");

  cout << "Loading Geant3 libraries ... finished" << endl;
}
