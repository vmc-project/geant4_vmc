//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file exampleTR.cxx
/// \brief The Geant4 VMC example TR application executable
///
/// \date 18/12/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "MCApplication.h"
#include "PrimaryGenerator.h"

#ifdef USE_GEANT4
#include "TG4RunConfiguration.h"
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
  VMC::TR::MCApplication* appl
    =  new VMC::TR::MCApplication("ExampleTR",
                                  "The exampleTR MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(1);
  appl->SetPrintModulo(10000);

#ifdef USE_GEANT4
  // RunConfiguration for Geant4 
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRootToGeant4", "emStandard", "", false, false);

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration,
                  argc, argv);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
#endif
  
#ifdef USE_GEANT3
  new TGeant3TGeo("C++ Interface to Geant3");
  gMC->SetProcess("DRAY",1);
  gMC->SetProcess("LOSS",1);
  gMC->SetProcess("HADR",0);
#endif

  // Run example
  appl->InitMC("");

#ifdef USE_GEANT4
  // Customise Geant4 setting after initialization:
  geant4->ProcessGeantMacro("g4config2.in");

  // Setting Geant4 visualization
  // geant4->ProcessGeantMacro("g4vis.in");
#endif

  appl->RunMC(5000);

  delete appl;
}  
