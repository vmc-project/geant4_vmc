//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file exampleMonopole.cxx
/// \brief The Geant4 VMC example Monopole application executable
///
/// The Geant4 VMC application executable
/// with explicitely instantiated TGeant3 or TGeant4 and linked 
/// with all libraries.
///
/// \date 15/07/2018
/// \author I. Hrivnacova; IPN, Orsay

#include "MCApplication.h"
#include "DetectorConstruction.h"

#ifdef USE_GEANT4
#include "TG4RunConfiguration.h"
#include "TGeant4.h"
#endif

#include "TThread.h"
#include "TInterpreter.h"

/// Application main program
int main(int argc, char** argv)
{
#ifdef USE_GEANT3
  // This example cannot run with Geant3
  std::cerr << "Monopole example: Geant3 is not supported." << std::endl;
  return 1;
#endif
  
  // Initialize Root threading.
  // (Multi-threading is triggered automatically if Geant4 was built 
  //  in MT mode.)
#ifdef G4MULTITHREADED
   TThread::Initialize();
   gInterpreter->SetProcessLineLock(false);
#endif

  // Create MC application
  VMC::Monopole::MCApplication* appl 
    =  new VMC::Monopole::MCApplication("ExampleMonopole",
                                        "The exampleMonopole MC application");
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

#ifdef USE_GEANT4
  // RunConfiguration for Geant4 
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRootToGeant4", "FTFP_BERT+monopole", "stepLimiter");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration,
                  argc, argv);
  std::cout << "Geant4 has been created." << std::endl;

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");  
#endif
  
  // Run from this main
  appl->InitMC("");

#ifdef USE_GEANT4
  // Customise Geant4 setting after initialization:
  // Physics list
  geant4->ProcessGeantMacro("g4config2.in");
#endif

#ifdef USE_GEANT4
  // Setting Geant4 visualization
  geant4->ProcessGeantMacro("g4vis.in");
#endif

  appl->RunMC(5);

  delete appl;
}  
