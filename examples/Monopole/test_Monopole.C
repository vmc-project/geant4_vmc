//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2019 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Tests
/// \file test_Monopole.C
/// \brief Example Monopole Test macro

void test_Monopole(const TString& configMacro)
{
/// Macro function for testing example Monopole 
/// \param configMacro  configuration macro loaded in initialization 

  // Create application if it does not yet exist
  Bool_t needDelete = kFALSE;
  if ( ! TVirtualMCApplication::Instance() ) {
    new VMC::Monopole::MCApplication("Example06", "The example06 MC application");
    needDelete = kTRUE;
  }

  // MC application
  VMC::Monopole::MCApplication* appl
    = (VMC::Monopole::MCApplication*)TVirtualMCApplication::Instance();

  // Set detector parameters
  // /testex/det/setMat G4_Si
  // /testex/det/setSizeX  10 cm
  // /testex/det/setSizeYZ 20 cm
  // /testex/det/setStepSize 0.2 mm
  // /testex/run/binSize 0.2 mm
  VMC::Monopole::DetectorConstruction* detector = appl->GetDetectorConstruction();
  detector->SetAbsorberMaterial("Si");
  detector->SetAbsorberSizeX(10);
  detector->SetAbsorberSizeYZ(20);
  detector->SetMaxStepSize(0.02);
  appl->SetBinSize(0.02);

 // Initialize MC
  appl->InitMC(configMacro);

  if ( TString(gMC->GetName()) == "TGeant4" ) {
    // Customise Geant4 setting after initialization:
    // Physics list
    ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");
  }

  // Run MC
  appl->RunMC(100);

  if ( needDelete ) delete appl;
}
