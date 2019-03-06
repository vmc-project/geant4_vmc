//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2015 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file exampleExGarfield.cxx
/// \brief The Geant4 VMC example ExGarfield application executable
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "MCApplication.h"

#ifdef USE_GEANT4
#include "RunConfiguration.h"
#include "TGeant4.h"
#endif

#ifdef USE_GEANT3
#include "TGeant3TGeo.h"
#endif

#include "TThread.h"
#include "TInterpreter.h"

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
  VMC::ExGarfield::MCApplication* appl
    =  new VMC::ExGarfield::MCApplication("ExampleExGarfield",
                                            "The exampleExGarfield MC application");

#ifdef USE_GEANT4
  // RunConfiguration for Geant4
  VMC::ExGarfield::RunConfiguration* runConfiguration
    = new VMC::ExGarfield::RunConfiguration("geomRootToGeant4", "FTFP_BERT");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration,
                  argc, argv);

  // Customise Geant4 setting
  // Fast simulation model configuration
  // + verbose level, global range cuts, etc.
  geant4->ProcessGeantMacro("g4config.in");
#endif

#ifdef USE_GEANT3
  new TGeant3TGeo("C++ Interface to Geant3");
#endif

  // Run example
  appl->InitMC("");

#ifdef USE_GEANT4
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config2.in");
  // Setting Geant4 visualization
  geant4->ProcessGeantMacro("g4vis.in");
#endif

  appl->RunMC(5);

  delete appl;
}
