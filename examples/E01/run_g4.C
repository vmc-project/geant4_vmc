// $Id: run_g4.C,v 1.1.1.1 2002/06/16 15:57:36 hristov Exp $
//
// Macro for running Example01 with Geant4. 

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

  appl->InitMC("g4Config.C");
  appl->RunMC(1);
}  
