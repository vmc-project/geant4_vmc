//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file exampleE03c.cxx
/// \brief The Geant4 VMC example E03c application executable
///
/// \date 30/04/2019
/// \author Benedikt Volkel, CERN

#include "Ex03PrimaryGenerator.h"
#include "Ex03cMCApplication.h"

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

  Bool_t isMulti = false;
  Int_t debug = 0;
#ifdef USE_MULTI
  isMulti = true;
  debug = 1;
#endif

  // Create MC application (thread local)
  Ex03cMCApplication* appl = new Ex03cMCApplication(
    "ExampleE03", "The exampleE03 MC application", isMulti, isMulti);
  appl->GetPrimaryGenerator()->SetNofPrimaries(20);
  appl->SetPrintModulo(1);
  appl->SetDebug(debug);

#ifdef USE_GEANT4
  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration =
    new TG4RunConfiguration("geomRoot", "FTFP_BERT", "", false, false);

  // TGeant4
  TGeant4* geant4 = new TGeant4(
    "TGeant4", "The Geant4 Monte Carlo", runConfiguration, argc, argv);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
#endif

#ifdef USE_GEANT3
  TGeant3TGeo* geant3 =
    new TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry");
  geant3->SetProcess("DRAY", 1);
  geant3->SetProcess("LOSS", 1);
  geant3->SetProcess("HADR", 0);
#endif

  // Run example
#ifdef USE_MULTI
  appl->InitMC();
#else
  appl->InitMC("");
#endif

#ifdef USE_GEANT4
#ifndef USE_MULTI
  // Setting Geant4 visualization
  geant4->ProcessGeantMacro("g4vis.in");
#endif
#endif

  appl->RunMC(5);

  delete appl;
}
