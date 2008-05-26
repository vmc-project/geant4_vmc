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
/// \file E06/g3tgeoConfig.C
/// \brief Configuration macro for Geant3+TGeo VirtualMC for Example06 

void Config()
{
/// The configuration function for Geant3+TGeo VMC for Example06
/// called during MC application initialization. 

  TGeant3TGeo* g3 =
  new TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry"); 
  cout << "Geant3TGeo has been created." << endl;
  
  gMC->SetProcess("CKOV",1);
  gMC->SetCut("CUTELE",10e-06);
  gMC->SetCut("CUTGAM",10e-06);
  
  ((TGeant3*)gMC)->SetSWIT(4,1000);
      // reduce printing from GTREVE_ROOT (sets one printing per 1000 tracks) 
}


