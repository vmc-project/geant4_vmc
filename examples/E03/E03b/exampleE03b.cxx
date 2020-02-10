//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file exampleE03.cxx
/// \brief The Geant4 VMC example E03 application executable
///
/// \date 26/02/2014
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex03MCApplication.h"

#ifdef USE_GEANT4
#include "TG4RunConfiguration.h"
#include "TGeant4.h"
#endif

#ifdef USE_GEANT3
#include "TGeant3TGeo.h"
#endif

#include "TInterpreter.h"
#include "TThread.h"

/// Application main program
int main(int argc, char** argv)
{
  // Initialize Root threading.
  // (Multi-threading is triggered automatically if Geant4 was built
  //  in MT mode.)
#ifdef G4MULTITHREADED
  TThread::Initialize();
  gInterpreter->SetProcessLineLock(false);
#endif

  // Create MC application (thread local)
  Ex03MCApplication* appl =
    new Ex03MCApplication("ExampleE03", "The exampleE03 MC application");

#ifdef USE_GEANT4
  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration =
    new TG4RunConfiguration("geomRootToGeant4", "FTFP_BERT");

  // TGeant4
  TGeant4* geant4 = new TGeant4(
    "TGeant4", "The Geant4 Monte Carlo", runConfiguration, argc, argv);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
#endif

#ifdef USE_GEANT3
  new TGeant3TGeo("C++ Interface to Geant3");
  gMC->SetProcess("DRAY", 1);
  gMC->SetProcess("LOSS", 1);
  gMC->SetProcess("HADR", 0);
#endif

  // Run example
  appl->InitMC("");

#ifdef USE_GEANT4
  // Setting Geant4 visualization
  geant4->ProcessGeantMacro("g4vis.in");
#endif

  appl->RunMC(5);

  delete appl;
}
