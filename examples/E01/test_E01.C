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
/// \file test_E01.C
/// \brief Example E01 test macro
///
/// Running Example E01 and testing VMC geometry getters;

void test_E01(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example E01 
/// \param configMacro   configuration macro loaded in initialization 
/// \param oldGeometry   if true - geometry is defined via VMC, otherwise 
///                      via TGeo

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
  gSystem->Load("libexample01");

  // MC application
  Ex01MCApplication* appl 
    = new Ex01MCApplication("Example01", "The example01 MC application");
    
  // Set geometry defined via VMC
  appl->SetOldGeometry(oldGeometry);  

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Run MC
  appl->RunMC(1);
  
  // Print info
  //cout << endl 
  //     << "Test VMC geometry getters << endl << endl;
  
  // Test VMC geometry getters
  appl->TestVMCGeometryGetters();
  
  delete appl;
}
