// $Id: $
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
}
