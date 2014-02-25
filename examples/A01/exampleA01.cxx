//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file exampleA01.cxx
/// \brief The Geant4 VMC example A01 application executable
///
/// The Geant4 VMC application executable
/// with explicitely instantiated TGeant3 or TGeant4 and linked 
/// with all libraries.
///
/// \date 31/01/2014
/// \author I. Hrivnacova; IPN, Orsay

#include "A01MCApplication.h"
#include "A01PrimaryGenerator.h"

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
  A01MCApplication* appl 
    =  new A01MCApplication("ExampleA01", 
                            "The examplA01 MC application");

#ifdef USE_GEANT4
  // RunConfiguration for Geant4 
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRootToGeant4", "FTFP_BERT");

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration,
                  argc, argv);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
  // Setting Geant4 visualization
  geant4->ProcessGeantMacro("g4vis.in");
#endif
  
#ifdef USE_GEANT3
  new TGeant3TGeo("C++ Interface to Geant3");
  gMC->SetProcess("DRAY",1);
  gMC->SetProcess("LOSS",1);
  gMC->SetProcess("HADR",1);
#endif

  // Run example
  if ( argc <= 1 ) {
    // Run from this main
    appl->GetPrimaryGenerator()->SetRandomize(false);
    appl->SetWriteStack(true);
    appl->SetWriteHits(true);
    appl->InitMC("");
    appl->RunMC(5);
  }
  else {
    // Run from Root macro
    ProcessMacro(argc, argv);
  }

  delete appl;
}  
