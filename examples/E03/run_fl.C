// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/run_fl.C
/// \brief Macro for running Example03 with Fluka 

void run_fl(const TString& configMacro = "flConfig.C")
{
/// Macro function for running Example03 with Fluka from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E03/flConfig.C 

  // Prepare Fluka working directory
  gSystem->Exec("../macro/run_fluka.sh");

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Fluka libraries
  gROOT->LoadMacro("../macro/fllibs.C");
  fllibs();

  // Load this example library
  gSystem->Load("libexample03");
  
  // Enter in Fluka working directory
  gSystem->cd("fluka_out");
 
  // MC application
  Ex03MCApplication* appl 
    =  new Ex03MCApplication("Example03", "The example03 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(20);
  appl->SetPrintModulo(1);

  appl->InitMC(configMacro);

  appl->RunMC(5);
  
  delete appl;
}  
