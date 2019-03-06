//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup ExGarfield
/// \file ExGarfield/g3tgeoConfig.C
/// \brief Configuration macro for Geant3+TGeo VirtualMC for ExGarfield example

void Config()
{
/// The configuration function for Geant3+TGeo VMC for ExGarfield example
/// called during MC application initialization.

  TGeant3* geant3
    = new TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry");

  cout << "Geant3 has been created." << endl;
}


