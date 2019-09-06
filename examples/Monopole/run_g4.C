//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

#include "TVirtualMC.h"

#include "set_g4_vis.C"

/// \ingroup Monopole
/// \file Monopole/run_g4.C
/// \brief Macro for running Monopole example with Geant4.

void run_g4(const TString& configMacro = "g4Config.C")
{
/// Macro function for running Monopole example with Geant4 from
/// Root interactive session.
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4.C.
/// \param configMacro configuration macro name, default \ref Monopole/g4Config.C

  // MC application
  VMC::Monopole::MCApplication* appl
    = new VMC::Monopole::MCApplication("ExampleMonopole", "The example Monopole MC application");

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

  // Customise Geant4 setting after initialization:
  // Physics list
  ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");

  // ((TGeant4*)gMC)->StartGeantUI();

  // Visualization setting
  set_g4_vis();

  // Run MC
  TStopwatch timer;
  timer.Start();
  appl->RunMC(5);
  timer.Stop();
  timer.Print();

  delete appl;
}

