//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file E03/read.C
/// \brief Macro for reading the E03 simulated data from Root file

#include "../macro/basiclibs.C"

void read()
{
/// Macro for reading the E03 simulated data from Root file

  // Load basic libraries
  basiclibs();

  // Load this example library
  gSystem->Load("libexample03");

  // MC application
  Ex03MCApplication* appl
    =  new Ex03MCApplication("Example03", "The example03 MC application", kRead);

  for (Int_t i=0; i<5; i++) {
    cout << "   Event no " << i+1 << ":" << endl;
    appl->ReadEvent(i);
    appl->GetCalorimeterSD()->PrintTotal();
    cout << endl;
  }
}
