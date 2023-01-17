//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file exampleE06.cxx
/// \brief The Geant4 VMC example E06 application executable
///
/// \date 26/02/2014
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex06MCApplication.h"
#include "Ex06PrimaryGenerator.h"

#ifdef USE_GEANT4
#include "TG4RunConfiguration.h"
#include "TGeant4.h"
#endif

#ifdef USE_GEANT3
#include "TGeant3TGeo.h"
#endif

#include "TROOT.h"

/// Application main program
int main(int argc, char** argv)
{
  // Initialize Root threading.
  // (Multi-threading is triggered automatically if Geant4 was built
  //  in MT mode.)
#ifdef G4MULTITHREADED
  ROOT::EnableThreadSafety();
#endif

  // Create MC application (thread local)
  Ex06MCApplication* appl =
    new Ex06MCApplication("ExampleE06", "The exampleE06 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(10);
  // appl->SetVerboseLevel(3);

#ifdef USE_GEANT4
  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration =
    new TG4RunConfiguration("geomRootToGeant4", "emStandard+optical");

  // TGeant4
  TGeant4* geant4 = new TGeant4(
    "TGeant4", "The Geant4 Monte Carlo", runConfiguration, argc, argv);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
#endif

#ifdef USE_GEANT3
  TGeant3* geant3 = new TGeant3TGeo("C++ Interface to Geant3");
  gMC->SetProcess("CKOV", 1);
  gMC->SetCut("CUTELE", 10e-06);
  gMC->SetCut("CUTGAM", 10e-06);
  geant3->SetSWIT(4, 1000);
  // reduce printing from GTREVE_ROOT (sets one printing per 1000 tracks)
#endif

  // Run example
  appl->InitMC("");

  // Activate storing tracks
  // gMC->SetCollectTracks(kTRUE);

#ifdef USE_GEANT4
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config2.in");
  // Setting Geant4 visualization
  geant4->ProcessGeantMacro("g4vis.in");
#endif

  // Run MC
  appl->RunMC(5);

  delete appl;
}
