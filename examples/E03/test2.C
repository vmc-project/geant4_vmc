// $Id: test2.C,v 1.1 2003/03/17 14:56:50 brun Exp $
//
// Common macro for Example03: Verbosity

void test(const char* setup)
{
  // MC application
  Ex03MCApplication* appl 
    =  new Ex03MCApplication("Example03", "The example03 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(1);
  appl->SetVerboseLevel(3);

  appl->InitMC(setup);
  appl->RunMC(1);

  delete appl;
}
