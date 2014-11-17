//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E02
/// \file E02/g3tgeoConfig.C
/// \brief Configuration macro for Geant3+TGeo VirtualMC for Example02 

void Config()
{
/// The configuration function for Geant3+TGeo VMC for Example02
/// called during MC application initialization. 

  TGeant3* geant3
    = new  TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry"); 
  
  cout << "Geant3 has been created." << endl;

  geant3->SetHADR(0);
}


