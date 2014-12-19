//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E01
/// \file E01/run_g4.C
/// \brief Macro for loading libraries to run Example01 with Geant4.

#include "../macro/g4libs.C"

void load_g4() 
{
  // Load Geant4 + VMC libraries
  g4libs();
  
  // Load this example library
  gSystem->Load("libvmc_E01");
}  
