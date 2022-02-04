//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup A01
/// \file A01/load_g4.C
/// \brief Macro for loading libraries to run example A01 with Geant4.

#include "../macro/g4libs.C"

void load_g4()
{
  // Load Geant4 + VMC libraries
  g4libs();

  // Load this example library
  gSystem->Load("libvmc_A01");

  // Load library with a user run configuration
  gSystem->Load("libgeant4_A01");
}
