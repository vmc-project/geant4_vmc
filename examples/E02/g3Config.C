// $Id: g3Config.C,v 1.4 2005/01/05 08:05:30 brun Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Configuration macro for Geant3 VirtualMC for Example02 

void Config()
{
  TGeant3* geant3
    = new  TGeant3("C++ Interface to Geant3"); 
  
  cout << "Geant3 has been created." << endl;

  gMC->SetProcess("HADR",0);
}


