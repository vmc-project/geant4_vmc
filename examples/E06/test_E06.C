//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_E06.C
/// \brief Example E06 Test macro

void test_E06(const TString& configMacro, Bool_t oldGeometry)
{
/// Macro function for testing example E06
/// \param configMacro  configuration macro loaded in initialization
/// \param oldGeometry  if true - geometry is defined via VMC, otherwise
///                     via TGeo


  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new Ex06MCApplication("Example06", "The example06 MC application");
    needDelete = kTRUE;
  }

  // MC application
  Ex06MCApplication* appl
    = (Ex06MCApplication*)TVirtualMCApplication::Instance();
  appl->GetPrimaryGenerator()->SetNofPrimaries(100);

  // Set geometry defined via VMC
  appl->SetOldGeometry(oldGeometry);


 // Initialize MC
  appl->InitMC(configMacro);

  if ( TString(gMC->GetName()) == "TGeant4" ) {
    // Customise Geant4 setting after initialization:
    // Physics list
    ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");
  }

  // Run MC
  appl->RunMC(10);

  if ( needDelete ) delete appl;
}
