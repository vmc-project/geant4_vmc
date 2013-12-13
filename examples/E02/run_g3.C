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
/// \file E02/run_g3.C
/// \brief Macro for running Example02 with Geant3 

void run_g3(const TString& configMacro = "g3tgeoConfig.C") 
{
/// Macro function for running Example02 with Geant3 from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E02/g3Config.C 

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Geant3 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();
  
  // Load this example library
  gSystem->Load("libmtroot");
  gSystem->Load("libexample02");

  // MC application
  Ex02MCApplication* appl 
    = new Ex02MCApplication("Example02", "The example02 MC application");

  // Initialize MC
  appl->InitMC(configMacro);
  
  gMC->SetCollectTracks(kTRUE);
  
  // Run MC
  appl->RunMC(5);

  delete appl;
}  
