// $Id: run_g3.C,v 1.2 2005/01/05 08:05:30 brun Exp $
//
// Macro for running Example03 with Geant3 

void run_g3(const TString& configMacro = "g3Config.C") 
{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Geant3 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();
  
  // Load this example library
  gSystem->Load("libexample03");

  // MC application
  Ex03MCApplication* appl 
    =  new Ex03MCApplication("Example03", "The example03 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(20);
  appl->SetPrintModulo(1);

  appl->InitMC(configMacro);

  // visualization setting
  gROOT->LoadMacro("set_vis.C");
  set_vis();

  appl->RunMC(5);
  
  delete appl;
}  
