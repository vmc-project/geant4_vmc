// $Id: run_g3.C,v 1.4 2005/01/05 08:05:30 brun Exp $
//
// Test macro:
// running Example02
// parameters:
// - configMacro: configuration macro loaded in initialization 
// - oldGeometry: if true - geometry is defined via VMC, otherwise via TGeo
// 

void test_E02(const TString& configMacro, Bool_t oldGeometry)
{
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
  
  // Load this example library
  gSystem->Load("libexample02");

  // MC application
  Ex02MCApplication* appl 
    = new Ex02MCApplication("Example02", "The example02 MC application");
    
  // Set geometry defined via VMC
  appl->SetOldGeometry(oldGeometry);  

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Run MC
  appl->RunMC(5);
    
  delete appl;
}  
