//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file E06/g3Config.C
/// \brief Configuration macro for Geant3 VirtualMC for Example06

void Config()
{
/// The configuration function for Geant3 VMC for Example06
/// called during MC application initialization.

  new  TGeant3("C++ Interface to Geant3");
  cout << "Geant3 has been created." << endl;

  gMC->SetProcess("CKOV",1);
  gMC->SetCut("CUTELE",10e-06);
  gMC->SetCut("CUTGAM",10e-06);

  ((TGeant3*)gMC)->SetSWIT(4,1000);
      // reduce printing from GTREVE_ROOT (sets one printing per 1000 tracks)
}


