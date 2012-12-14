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
/// \file A01/test_A01.C
/// \brief Example A01 Test macro
///
/// Running A01 example

void test_A01(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example A01 
/// \param configMacro  configuration macro loaded in initialization 
/// \param oldGeometry  if true - geometry is defined via VMC, otherwise 
///                     via TGeo

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load MC libraries
  TString mc = configMacro(0, 2);
  if ( mc == "g3" ) {
    // Geant3 libraries
    gROOT->LoadMacro("../macro/g3libs.C");
    g3libs();
  }
  else if ( mc == "g4" ) {  
    // Geant4 libraries
    gROOT->LoadMacro("../macro/g4libs.C");
    g4libs();
  }  
  else if ( mc == "fl" ) {  
    // Fluka libraries
    gROOT->LoadMacro("../macro/fllibs.C");
    fllibs();
    
    // Prepare Fluka working directory
    gSystem->Exec("../macro/run_fluka.sh");

    // Enter in Fluka working directory
    gSystem->cd("fluka_out");
  }  
  
  // Load this example library
  gSystem->Load("libexampleA01");
 
  // MC application
  A01MCApplication* appl 
    =  new A01MCApplication("ExampleA01", "The exampleA01 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(100);
  appl->SetWriteStack(true);
  appl->SetWriteHits(true);

  appl->InitMC(configMacro);

  TStopwatch timer;
  timer.Start();  
  appl->RunMC(5);
  timer.Stop();
  timer.Print();
  
  delete appl;
}  
