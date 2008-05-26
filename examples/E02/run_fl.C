// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E02
/// \file E02/run_fl.C
/// \brief Macro for running Example02 with Fluka 

void run_fl(const TString& configMacro = "flConfig.C") 
{
/// Macro function for running Example02 with Fluka from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E02/flConfig.C 

  // Prepare Fluka working directory
  gSystem->Exec("../macro/run_fluka.sh");

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Fluka libraries
  gROOT->LoadMacro("../macro/fllibs.C");
  fllibs();

  // Load this example library
  gSystem->Load("libexample02");
  
  // Enter in Fluka working directory
  gSystem->cd("fluka_out");

  cout << "Go to create Ex02MCApplication" << endl;

  // MC application
  Ex02MCApplication* appl 
    = new Ex02MCApplication("Example02", "The example02 MC application");

  cout << "Go to create Ex02MCApplication - done" << endl;

  appl->InitMC(configMacro);
  
  appl->RunMC(5);

  delete appl;
}  
