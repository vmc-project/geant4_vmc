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

  // Create MC application
  VMC::Monopole::MCApplication* appl 
    =  new VMC::Monopole::MCApplication("ExampleMonopole",
                                        "The exampleMonopole MC application");

#ifdef USE_GEANT4
  // RunConfiguration for Geant4 
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRootToGeant4");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration,
                  argc, argv);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  // geant4->ProcessGeantMacro("g4config.in");
  geant4->ProcessGeantCommand("/mcVerbose/all 1");  
#endif
  
#ifdef USE_GEANT3
  TGeant3* geant3 = new TGeant3TGeo("C++ Interface to Geant3");
  geant3->SetHADR(0);
#endif

  // Run from this main
  appl->InitMC("");
  appl->RunMC(1);

  delete appl;
}  
