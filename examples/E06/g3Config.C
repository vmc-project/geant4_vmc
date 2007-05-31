// $Id: g3Config.C,v 1.1 2005/05/17 13:52:01 brun Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Configuration macro for Geant3 VirtualMC for Example03

void Config()
{
  new  TGeant3("C++ Interface to Geant3"); 
  cout << "Geant3 has been created." << endl;

  gMC->SetProcess("CKOV",1);
  gMC->SetCut("CUTELE",10e-06);
  gMC->SetCut("CUTGAM",10e-06);
  
  ((TGeant3*)gMC)->SetSWIT(4,1000);
      // reduce printing from GTREVE_ROOT (sets one printing per 1000 tracks) 
}


