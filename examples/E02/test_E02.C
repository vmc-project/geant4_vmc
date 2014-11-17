//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_E02.C
/// \brief Example E02 Test macro
///
/// Running Example02

void test_E02(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example E02 
/// \param configMacro  configuration macro loaded in initialization 
/// \param oldGeometry  if true - geometry is defined via VMC, otherwise 
///                     via TGeo

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new Ex02MCApplication("Example02", "The example02 MC application");
    needDelete = kTRUE;
  }  

  // MC application
  Ex02MCApplication* appl 
    = (Ex02MCApplication*)TVirtualMCApplication::Instance();

  // Set geometry defined via VMC
  appl->SetOldGeometry(oldGeometry);  

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Run MC
  appl->RunMC(5);
    
  if ( needDelete ) delete appl;
}  
