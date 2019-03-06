//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Gflash/g3Config.C
/// \brief Configuration macro for Geant3 VirtualMC for Gflash example

void Config()
{
/// The configuration function for Geant3 VMC for Gflash example
/// called during MC application initialization.

  TGeant3* geant3
    = new  TGeant3("C++ Interface to Geant3");

  cout << "Geant3 has been created." << endl;
}


