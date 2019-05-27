//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E06
/// \file E06/g4Config2.C
/// \brief Configuration macro for Geant4 VirtualMC for Example06
///
/// Customise Geant4 setting after initialization:
 
void g4Config2()
{
/// The configuration function for Geant4 VMC for Example06
/// called after MC application initialization.

  ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");
}
