// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/g3tgeoConfig.C
/// \brief Configuration macro for Geant3+TGeo VirtualMC for Example03 

void Config()
{
/// The configuration function for Geant3+TGeo VMC for Example03
/// called during MC application initialization. 

  TGeant3* geant3
    = new TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry"); 
  
  cout << "Geant3 has been created." << endl;

  gMC->SetProcess("DRAY",1);
  gMC->SetProcess("LOSS",1);
  gMC->SetProcess("HADR",0);
}


