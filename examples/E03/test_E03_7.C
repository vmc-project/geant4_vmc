//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_E03_6.C
/// \brief Example E03 Test macro 6
///
/// Running Example03

void test_E03_7(const TString& configMacro = "g4Config6.C", Bool_t oldGeometry = kFALSE)
{
/// Macro function for testing example E03
/// \param configMacro  configuration macro loaded in initialization
///                     (g4Config6.C or g4tgeoConfig6.C)
/// \param oldGeometry  if true - geometry is defined via VMC, otherwise
///                     via TGeo
///
/// Test special biasing operation which activates the INCXX phyiscis in selected
/// media.

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new Ex03MCApplication("Example03", "The example03 MC application");
    needDelete = kTRUE;
  }

  // MC application
  Ex03MCApplication* appl
    = (Ex03MCApplication*)TVirtualMCApplication::Instance();
  appl->GetPrimaryGenerator()->SetPrimaryType(Ex03PrimaryGenerator::kPion);
  appl->GetPrimaryGenerator()->SetNofPrimaries(10);

  // Set geometry defined via VMC
  appl->SetOldGeometry(oldGeometry);

  appl->InitMC(configMacro);

  // Visualization setting
  // Activate this code to check curving of trajectories in field
  //set_vis();

  appl->RunMC(1);

  // Inactivate this code to check curving of trajectories in field
  if ( needDelete ) delete appl;
}
