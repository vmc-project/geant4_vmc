//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E01
/// \file E01/g4Config.C
/// \brief Configuration macro for Geant4 VMC for Example01
///
/// For geometry defined with Root and selected Geant4 native navigation

void Config()
{
/// The configuration function for Geant4 VMC for Example01
/// called during MC application initialization.
/// For geometry defined with Root and selected Geant4 native navigation

  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration
    = new TG4RunConfiguration("geomRootToGeant4");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  geant4->ProcessGeantCommand("/mcVerbose/all 1");

  cout << "Geant4 has been created." << endl;

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  // geant4->ProcessGeantMacro("g4config.in");
}
