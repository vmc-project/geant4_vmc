//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E06
/// \file E06/g4Config.C
/// \brief Configuration macro for Geant4 VirtualMC for Example06
///
/// For geometry defined with Root and selected Geant4 native navigation

void Config()
{
/// The configuration function for Geant4 VMC for Example06
/// called during MC application initialization.
/// For geometry defined with Root and selected Geant4 native navigation

  // Default Geant4 VMC run configuration
  TG4RunConfiguration* runConfiguration
    = new TG4RunConfiguration("geomRootToGeant4", "FTFP_BERT_EMZ+optical");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
