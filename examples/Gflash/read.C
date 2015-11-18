//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Gflash/read.C
/// \brief Macro for reading the Gflash simulated data from Root file

#include "../macro/basiclibs.C"

void read() 
{
/// Macro for reading the Gflash simulated data from Root file

  // Load basic libraries
  basiclibs();

  // Load this example library
  gSystem->Load("libexampleGflash");
  
  // MC application
  Gflash::MCApplication* appl 
    =  new Gflash::MCApplication("ExampleGflash", "The exampleGflash MC application", kRead);

  for (Int_t i=0; i<5; i++) {
    cout << "   Event no " << i+1 << ":" << endl;
    appl->ReadEvent(i);
    appl->GetCalorimeterSD()->PrintTotal();
    cout << endl;
  }  
}  
