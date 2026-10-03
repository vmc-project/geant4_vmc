//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2026 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_E03_9.C
/// \brief Example E03 test macro 9
///
/// Test hit accounting in reflected sensitive volumes (E03a and E03b).
/// Use geomRootToGeant4 (g4Config.C) or, with oldGeometry=true,
/// geomVMC+RootToGeant4 (g4ConfigMixed.C).

void test_E03_9(const TString& configMacro = "g4Config.C", Bool_t oldGeometry = false)
{
  /// Macro function for testing reflected hits in examples E03a and E03b.
  /// \param configMacro  configuration macro loaded in initialization
  /// \param oldGeometry If true, build with VMC calls and add reflections
  ///                    with ROOT; requires geomVMC+RootToGeant4.

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
  appl->GetDetectorConstruction()->SetUseReflection(kTRUE);
  // A centred muon crosses both half-slabs without relying on a shower.
  appl->GetPrimaryGenerator()->SetPrimaryType(Ex03PrimaryGenerator::kTestField);
  appl->GetPrimaryGenerator()->SetIsRandom(kFALSE);
  appl->GetDetectorConstruction()->SetRequireReflectedHits(kTRUE);

  appl->InitMC(configMacro);
  appl->RunMC(3);

  if (needDelete) delete appl;
}
