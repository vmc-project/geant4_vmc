//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Monopole
/// \file Monopole/g4tgeoConfig.C
/// \brief Configuration macro for Geant4 VMC for Monopole example
///
/// For geometry defined with Root and selected G4Root navigation

void Config()
{
/// The configuration function for Geant4 VMC for Monopole example
/// called during MC application initialization. 
/// For geometry defined with Root and selected RG4Root navigation

  // RunConfiguration for Geant4
  // TG4RunConfiguration* runConfiguration 
  //   = new TG4RunConfiguration("geomRoot", "FTFP_BERT+monopole", "stepLimiter");
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRoot", "FTFP_BERT+monopole", "");
       // "stepLimiter" is included by default, but G4MonopolePhysics has 
       // its own step limiter applied to G4Monopole only
  
  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
