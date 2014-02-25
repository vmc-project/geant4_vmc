//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file exampleE06.cxx
/// \brief The Geant4 VMC example E06 application executable
///
/// The Geant4 VMC application executable
/// with explicitely instantiated TGeant3 or TGeant4 and linked 
/// with all libraries.
///
/// \date 31/01/2014
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

#include "TThread.h"
#include "TROOT.h"
#include "TInterpreter.h"

#include <string>

namespace {

// Utility function
void ProcessMacro(int argc, char** argv) {
  //std::cout << "Program arguments " << argc << "  ";
  //for (G4int i=0; i< argc; ++i) { std::cout << argv[i] << ",  "; }
  //std::cout << std::endl;

  Int_t counter = 1;
  std::string macroName = argv[counter];
  std::string functionName = macroName;
  functionName.erase(functionName.find(".C"), 2);
  functionName += "(";
  while ( ++counter < argc ) {
    functionName += argv[counter];
    if ( counter < (argc - 1) ) functionName += ",";
  }
  functionName += ")";
  //std::cout << "macroName: " << macroName << std::endl;
  //std::cout << "functionName: " << functionName << std::endl;

  gROOT->LoadMacro(macroName.data());
  gInterpreter->ProcessLine(functionName.data());
}

}

int main(int argc, char** argv)
{
  // Initialize Root threading.
  // (Multi-threading is triggered automatically if Geant4 was built 
  //  in MT mode.)
#ifdef G4MULTITHREADED
   TThread::Initialize();
#endif

  // Create MC application (thread local)
  Ex06MCApplication* appl 
    =  new Ex06MCApplication("ExampleE06", 
                             "The exampleE06 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(10);  
  //appl->SetVerboseLevel(3);  

#ifdef USE_GEANT4
  // RunConfiguration for Geant4 
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRootToGeant4", "emStandard+optical");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration,
                  argc, argv);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
#endif
  
#ifdef USE_GEANT3
  TGeant3* geant3
    = new TGeant3TGeo("C++ Interface to Geant3");
  gMC->SetProcess("CKOV",1);
  gMC->SetCut("CUTELE",10e-06);
  gMC->SetCut("CUTGAM",10e-06);
  geant3->SetSWIT(4,1000);
      // reduce printing from GTREVE_ROOT (sets one printing per 1000 tracks) 
#endif

  // Run example
  if ( argc <= 1 ) {
    // Initialize MC
    appl->InitMC("");
  
    // Activate storing tracks
    // gMC->SetCollectTracks(kTRUE);

#ifdef USE_GEANT4
    // Customise Geant4 setting after initialization:
    // Physics list
    geant4->ProcessGeantMacro("g4config2.in");
    // Visualization settings
    geant4->ProcessGeantMacro("g4vis.in");
#endif
  
    // Run MC
    appl->RunMC(5);
  }
  else {
    // Run from Root macro
    ProcessMacro(argc, argv);
  }

  delete appl;
}  
