// $Id: run_g3.C,v 1.1 2003/03/17 14:56:50 brun Exp $
//
// Macro for running Example03 with Geant3 

void run_g3(const TString& configMacro = "g3Config.C",
            const TString& testMacro = "test1.C") 
{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Geant3 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();
  
  // Load this example library
  gSystem->Load("libexample03");

  // Run test macro
  gROOT->LoadMacro(testMacro);
  test(configMacro);
}  
