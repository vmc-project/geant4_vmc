// $Id: run_g3.C,v 1.2 2002/08/29 10:12:26 ivana Exp $
//
// Macro for running Example02 with Geant3 

{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Geant3 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();
  
  // Load this example library
  gSystem->Load("libexample02");

  // MC application
  Ex02MCApplication* appl 
    = new Ex02MCApplication("Example02", "The example02 MC application");

  appl->InitMC("g3Config.C");
  appl->RunMC(5);
}  
