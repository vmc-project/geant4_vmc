//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file E01/g4ConfigOld.C
/// \brief Configuration macro for Geant4 VMC for Example01
///
/// For geometry defined with VMC and selected Geant4 native navigation

void Config()
{
/// The configuration function for Geant4 VMC for Example01
/// called during MC application initialization.
/// For geometry defined with VMC and selected Geant4 native navigation

  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration
    = new TG4RunConfiguration("geomVMCtoGeant4");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;
}
