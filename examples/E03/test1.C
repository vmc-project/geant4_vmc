// $Id: test1.C,v 1.1 2003/03/17 14:56:50 brun Exp $
//
// Common macro for Example03: Calorimetry

void test(const char* setup)
{
  // MC application
  Ex03MCApplication* appl 
    =  new Ex03MCApplication("Example03", "The example03 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(20);
  appl->SetPrintModulo(1);

  appl->InitMC(setup);

  // visualization setting
  gROOT->LoadMacro("set_vis.C");
  set_vis();

  appl->RunMC(5);
  
  delete appl;
}
