// $Id: run_g4.C,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
//
// Macro for running Example02 with Geant4. 

{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();

  // Load this example library
  gSystem->Load("libexample02");
  
  // MC application
  Ex02MCApplication* appl 
    =  new Ex02MCApplication("Example02", "The example02 MC application");

  appl->InitMC("g4Config.C");
 
  // Setting Geant4 visualization
 ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");
  
  appl->RunMC(5);

  delete appl;
}  
