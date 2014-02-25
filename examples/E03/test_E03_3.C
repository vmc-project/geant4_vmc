// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_E03_3.C
/// \brief Example E03 Test macro 3
///
/// Running Example03

void test_E03_3(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example E03 
/// \param configMacro  configuration macro loaded in initialization 
/// \param oldGeometry  if true - geometry is defined via VMC, otherwise 
///                     via TGeo
/// 
/// Run user defined primary particles with verbosity level switched on 

  // Load application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    gROOT->LoadMacro("./test_E03_load.C");
    test_E03_load(configMacro, oldGeometry);
    needDelete = kTRUE;
  }  
 
  // MC application
  Ex03MCApplication* appl
    = (Ex03MCApplication*)TVirtualMCApplication::Instance();
  appl->GetPrimaryGenerator()->SetNofPrimaries(2);
  appl->GetPrimaryGenerator()->SetPrimaryType(Ex03PrimaryGenerator::kUser);
  appl->SetPrintModulo(1);
  appl->SetVerboseLevel(3);

  // Set geometry defined via VMC
  appl->SetOldGeometry(oldGeometry);  

  appl->InitMC(configMacro);

  // visualization setting
  // gROOT->LoadMacro("set_vis.C");
  // set_vis();

  appl->RunMC(1);

  if ( needDelete ) delete appl;
}  
