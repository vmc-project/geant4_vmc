//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_ExGarfield.C
/// \brief Example ExGarfield Test macro 1
///
/// Running Example ExGarfield

//#include "set_vis.C"

void test_ExGarfield(const TString& configMacro)
{
/// Macro function for testing example ExGarfield
/// \param configMacro  configuration macro loaded in initialization
///
/// Run 10 events and print the calorimeter hits.

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new VMC::ExGarfield::MCApplication("ExampleExGarfield", "The example ExGarfield MC application");
    needDelete = kTRUE;
  }

  // MC application
  VMC::ExGarfield::MCApplication* appl
    = (VMC::ExGarfield::MCApplication*)TVirtualMCApplication::Instance();
  appl->InitMC(configMacro);

  // visualization setting
  // set_vis();

  appl->RunMC(5);

  if ( needDelete ) delete appl;
}
