//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/load_g4a.C
/// \brief Macro for loading libraries to run exampl E03a with Geant4.

#include "../macro/g4libs.C"

void load_g4a()
{
  // Load Geant4 + VMC libraries
  g4libs();

  // Load this example library
  gSystem->Load("libvmc_E03a");

  // Load library with a user run configuration
  gSystem->Load("libgeant4_E03a");
}
