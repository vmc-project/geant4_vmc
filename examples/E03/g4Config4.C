//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/g4Config4.C
/// \brief Configuration macro for Geant4 VirtualMC for Example03
///
/// Demonstrates activation of VMC cuts and process controls

void Config()
{
/// The configuration function for Geant4 VMC for Example03
/// called during MC application initialization.
/// For geometry defined with Root and selected Geant4 native navigation

  // Run configuration with special cuts and special controls activated
  TG4RunConfiguration* runConfiguration
     = new TG4RunConfiguration("geomRootToGeant4", "FTFP_BERT",
                               "specialCuts+specialControls");

  // Activate usage of old regions manager
  // that sets production thresholds by ranges
  // runConfiguration->SetSpecialCutsOld(true);

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
