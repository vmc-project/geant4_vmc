// $Id: run_g3.C,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
//
// Macro for running Example01 with Geant3 

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

  appl->InitMC("g3Config.C");
  
  appl->RunMC(1);
}  
