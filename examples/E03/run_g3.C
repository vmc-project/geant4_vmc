// $Id: run_g3.C,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
//
// Macro for running Example03 with Geant3 

void run_g3(const TString& testMacro = "test1.C") 
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
  test("g3Config.C");
}  
