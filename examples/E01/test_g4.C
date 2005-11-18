// $Id: run_g4.C,v 1.2 2003/06/03 17:05:48 brun Exp $
//
// Macro for testing VMC getters with Geant4. 

{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();
  
  // Load this example library
  gSystem->Load("libexample01");

  // MC application
  Ex01MCApplication* appl 
    = new Ex01MCApplication("Example01", "The example01 MC application");

  // Initialize MC
  appl->InitMC("g4Config.C");
  
  // Test VMC geometry getters
  appl->TestVMCGeometryGetters();
  
  delete appl;
}  
