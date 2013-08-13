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
/// \file E03/g4Config1.C
/// \brief Configuration macro for Geant4 VirtualMC for Example03
///
/// For geometry defined with Geant4 in a user run configuration.

void Config()
{
/// The configuration function for Geant4 VMC for Example03
/// called during MC application initialization. 
/// For geometry defined with Geant4 in a user run configuration.

  // Load library with a user run configuration
  gSystem->Load("libgeant4e03");

  // Run configuration with user geometry construction via Geant4
  Ex03RunConfiguration1* runConfiguration 
    = new Ex03RunConfiguration1("FTFP_BERT");
  
  // Run configuration with user geometry construction via Geant4
  // + special cuts activated
  // Ex03RunConfiguration1* runConfiguration 
  //   = new Ex03RunConfiguration1("emStandard", "specialCuts");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");

  cout << "Geant4 has been created." << endl;
}
