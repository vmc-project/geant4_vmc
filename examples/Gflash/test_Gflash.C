//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_Gflash.C
/// \brief Example Gflash Test macro 1
///
/// Running Example Gflash

//#include "set_vis.C"

void test_Gflash(const TString& configMacro)
{
/// Macro function for testing example Gflash
/// \param configMacro  configuration macro loaded in initialization
/// \param nofEvents  number of events ti be run
/// 
/// Run 10 events and print the calorimeter hits.

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new VMC::Gflash::MCApplication("ExampleGflash", "The exampleGflash MC application");
    needDelete = kTRUE;
  }  
 
  // MC application
  VMC::Gflash::MCApplication* appl
    = (VMC::Gflash::MCApplication*)TVirtualMCApplication::Instance();
  appl->InitMC(configMacro);

  // visualization setting
  // set_vis();

  appl->RunMC(10);

  if ( needDelete ) delete appl;
}  
