//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_Monopole.C
/// \brief Example Monopole test macro
///
/// Running Example Monopole and testing VMC geometry getters

void test_Monopole(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example Monopole 
/// \param configMacro   configuration macro loaded in initialization 
/// \param oldGeometry   if true - geometry is defined via VMC, otherwise 
///                      via TGeo

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new VMC::Monopole::MCApplication("ExampleMonopole", "The example Monopole MC application");
    needDelete = kTRUE;
  }  

  // Set geometry defined via VMC
  VMC::Monopole::MCApplication* appl
    = (VMC::Monopole::MCApplication*)TVirtualMCApplication::Instance();

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Run MC
  appl->RunMC(1);
  
  if ( needDelete ) delete appl;
}
