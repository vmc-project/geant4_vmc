//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2015 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file testExGarfield.cxx
/// \brief The Geant4 VMC example ExGarfield test application
///
/// The Geant4 VMC test application
/// with explicitely instantiated TGeant3 or TGeant4 and linked
/// with all libraries.
///
/// <pre>
/// Usage:
/// testExGarfield
///   [-g4g,  --g4-geometry]:        Geant4 VMC geometry option
///   [-g4pl, --g4-physics-list]:    Geant4 physics list selection
///   [-g4sp, --g4-special-physics]: Geant4 special physics selection
///   [-g4m,  --g4-macro]:           Geant4 macro
///   [-g4vm, --g4-vis-macro]:       Geant4 visualization macro
///   [-g3g,  --g3-geometry]:        Geant3 geometry option
///   (TGeant3,TGeant3TGeo)
///   [-rm,   --root-macro]:         Root macro
///   [-v,    --verbose]:            verbose option (yes,no)
///
/// Note that the g4* and g3* options are available only when built
/// with the corresponding VMC_WITH_Geant4 or VMC_WITH_Geant3 option.
/// Root macro with arguments should be passed within '', eg.
///  --root-macro 'test_ExGarfield.C("",kFALSE)'
/// </pre>
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

#include "TInterpreter.h"
#include "TROOT.h"
#include "TThread.h"

#include <iostream>
#include <string>

namespace
{

/// Prints usage on error output stream
void PrintUsage(std::string programName)
{
  std::cerr << " Usage: " << std::endl;
  std::cerr << " " << programName << std::endl;
#ifdef USE_GEANT4
  std::cerr << "   [-g4g,  --g4-geometry]:        Geant4 VMC geometry option"
            << std::endl;
  std::cerr << "   [-g4pl, --g4-physics-list]:    Geant4 physics list selection"
            << std::endl;
  std::cerr
    << "   [-g4sp, --g4-special-physics]: Geant4 special physics selection"
    << std::endl;
  std::cerr << "   [-g4m,  --g4-macro]:           Geant4 macro" << std::endl;
  std::cerr << "   [-g4vm, --g4-vis-macro]:       Geant4 visualization macro"
            << std::endl;
  std::cerr << "   [-g4uc, --g4-user-class]:      Geant4 user class "
            << "                                  (geometry, regions, "
               "physics-list, field)"
            << std::endl;
#endif
#ifdef USE_GEANT3
  std::cerr << "   [-g3g,  --g3-geometry]:        Geant3 geometry option "
               "(TGeant3,TGeant3TGeo)"
            << std::endl;
#endif
  std::cerr << "   [-rm,   --root-macro]:         Root macro" << std::endl;
  std::cerr << "   [-v,    --verbose]:            verbose option (yes,no)"
            << std::endl;
}

#ifdef USE_GEANT4
/// Prints selected configuration on output stream (Geant4)
void PrintG4Configuration(const std::string& programName,
  const std::string& g4Geometry, const std::string& g4PhysicsList,
  const std::string& g4SpecialPhysics, const std::string& g4Macro,
  const std::string& g4VisMacro, const std::string& g4Session,
  const std::string& rootMacro)
{
  std::cout << " Running " << programName << " with options:" << std::endl;
  std::cout << "   --g4-geometry:        " << g4Geometry << std::endl;
  std::cout << "   --g4-physics-list:    " << g4PhysicsList << std::endl;
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
  if (rootMacro.size()) {
    std::cout << "   --root-macro:         " << rootMacro << std::endl;
  }
}
#endif

#ifdef USE_GEANT3
/// Prints selected configuration on output stream (Geant3)
void PrintG3Configuration(const std::string& programName,
  const std::string& g3Geometry, const std::string& rootMacro)
{
  std::cout << " Running: " << std::endl;
  std::cout << " " << programName << std::endl;
  std::cout << "     --g3-geometry:        " << g3Geometry << std::endl;
  if (rootMacro.size()) {
    std::cout << "     --root-macro]:    " << rootMacro << std::endl;
  }
}
#endif

} // namespace

/// Application main program
int main(int argc, char** argv)
{
  // Initialize Root threading.
  // (Multi-threading is triggered automatically if Geant4 was built
  //  in MT mode.)
#ifdef G4MULTITHREADED
  ROOT::EnableThreadSafety();
#endif

  // Process arguments
  // This code is generic with the exception of the start values and
  // the program name
#ifdef USE_GEANT4
  std::string g4Geometry = "geomRootToGeant4";
  std::string g4PhysicsList = "FTFP_BERT";
  std::string g4SpecialPhysics = "stepLimiter";
  std::string g4Macro = "g4config.in";
  std::string g4VisMacro = "g4vis.in";
  std::string g4Session = "";
#endif
#ifdef USE_GEANT3
  std::string g3Geometry = "TGeant3TGeo";
#endif
  std::string rootMacro = "";
  std::string verbose = "yes";

  for (Int_t i = 1; i < argc; i = i + 2) {
    std::cout << "processing " << argv[i] << " with " << argv[i + 1]
              << std::endl;
#ifdef USE_GEANT4
    if (std::string(argv[i]) == "--g4-geometry" ||
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
#endif
#ifdef USE_GEANT3
    if (std::string(argv[i]) == "--g3-geometry" ||
        std::string(argv[i]) == "-g3g")
      g3Geometry = argv[i + 1];
#endif
    else if (std::string(argv[i]) == "--root-macro" ||
             std::string(argv[i]) == "-rm")
      rootMacro = argv[i + 1];
    else if (std::string(argv[i]) == "--verbose" ||
             std::string(argv[i]) == "-v")
      verbose = argv[i + 1];
    else {
      PrintUsage("testExGarfield");
      return 1;
    }
  }

  if (verbose == "yes") {
#ifdef USE_GEANT4
    PrintG4Configuration("testExGarfield", g4Geometry, g4PhysicsList,
      g4SpecialPhysics, g4Macro, g4VisMacro, g4Session, rootMacro);
#endif
#ifdef USE_GEANT3
    PrintG3Configuration("testExGarfield", g3Geometry, rootMacro);
#endif
  }
  //
  // end of code to process arguments

  // Create MC application (thread local)
  VMC::ExGarfield::MCApplication* appl = new VMC::ExGarfield::MCApplication(
    "ExampleExGarfield", "The example ExGarfield MC application");

#ifdef USE_GEANT4
  // RunConfiguration for Geant4
  VMC::ExGarfield::RunConfiguration* runConfiguration =
    new VMC::ExGarfield::RunConfiguration(
      g4Geometry, g4PhysicsList, g4SpecialPhysics);

  // TGeant4
  TGeant4* geant4 = new TGeant4(
    "TGeant4", "The Geant4 Monte Carlo", runConfiguration, argc, argv);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  if (g4Macro.size()) {
    geant4->ProcessGeantMacro(g4Macro.data());
  }
#endif

#ifdef USE_GEANT3
  if (g3Geometry == "TGeant3") {
    new TGeant3("C++ Interface to Geant3");
  }
  else if (g3Geometry == "TGeant3TGeo") {
    new TGeant3TGeo("C++ Interface to Geant3");
  }
  else {
    PrintUsage("testGarfield");
    return 1;
  }
#endif

  // Run example
  if (!rootMacro.size()) {
    appl->InitMC("");
#ifdef USE_GEANT4
    // Customise Geant4 setting
    // (verbose level, global range cut, ..)
    geant4->ProcessGeantMacro("g4config2.in");
    // Setting Geant4 visualization
    if (g4VisMacro.size()) {
      geant4->ProcessGeantMacro(g4VisMacro.data());
    }
#endif
    appl->RunMC(5);
  }
  else {
    // Run from Root macro
    gROOT->Macro(rootMacro.data());
  }

  delete appl;
}
