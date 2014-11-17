//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01/read.C
/// \brief Macro for reading the A01 simulated data from Root file

void read() 
{
/// Macro for reading the E03 simulated data from Root file

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load this example library
  gSystem->Load("libexampleA01");
  
  // MC application
  A01MCApplication* appl 
    =  new A01MCApplication("Example A01", "The Example A01 MC application", kRead);

  for (Int_t i=0; i<5; i++) {
    cout << "   Event no " << i+1 << ":" << endl;
    appl->ReadEvent(i);
    appl->GetHodoscopeSD1()->Print();
    appl->GetHodoscopeSD2()->Print();
    appl->GetDriftChamberSD1()->Print();
    appl->GetDriftChamberSD2()->Print();
    appl->GetEmCalorimeterSD()->PrintTotal();
    appl->GetHadCalorimeterSD()->PrintTotal();
    cout << endl;
  }  
}  
