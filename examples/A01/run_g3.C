// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2012 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup A01
/// \file A01/run_g3.C
/// \brief Macro for running Example A01 with Geant3 

void run_g3(const TString& configMacro = "g3tgeoConfig.C") 
{
/// Macro function for running Example A01 with Geant3 from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E03/g3Config.C 

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Geant3 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();
  
  // Load this example library
  gSystem->Load("libexampleA01");

  // MC application
  A01MCApplication* appl 
    =  new A01MCApplication("ExampleA01", "The exampleA01 MC application");
  appl->GetPrimaryGenerator()->SetRandomize(false);
  appl->SetWriteStack(true);
  appl->SetWriteHits(true);

  appl->InitMC(configMacro);

  // visualization setting
  gROOT->LoadMacro("set_vis.C");
  set_vis();

  TStopwatch timer;
  timer.Start();  
  appl->RunMC(5);
  timer.Stop();
  timer.Print();
  
  delete appl;
}  