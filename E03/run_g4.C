// $Id: run_g4.C,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
//
// Macro for running Example03 with Geant4. 

void run_g4(const TString& testMacro = "test1.C")
{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();

  // Load this example library
  gSystem->Load("libexample03");
  
  // test 1
  gROOT->LoadMacro(testMacro);
  test("g4Config.C");
  //test("g4Config2.C");  // special config for test3
}  
