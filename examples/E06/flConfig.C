// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E06
/// \file E06/flConfig.C
/// \brief Configuration macro for Fluka VMC for Example06 

void Config()
{
/// The configuration function for Fluka VMC for Example06
/// called during MC application initialization. 

  new  TFluka("C++ Interface to Fluka", 0/*verbositylevel*/);

  cout << "Fluka has been created." << endl;

  gMC->SetProcess("CKOV",1);
  gMC->SetCut("CUTELE",10e-06);
  gMC->SetCut("CUTGAM",10e-06);
}


