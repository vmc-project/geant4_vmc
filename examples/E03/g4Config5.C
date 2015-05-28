//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/g4Config5.C
/// \brief Configuration macro for Geant4 VirtualMC for Example03
///
/// For geometry with user defined magnetic fiels equation of motion and/or
/// its integrator (stepper) defined in a user run configuration.

void Config()
{
/// The configuration function for Geant4 VMC for Example03
/// called during MC application initialization. 
/// For geometry with a user defined magnetic field equation of motion and integrator.
  
  // Run configuration with user geometry construction via Geant4
  Ex03RunConfiguration4* runConfiguration 
    = new Ex03RunConfiguration4("geomRootToGeant4", "FTFP_BERT");
  
  // Run configuration with user geometry construction via Geant4
  // + special cuts activated
  // Ex03RunConfiguration4* runConfiguration 
  //   = new Ex03RunConfiguration4("geomRootToGeant4", "FTFP_BERT", "specialCuts");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");

  cout << "Geant4 has been created." << endl;
}
