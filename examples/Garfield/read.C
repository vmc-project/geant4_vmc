//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Garfield/read.C
/// \brief Macro for reading the Garfield simulated data from Root file

#include "../macro/basiclibs.C"

void read() 
{
/// Macro for reading the Garfield simulated data from Root file

  // Load basic libraries
  basiclibs();

  // Load Garfield library
  gSystem->Load("libGarfield");

  // Load this example library
  gSystem->Load("libvmc_Garfield");
  
  // MC application
  VMC::Garfield::MCApplication* appl 
    =  new VMC::Garfield::MCApplication("ExampleGarfield", "The exampleGarfield MC application", kRead);

  for (Int_t i=0; i<5; i++) {
    cout << "   Event no " << i+1 << ":" << endl;
    appl->ReadEvent(i);
    appl->GetCalorimeterSD()->PrintTotal();
    cout << endl;
  }  
}  
