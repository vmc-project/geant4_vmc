//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E01
/// \file E01/g3tgeoConfig.C
/// \brief Configuration macro for Geant3+TGeo VMC for Example01

void Config()
{
/// The configuration function for Geant3+TGeo VMC for Example01
/// called during MC application initialization.

  new  TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry");

  cout << "Geant3 has been created." << endl;
}


