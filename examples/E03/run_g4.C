// $Id: run_g4.C,v 1.3 2005/10/13 09:10:25 brun Exp $
//
// Macro for running Example03 with Geant4. 

void run_g4(const TString& configMacro = "g4Config.C")
{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();

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
  
  //delete appl;
}  
