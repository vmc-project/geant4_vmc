//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/load_g3b.C
/// \brief Macro for loading libraries to run example E03b with Geant3.

#include "../macro/g3libs.C"

void load_g3b()
{
  // Load Geant4 libraries
  g3libs();

  // Load this example library
  gSystem->Load("libvmc_E03b");
}
