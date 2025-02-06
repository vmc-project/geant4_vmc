//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2025 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file listPLOptions.cxx
/// \brief Print available base physics lists and EM options
///
/// Usage:
/// <pre>
/// ./listPLOptions [-b base lists ] [-em em options ] [-v verbose]
///    -b    list available base physics lists
///    -em   list available EM options
//     -v    precede lists with a title
/// </pre>
///
/// \date 20/01/2025
/// \author I. Hrivnacova; IJCLab, Orsay

#include "G4PhysListFactory.hh"
#include "G4String.hh"
#include "G4ios.hh"

#include <vector>

namespace
{

void PrintUsage()
{
  G4cerr << " Usage: " << G4endl
         << " listPLOptions [-b] [-em] [-v]" << G4endl
         << "        -b    list available base physics lists" << G4endl
         << "        -em   list available EM options" << G4endl
         << "        -v    precede lists with a title" << G4endl;
}

void PrintNameList(const std::vector<G4String>& nameList)
{
  for (auto name : nameList) {
    G4cout << name << " ";
  }
  G4cout << G4endl;
}

}  // namespace

int main(int argc, char** argv)
{
  // Evaluate arguments
  //
  if (argc > 5) {
    PrintUsage();
    return 1;
  }

  G4bool baseLists = false;;
  G4bool emOptions = false;
  G4bool verbose = false;
  for (G4int i = 1; i < argc; ++i) {
    if (G4String(argv[i]) == "-b")
      baseLists = true;
    else if (G4String(argv[i]) == "-em")
      emOptions = true;
    else if (G4String(argv[i]) == "-v")
      verbose = true;
    else {
      PrintUsage();
      return 1;
    }
  }

  if ( (! baseLists) && (! emOptions) ) {
    PrintUsage();
    return 1;
  }

  // Physics list factory
  G4PhysListFactory factory;

  if (baseLists) {
    if (verbose) {
      G4cout << "Base physics lists: ";
    }
    PrintNameList(factory.AvailablePhysLists());
  }

  if (emOptions) {
    if (verbose) {
      G4cout << "EM Options: ";
    }
    PrintNameList(factory.AvailablePhysListsEM());
  }
}
