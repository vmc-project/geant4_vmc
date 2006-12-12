// $Id: run_g4.C,v 1.2 2003/06/03 17:05:49 brun Exp $
//
// Macro for running Example02 with Geant4. 

void run_g4(const TString& configMacro = "g4Config.C") 
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

  appl->InitMC(configMacro);
 
  // Setting Geant4 visualization
  ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");
  
  appl->RunMC(5);

  delete appl;
}  
