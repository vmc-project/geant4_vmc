// $Id: run_g4.C,v 1.3 2005/10/13 09:10:25 brun Exp $
//
// Test macro:
// running Example03
// parameters:
// - configMacro: configuration macro loaded in initialization 
// - oldGeometry: if true - geometry is defined via VMC, otherwise via TGeo
// 
// First run 5 events with 20 primaries and print the calorimeter hits;
// secon run 1 primaray with switch on verbosity level.

void test_E03(const TString& configMacro, Bool_t oldGeometry)
{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load MC libraries
  TString mc = configMacro(0, 2);
  if ( mc == "g3" ) {
    // Geant3 libraries
    gROOT->LoadMacro("../macro/g3libs.C");
    g3libs();
  }
  else if ( mc == "g4" ) {  
    // Geant4 libraries
    gROOT->LoadMacro("../macro/g4libs.C");
    g4libs();
  }  

  // Load this example library
  gSystem->Load("libexample03");
 
  // MC application
  Ex03MCApplication* appl 
    =  new Ex03MCApplication("Example03", "The example03 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(20);
  appl->SetPrintModulo(1);

  // Set geometry defined via VMC
  appl->SetOldGeometry(oldGeometry);  

  appl->InitMC(configMacro);

  // visualization setting
  // gROOT->LoadMacro("set_vis.C");
  // set_vis();

  appl->RunMC(5);

  // Reset number of primaries and verbose level
  appl->GetPrimaryGenerator()->SetNofPrimaries(1);
  appl->SetVerboseLevel(3);

  appl->RunMC(1);

  delete appl;
}  
