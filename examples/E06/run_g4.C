// $Id: run_g4.C,v 1.1 2005/05/17 13:52:01 brun Exp $
//
// Macro for running Example06 with Geant4. 

void run_g4(const TString& configMacro = "g4Config.C") 
{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();

  // Load this example libraries
   gSystem->Load("libexample06");

  // MC application
  Ex06MCApplication* appl 
    = new Ex06MCApplication("Example06", "The example06 MC application");
    
  appl->GetPrimaryGenerator()->SetNofPrimaries(100);  
  //appl->SetVerboseLevel(3);  
  
  // Macro to run with primary = polarized optical photon
  //gROOT->LoadMacro("optPhoton.C");
  //optPhoton();  

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Customise Geant4 setting after initialization:
  // Physics list
  ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");
  // Visualization settings
  ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");

  // Run MC
  appl->RunMC(10);

  delete appl;
}  
