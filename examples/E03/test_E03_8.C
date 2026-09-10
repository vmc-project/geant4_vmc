//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2026 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_E03_8.C
/// \brief Example E03 test macro 8
///
/// Test CurrentVolOffID/Name with nested TGeo assemblies.

void test_E03_8(const TString& configMacro = "g4tgeoConfig.C", Bool_t oldGeometry = false)
{
  /// Macro function for testing assembly levels in example E03a.
  /// \param configMacro  configuration macro loaded in initialization
  /// \param oldGeometry  must be false; assemblies require TGeo geometry

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if (!TVirtualMCApplication::Instance()) {
    new Ex03MCApplication("Example03", "The example03 MC application");
    needDelete = kTRUE;
  }

  Ex03MCApplication* appl =
    (Ex03MCApplication*)TVirtualMCApplication::Instance();
  appl->GetPrimaryGenerator()->SetNofPrimaries(1);
  appl->SetPrintModulo(1);
  appl->SetOldGeometry(oldGeometry);
  appl->GetDetectorConstruction()->SetUseAssemblies(kTRUE);

  appl->InitMC(configMacro);
  appl->RunMC(1);

  if (needDelete) delete appl;
}
