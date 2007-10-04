// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Configuration macro for Geant3+TGeo VirtualMC for Example02 

void Config()
{
  TGeant3* geant3
    = new  TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry"); 
  
  cout << "Geant3 has been created." << endl;

  geant3->SetHADR(0);
}


