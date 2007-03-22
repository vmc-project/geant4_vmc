// $Id: run_g3.C,v 1.4 2005/01/05 08:05:30 brun Exp $
//
// Macro for running Example01 with Geant3 

void run_g3(const TString& configMacro = "g3tgeoConfig.C") 
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
  
  // Run MC
  appl->RunMC(1);
  
  delete appl;
}  
