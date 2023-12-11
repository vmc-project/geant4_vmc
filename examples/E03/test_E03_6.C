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

// Activate this code to check curving of trajectories in field
//#include "set_vis.C"

void test_E03_6(const TString& configMacro = "g4Config5.C", Bool_t oldGeometry = kFALSE)
{
/// Macro function for testing example E03
/// \param configMacro  configuration macro loaded in initialization
///                     (g4Config5.C or g4tgeoConfig5.C)
/// \param oldGeometry  if true - geometry is defined via VMC, otherwise
///                     via TGeo
///
/// Activate user defined magnetic field equation of motion and stepper
/// and run an event of 5 muons.

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new Ex03MCApplication("Example03", "The example03 MC application");
    needDelete = kTRUE;
  }

  // MC application
  Ex03MCApplication* appl
    = (Ex03MCApplication*)TVirtualMCApplication::Instance();
  appl->GetPrimaryGenerator()->SetNofPrimaries(5);
  appl->GetPrimaryGenerator()->SetPrimaryType(Ex03PrimaryGenerator::kTestField);
  appl->SetField(20);  // Set field value in z-direction to 20 kGauss (2tesla)

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
