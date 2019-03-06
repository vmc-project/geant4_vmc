//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_TR.C
/// \brief Example TR Test macro
///
/// Running Example TR

//#include "set_vis.C"

void test_TR(const TString& configMacro)
{
/// Macro function for testing example TR
/// \param configMacro  configuration macro loaded in initialization
///
/// Run 5000 events and print the Mean energy deposit in absorber.

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new VMC::TR::MCApplication("ExampleTR", "The exampleTR MC application");
    needDelete = kTRUE;
  }

  // MC application
  VMC::TR::MCApplication* appl
    = (VMC::TR::MCApplication*)TVirtualMCApplication::Instance();
  appl->GetPrimaryGenerator()->SetNofPrimaries(1);
  appl->SetPrintModulo(10000);
  appl->InitMC(configMacro);

  // visualization setting
  // set_vis();

  appl->RunMC(5000);

  if ( needDelete ) delete appl;
}
