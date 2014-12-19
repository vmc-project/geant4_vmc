//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_E03_2.C
/// \brief Example E03 Test macro 2
///
/// Running Example03

//#include "set_vis.C"

void test_E03_2(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example E03 
/// \param configMacro  configuration macro loaded in initialization 
/// \param oldGeometry  if true - geometry is defined via VMC, otherwise 
///                     via TGeo
/// 
/// Run 1 primary with verbosity level switched on .

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new Ex03MCApplication("Example03", "The example03 MC application");
    needDelete = kTRUE;
  }  

  // MC application
  Ex03MCApplication* appl
    = (Ex03MCApplication*)TVirtualMCApplication::Instance();
  appl->GetPrimaryGenerator()->SetNofPrimaries(1);
  appl->SetPrintModulo(1);
  appl->SetVerboseLevel(3);

  // Set geometry defined via VMC
  appl->SetOldGeometry(oldGeometry);  

  appl->InitMC(configMacro);

  // visualization setting
  // set_vis();

  appl->RunMC(1);

  if ( needDelete ) delete appl;
}  
