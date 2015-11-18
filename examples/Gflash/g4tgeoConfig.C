//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Gflash
/// \file Gflash/g4tgeoConfig.C
/// \brief Configuration macro for Geant4 VirtualMC for Gflash example
///
/// For geometry defined with Root and selected G4Root navigation

void Config()
{
/// The configuration function for Geant4 VMC for Gflash example
/// called during MC application initialization. 
/// For geometry defined with Root and selected G4Root navigation

  // Run configuration
  Gflash::RunConfiguration* runConfiguration 
    = new Gflash::RunConfiguration("geomRoot", "FTFP_BERT");
  
  // Run configuration with special cuts activated
  // Gflash::RunConfiguration* runConfiguration 
  //   = new Gflash::RunConfiguration("geomRoot", "FTFP_BERT", "specialCuts");
  
  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;
  
  // Customise Geant4 setting
  // Fast simulation model configuration
  // + verbose level, global range cuts, etc.
  geant4->ProcessGeantMacro("g4config.in");
}
