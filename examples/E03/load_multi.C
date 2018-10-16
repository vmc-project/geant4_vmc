//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/run_g4.C
/// \brief Macro for loading libraries to run ExamplE03 with Geant4.

#include "../macro/g3g4libs.C"

void load_multi()
{
  // Load Geant4 + VMC libraries
  g3libs();
  g4libs();

  // Load this example library
  gSystem->Load("libmtroot");
  gSystem->Load("libvmc_E03c");

  // Load library with a user run configuration
  gSystem->Load("libgeant4_E03c");
}
