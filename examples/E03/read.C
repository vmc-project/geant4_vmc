//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/read.C
/// \brief Macro for reading the E03 simulated data from Root file

void read()
{
/// Macro for reading the E03 simulated data from Root file
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4a[b,c].C.

  // MC application
  Int_t threadId = 1;
  Ex03MCApplication* appl
    =  new Ex03MCApplication("Example03", "The example03 MC application");

  for (Int_t i=0; i<5; i++) {
    cout << "   Event no " << i+1 << ":" << endl;
    appl->ReadEvent(i, threadId);
    appl->GetCalorimeterSD()->PrintTotal();
    cout << endl;
  }
}
