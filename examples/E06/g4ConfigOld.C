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
// Configuration macro for Geant4 VirtualMC for Example06
// for geometry defined with VMC and selected Geant4 native navigation

void Config()
{
  // Default Geant4 VMC run configuration
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomVMCtoGeant4", "emStandard+optical");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);
  
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
