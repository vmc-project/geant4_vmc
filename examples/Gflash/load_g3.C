//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Gflash
/// \file Gflash/load_g3.C
/// \brief Macro for loading libraries to run Gflash example with Geant3.

#include "../macro/g3libs.C"

void load_g3()
{
  // Load Geant4 libraries
  g3libs();

  // Load this example library
  gSystem->Load("libvmc_Gflash");
}
