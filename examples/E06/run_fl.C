// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E06
/// \file E06/run_fl.C
/// \brief Macro for running Example06 with Fluka 

void run_fl(const TString& configMacro = "flConfig.C")
{
/// Macro function for running Example06 with Fluka from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E06/flConfig.C 

  // Prepare Fluka working directory
  gSystem->Exec("../macro/run_fluka.sh");

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Fluka libraries
  gROOT->LoadMacro("../macro/fllibs.C");
  fllibs();

  // Load this example library
  gSystem->Load("libexample06");
 
  // Enter in Fluka working directory
  gSystem->cd("fluka_out");

  // MC application
  Ex06MCApplication* appl 
    = new Ex06MCApplication("Example06", "The example06 MC application");
    
  appl->GetPrimaryGenerator()->SetNofPrimaries(100);  
  // appl->SetVerboseLevel(3);  

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Run MC
  appl->RunMC(10);

  delete appl;
}  
