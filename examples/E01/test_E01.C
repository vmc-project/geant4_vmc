//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_E01.C
/// \brief Example E01 test macro
///
/// Running Example E01 and testing VMC geometry getters

void test_E01(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example E01 
/// \param configMacro   configuration macro loaded in initialization 
/// \param oldGeometry   if true - geometry is defined via VMC, otherwise 
///                      via TGeo

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new Ex01MCApplication("Example01", "The example01 MC application");
    needDelete = kTRUE;
  }  

  // Set geometry defined via VMC
  Ex01MCApplication* appl
    = (Ex01MCApplication*)TVirtualMCApplication::Instance();
  appl->SetOldGeometry(oldGeometry);  

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Run MC
  appl->RunMC(1);
  
  // Test VMC geometry getters
  appl->TestVMCGeometryGetters();
  
  if ( needDelete ) delete appl;
}
