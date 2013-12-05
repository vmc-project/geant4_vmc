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
/// \file A01/run_g4.C
/// \brief Macro for running Example A01 with Geant4. 

void run_g4(const TString& configMacro = "g4Config.C")
{
/// Macro function for running Example A01 with Geant4 from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E03/g4Config.C 

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();

  // Load this example library
  gSystem->Load("libmtroot");
  gSystem->Load("libexampleA01");
 
  // Initialize Root threading
  TThread::Initialize();

  // MC application
  A01MCApplication* appl 
    =  new A01MCApplication("ExampleA01", "The exampleA01 MC application");
  appl->GetPrimaryGenerator()->SetRandomize(false);
  appl->SetWriteStack(true);
  appl->SetWriteHits(true);

  appl->InitMC(configMacro);

  // visualization setting
  //gROOT->LoadMacro("set_vis.C");
  //set_vis();

  TStopwatch timer;
  timer.Start();  
  appl->RunMC(500);
  timer.Stop();
  timer.Print();
  
  delete appl;
}  
