//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------


/// \file E01/g3Config.C
/// \brief Configuration macro for Geant3 VMC for Example01 

void Config()
{
/// The configuration function for Geant3 VMC for Example01
/// called during MC application initialization. 

  new  TGeant3("C++ Interface to Geant3");

  cout << "Geant3 has been created." << endl;
}


