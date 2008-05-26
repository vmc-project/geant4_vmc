// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E01
/// \file E01/flConfig.C
/// \brief Configuration macro for Fluka VMC for Example01 

void Config()
{
/// The configuration function for Fluka VMC for Example01
/// called during MC application initialization. 

  new  TFluka("C++ Interface to Fluka", 0/*verbositylevel*/);

  cout << "Fluka has been created." << endl;
}


