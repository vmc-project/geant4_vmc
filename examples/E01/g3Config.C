// $Id: g3Config.C,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Configuration macro for Geant3 VMC for Example01 

void Config()
{
  new  TGeant3("C++ Interface to Geant3");

  cout << "Geant3 has been created." << endl;
}


