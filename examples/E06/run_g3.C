// $Id: run_g3.C,v 1.1 2005/05/17 13:52:01 brun Exp $
//
// Macro for running Example06 with Geant3 

void run_g3(const TString& configMacro = "g3Config.C") 
{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Geant3 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();
  
  // Load this example library
  gSystem->Load("libexample06");

  // MC application
  Ex06MCApplication* appl 
    = new Ex06MCApplication("Example06", "The example06 MC application");
    
  appl->GetPrimaryGenerator()->SetNofPrimaries(100);  
  //appl->SetVerboseLevel(3);  

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Run MC
  appl->RunMC(10);

  delete appl;
}  
