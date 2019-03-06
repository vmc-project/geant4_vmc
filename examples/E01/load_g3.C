//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E01
/// \file E01/run_g3.C
/// \brief Macro for loading libraries to run Example01 with Geant3.

#include "../macro/g3libs.C"

void load_g3()
{
  // Load Geant3 + VMC libraries
  g3libs();

  // Load this example library
  gSystem->Load("libvmc_E01");
}
