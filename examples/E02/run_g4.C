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
/// \file E02/run_g4.C
/// \brief Macro for running Example02 with Geant4. 

void run_g4(const TString& configMacro = "g4Config.C") 
{
/// Macro function for running Example02 with Geant4 from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E02/g4Config.C 

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();

  // Load this example library
  gSystem->Load("libexample02");

  // MC application
  Ex02MCApplication* appl 
    =  new Ex02MCApplication("Example02", "The example02 MC application");

  appl->InitMC(configMacro);
  
  // TODO: pass this infor from master to workers
  //gMC->SetCollectTracks(kTRUE);
 
  // Setting Geant4 visualization
  ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");
  
  appl->RunMC(50);

  delete appl;
}  
