//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Monopole
/// \file Monopole/g4Config.C
/// \brief Configuration macro for Geant4 VMC for Monopole example
///
/// For geometry defined with Root and selected Geant4 native navigation

void Config()
{
/// The configuration function for Geant4 VMC for Monopole example
/// called during MC application initialization.
/// For geometry defined with Root and selected Geant4 native navigation

  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration
    = new TG4RunConfiguration("geomRootToGeant4", "FTFP_BERT+monopole", "stepLimiter");

  // Define monopole properties
  // (uncomment the lines below to change the defaults)
  // runConfiguration->SetParameter("monopoleMass", 100.);
  // runConfiguration->SetParameter("monopoleElCharge", 0.);
  // runConfiguration->SetParameter("monopoleMagCharge", 1.);

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
