// $Id: g3tgeoConfig.C,v 1.1 2005/01/05 08:05:30 brun Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Configuration macro for Geant3+TGeo VMC for Example01 

void Config()
{
  new  TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry");

  cout << "Geant3 has been created." << endl;
}


