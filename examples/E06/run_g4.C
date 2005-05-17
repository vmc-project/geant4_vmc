// $Id: $
//
// Macro for running Example06 with Geant4. 

void run_g4()
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
  appl->InitMC("g4Config.C");
  
  // Customise Geant4 setting after initialization:
  // Physics list
  ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");
  // Visualization settings
  //((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");

  // Run MC
  appl->RunMC(10);

  delete appl;
}  
