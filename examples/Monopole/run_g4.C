//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Monopole
/// \file Monopole/run_g4.C
/// \brief Macro for running Monopole example with Geant4. 

void run_g4(const TString& configMacro = "g4tgeoConfig.C") 
{
/// Macro function for running Monopole example with Geant4 from
/// Root interactive session.
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4.C.
/// \param configMacro configuration macro name, default \ref Monopole/g4Config.C 

  // MC application
  VMC::Monopole::MCApplication* appl 
    = new VMC::Monopole::MCApplication("ExampleMonopole", "The example Monopole MC application");

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

  // Run MC
  appl->RunMC(100);

  delete appl;
}

