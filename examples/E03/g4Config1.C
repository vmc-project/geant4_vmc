// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Configuration macro for Geant4 VirtualMC for Example03
// for geometry defined with Geant4 in a user run configuration


void Config()
{
  // Load G4 novice N02 example library (with detector construction)
  gSystem->Load("$G4INSTALL/tmp/$G4SYSTEM/exampleN03/libexampleN03");

  // Load library with a user run configuration
  gSystem->Load("libgeant4e03");

  // Run configuration with user geometry construction via Geant4
  Ex03RunConfiguration1* runConfiguration 
    = new Ex03RunConfiguration1("emStandard");
  
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
