//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/set_g4_vis.C
/// \brief Macro for setting G4 visualization for Example TR

#include "TVirtualMC.h"

void set_g4_vis()
{
/// Macro for setting G4 visualization for Example TR

  ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");
}
