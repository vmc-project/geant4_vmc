//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/g4tgeoConfig4.C
/// \brief Configuration macro for Geant4 VirtualMC for Example03
///
/// Demonstrates special biasing operation which activates the INCXX physics in selected
/// media.

void Config()
{
/// The configuration function for Geant4 VMC for Example03
/// called during MC application initialization.
/// For geometry defined with Root and G4Root navigation

  // Run configuration with added biasing physics
  TG4RunConfiguration* runConfiguration
     = new TG4RunConfiguration("geomRoot", "FTFP_BERT+biasing");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");

  // Verbosity
  geant4->ProcessGeantCommand("/tracking/verbose 1");

  // Define media with the INCXX physics
  geant4->ProcessGeantCommand("/mcVerbose/biasingConfigurationManager 3");
  geant4->ProcessGeantCommand("/mcPhysics/biasing/setModel inclxx");
  geant4->ProcessGeantCommand("/mcPhysics/biasing/setRegions Lead");
  geant4->ProcessGeantCommand("/mcPhysics/biasing/setParticles proton neutron pi+ pi-");
}
