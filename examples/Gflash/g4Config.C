//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Gflash
/// \file Gflash/g4Config.C
/// \brief Configuration macro for Geant4 VirtualMC for Gflash example
///
/// For geometry defined with Root and Geant4 native navigation

void Config()
{
/// The configuration function for Geant4 VMC for Gflash example
/// called during MC application initialization.

  // Run configuration
  TG4RunConfiguration* runConfiguration
      = new TG4RunConfiguration("geomRootToGeant4", "FTFP_BERT", "gflash", false, false);

  // Run configuration with special cuts activated
  // Gflash::RunConfiguration* runConfiguration
  //   = new Gflash::RunConfiguration("geomRootToGeant4", "FTFP_BERT", "specialCuts");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;

  // Customise Geant4 setting
  // Fast simulation model configuration
  // + verbose level, global range cuts, etc.
  geant4->ProcessGeantMacro("g4config.in");

  cout << "Processing Config() done." << endl;
}
