// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_E06.C
/// \brief Example E06 Test macro

void test_E06(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example E06 
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
  gSystem->Load("libexample06");

  // MC application
  Ex06MCApplication* appl 
    = new Ex06MCApplication("Example06", "The example06 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(100);  
    
  // Set geometry defined via VMC
  appl->SetOldGeometry(oldGeometry);  


 // Initialize MC
  appl->InitMC(configMacro);
  
  if ( mc == "g4" ) {  
    // Customise Geant4 setting after initialization:
    // Physics list
    ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");
  }  

  // Run MC
  appl->RunMC(10);
    
  delete appl;
}  
