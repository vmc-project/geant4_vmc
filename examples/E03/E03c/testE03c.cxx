//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file testE03c.cxx
/// \brief The Geant4 VMC example E03c test application
///
/// The Geant4 VMC test application
/// with explicitely instantiated TGeant3 or TGeant4 and linked
/// with all libraries.
///
/// <pre>
/// Usage:
/// testE03c
///   [-g4g,  --g4-geometry]:        Geant4 VMC geometry option
///   [-g4pl, --g4-physics-list]:    Geant4 physics list selection
///   [-g4sp, --g4-special-physics]: Geant4 special physics selection
///   [-g4m,  --g4-macro]:           Geant4 macro
///   [-g4vm, --g4-vis-macro]:       Geant4 visualization macro
///   [-g3g,  --g3-geometry]:        Geant3 geometry option
///   (TGeant3,TGeant3TGeo)
///   [-r4m,  --root-macro]:         Root macro
///   [-v,    --verbose]:            verbose option (yes,no)
///
/// Note that the g4* and g3* options are available only when built
/// with the corresponding VMC_WITH_Geant4 or VMC_WITH_Geant3 option.
/// Root macro with arguments should be passed within '', eg.
///  --root-macro 'test_E03c_1.C("",kFALSE)'
/// </pre>
///
/// \date 30/04/2019
/// Benedikt Volkel, CERN

#include "Ex03cMCApplication.h"
#include "Ex03PrimaryGenerator.h"

#ifdef USE_GEANT4
#include "Ex03RunConfiguration1.h"
#include "Ex03RunConfiguration2.h"
#include "Ex03RunConfiguration3.h"
#include "Ex03RunConfiguration4.h"
#include "TG4RunConfiguration.h"
#include "TGeant4.h"
#endif

#ifdef USE_GEANT3
#include "TGeant3TGeo.h"
#endif

#include "TInterpreter.h"
#include "TROOT.h"
#include "TThread.h"

#include <iostream>
#include <string>

// Define application parameters in global space
#ifdef USE_GEANT4
std::string g4Geometry = "geomRoot";
std::string g4PhysicsList = "FTFP_BERT";
std::string g4SpecialPhysics = "stepLimiter";
std::string g4Macro = "g4config.in";
std::string g4VisMacro = "g4vis.in";
std::string g4Session = "";
std::string g4UserClass = "";
#endif
#ifdef USE_GEANT3
std::string g3Geometry = "TGeant3TGeo";
#endif
std::string firstEngine = "g3";
std::string rootMacro = "";
std::string verbose = "yes";

namespace
{

/// Prints usage on error output stream
void PrintUsage(const std::string& programName)
{
  std::cerr
    << "\nUsage:\n"
    << " " << programName << "\n"
#ifdef USE_GEANT4
    << "   [-g4g,  --g4-geometry]:        Geant4 VMC geometry option\n"
    << "   [-g4pl, --g4-physics-list]:    Geant4 physics list selection\n"
    << "   [-g4sp, --g4-special-physics]: Geant4 special physics selection\n"
    << "   [-g4m,  --g4-macro]:           Geant4 macro\n"
    << "   [-g4vm, --g4-vis-macro]:       Geant4 visualization macro\n"
    << "   [-g4uc, --g4-user-class]:      Geant4 user class \n"
    << "                                  (geometry, regions, physics-list, "
       "field)\n"
#endif
#ifdef USE_GEANT3
    << "   [-g3g,  --g3-geometry]:        Geant3 geometry option (TGeant3[TGeo])\n"
#endif
    << "   [-r4m,  --root-macro]:         Root macro\n"
    << "   [-v,    --verbose]:            verbose option (yes,no)\n"
#ifdef USE_MULTI
    << "   [-fe,   --first-engine]:       first engine (g3,g4)\n"
#endif
    << std::endl;
}

/// Prints selected configuration on output stream (Geant4)
void PrintConfiguration(const std::string& programName)
{
  std::cout
    << " Running " << programName << " with options:" << std::endl;
#ifdef USE_GEANT4
  std::cout
    << "   --g4-geometry:        " << g4Geometry << std::endl
    << "   --g4-physics-list:    " << g4PhysicsList << std::endl;
  if (g4SpecialPhysics.size()) {
    std::cout << "   --g4-special-physics: " << g4SpecialPhysics << std::endl;
  }
  if (g4Macro.size()) {
    std::cout << "   --g4-macro:           " << g4Macro << std::endl;
  }
  if (g4VisMacro.size()) {
    std::cout << "   --g4-vis-macro:       " << g4VisMacro << std::endl;
  }
  if (g4Session.size()) {
    std::cout << "   --g4-session:         " << g4Session << std::endl;
  }
  if (g4UserClass.size()) {
    std::cout << "   --g4-user-class:      " << g4UserClass << std::endl;
  }
#endif
#ifdef USE_GEANT3
  std::cout << "   --g3-geometry:        " << g3Geometry << std::endl;
#endif
#ifdef USE_MULTI
  std::cout << "   --first-engine:       " << firstEngine << std::endl;
#endif
  if (rootMacro.size()) {
    std::cout << "   --root-macro:         " << rootMacro << std::endl;
  }
}

void CreateGeant4(int argc, char** argv)
{
#ifdef USE_GEANT4
  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration = 0;
  if (!g4UserClass.size()) {
    runConfiguration =
      new TG4RunConfiguration(g4Geometry, g4PhysicsList, g4SpecialPhysics, false, false);
  }
  else if (g4UserClass == "geometry") {
    runConfiguration =
      new Ex03RunConfiguration1(g4PhysicsList, g4SpecialPhysics);
  }
  else if (g4UserClass == "physics-list") {
    runConfiguration = new Ex03RunConfiguration2(g4Geometry, g4SpecialPhysics);
  }
  else if (g4UserClass == "regions") {
    runConfiguration =
      new Ex03RunConfiguration3(g4Geometry, g4PhysicsList, g4SpecialPhysics);
  }
  else if (g4UserClass == "field") {
    runConfiguration =
      new Ex03RunConfiguration4(g4Geometry, g4PhysicsList, g4SpecialPhysics, false, false);
  }
  else {
    PrintUsage("testE03s");
    return;
  }

  // TGeant4
  TGeant4* geant4 = new TGeant4(
    "TGeant4", "The Geant4 Monte Carlo", runConfiguration, argc, argv);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  if (g4Macro.size()) {
    geant4->ProcessGeantMacro(g4Macro.data());
  }
#endif
}

void CreateGeant3()
{
#ifdef USE_GEANT3
  TGeant3TGeo* geant3
    = new TGeant3TGeo("C++ Interface to Geant3");
  geant3->SetProcess("DRAY", 1);
  geant3->SetProcess("LOSS", 1);
  geant3->SetProcess("HADR", 0);
#endif
}

} // namespace

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

// Process arguments
  for (Int_t i = 1; i < argc; i = i + 2) {
    std::cout
      << "processing " << argv[i] << " with " << argv[i + 1] << std::endl;
    if (std::string(argv[i]) == "--root-macro" ||
             std::string(argv[i]) == "-rm")
      rootMacro = argv[i + 1];
    else if (std::string(argv[i]) == "--verbose" ||
             std::string(argv[i]) == "-v")
      verbose = argv[i + 1];
#ifdef USE_GEANT4
    else if (std::string(argv[i]) == "--g4-geometry" ||
        std::string(argv[i]) == "-g4g")
      g4Geometry = argv[i + 1];
    else if (std::string(argv[i]) == "--g4-physics-list" ||
             std::string(argv[i]) == "-g4pl")
      g4PhysicsList = argv[i + 1];
    else if (std::string(argv[i]) == "--g4-special-physics" ||
             std::string(argv[i]) == "-g4sp")
      g4SpecialPhysics = argv[i + 1];
    else if (std::string(argv[i]) == "--g4-macro" ||
             std::string(argv[i]) == "-g4m")
      g4Macro = argv[i + 1];
    else if (std::string(argv[i]) == "--g4-vis-macro" ||
             std::string(argv[i]) == "-g4vm")
      g4VisMacro = argv[i + 1];
    else if (std::string(argv[i]) == "--g4-session" ||
             std::string(argv[i]) == "-g4s")
      g4Session = argv[i + 1];
    // the following option is specific to use of Geant4 dependent classes
    else if (std::string(argv[i]) == "--g4-user-class" ||
             std::string(argv[i]) == "-g4uc")
      g4UserClass = argv[i + 1];
#endif
#ifdef USE_GEANT3
    else if (std::string(argv[i]) == "--g3-geometry" ||
        std::string(argv[i]) == "-g3g")
      g3Geometry = argv[i + 1];
#endif
#ifdef USE_MULTI
    else if (std::string(argv[i]) == "--first-engine" ||
             std::string(argv[i]) == "-fe")
      firstEngine = argv[i + 1];
#endif
    else {
      PrintUsage("testE03c");
      return 1;
    }
  }

  if (verbose == "yes") {
    PrintConfiguration("testE03c");
  }
  //
  // end of code to process arguments

  Bool_t isMulti = false;
  Int_t debug = 0;
#ifdef USE_MULTI
  isMulti = true;
  debug = 1;
#endif

  // Create MC application (thread local)
  Ex03cMCApplication* appl =
    new Ex03cMCApplication("ExampleE03", "The exampleE03 MC application", isMulti, isMulti);
  appl->SetDebug(debug);

  if (firstEngine == "g3") {
    CreateGeant3();
    CreateGeant4(argc, argv);
  } else {
    CreateGeant4(argc, argv);
    CreateGeant3();
  }

  if (!rootMacro.size()) {
    // Run example without Root macro
#ifdef USE_MULTI
     appl->InitMC();
#else
     appl->InitMC("");
#endif

    // Setting Geant4 visualization
// #ifdef USE_GEANT4
// #ifndef USE_MULTI
//     if (g4VisMacro.size()) {
//       geant4->ProcessGeantMacro(g4VisMacro.data());
//     }
// #endif
// #endif
    appl->RunMC(5);
  }
  else {
    // Run from Root macro
    std::cout << "Go to call macro" << std::endl;
    gROOT->Macro(rootMacro.data());
  }

  delete appl;
}
