// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007-2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file A01/test_A01.C
/// \brief Example A01 Test macro
///
/// Running A01 example

void test_A01(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example A01 
/// \param configMacro  configuration macro loaded in initialization 
/// \param oldGeometry  if true - geometry is defined via VMC, otherwise 
///                     via TGeo

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new A01MCApplication("ExampleA01", "The exampleA01 MC application");
    needDelete = kTRUE;
  }  

  // MC application
  A01MCApplication* appl 
    = (A01MCApplication*)TVirtualMCApplication::Instance();
  appl->GetPrimaryGenerator()->SetNofPrimaries(100);
  appl->SetWriteStack(true);
  appl->SetWriteHits(true);

  appl->InitMC(configMacro);

  TStopwatch timer;
  timer.Start();  
  appl->RunMC(5);
  timer.Stop();
  timer.Print();
  
  if ( needDelete ) delete appl;
}  
