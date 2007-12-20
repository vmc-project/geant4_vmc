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
// Configuration macro for Fluka VMC for Example01 


void Config()
{
  new  TFluka("C++ Interface to Fluka", 0/*verbositylevel*/);

  cout << "Fluka has been created." << endl;
}


