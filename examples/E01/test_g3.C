// $Id: test_g3.C,v 1.4 2005/01/05 08:05:30 brun Exp $
//
// Macro for testing VMC getters with Geant4

void test_g3(const TString& configMacro = "g3Config.C") 
{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Geant3 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();
  
  // Load this example library
  gSystem->Load("libexample01");

  // MC application
  Ex01MCApplication* appl 
    = new Ex01MCApplication("Example01", "The example01 MC application");

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Test VMC geometry getters
  appl->TestVMCGeometryGetters();

  delete appl;
}  
