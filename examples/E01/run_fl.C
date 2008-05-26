// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E01
/// \file E01/run_fl.C
/// \brief Macro for running Example01 with Fluka 

void run_fl(const TString& configMacro = "flConfig.C") 
{
/// Macro function for running Example01 with Fluka from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E01/flConfig.C 

  // Prepare Fluka working directory
  gSystem->Exec("../macro/run_fluka.sh");

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Fluka libraries
  gROOT->LoadMacro("../macro/fllibs.C");
  fllibs();
  
  // Load this example library
  gSystem->Load("libexample01");

  // Enter in Fluka working directory
  gSystem->cd("fluka_out");

  // MC application
  Ex01MCApplication* appl
    = new Ex01MCApplication("Example01", "The example01 MC application");

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Run MC
  appl->RunMC(1);
  
  delete appl;
}  
