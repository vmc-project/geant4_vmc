//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Gflash/set_g4_vis.C
/// \brief Macro for setting G4 visualization for example Gflash

#include "TVirtualMC.h"

void set_g4_vis()
{
/// Macro for setting G4 visualization for example Gflash

  ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");
}
