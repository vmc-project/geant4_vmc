// $Id: g4ConfigOld.C,v 1.1 2006/12/12 16:21:38 brun Exp $

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
// for geometry defined with VMC and selected Geant4 native navigation

void Config()
{
  // Run configuration
  TG4RunConfiguration* runConfiguration 
      = new TG4RunConfiguration("geomVMCtoGeant4", "emStandard");
    
  // Run configuration with special cuts activated
  // Ex03RunConfiguration2* runConfiguration 
  //  = new Ex03RunConfiguration2("geomVMCToGeant4", "emStandard", "specialCuts");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;
  
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
