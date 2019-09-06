//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup ExGarfield
/// \file ExGarfield/run_g4.C
/// \brief Macro for running Example ExGarfield with Geant4.

#include "TVirtualMC.h"

#include "set_g4_vis.C"

void run_g4(const TString& configMacro = "g4Config.C")
{
/// Macro function for running Example ExGarfield with Geant4 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4.C.
/// \param configMacro configuration macro name, default \ref ExGarfield/g4Config.C

  // MC application
  VMC::ExGarfield::MCApplication* appl
    =  new VMC::ExGarfield::MCApplication("ExampleExGarfield", "The example ExGarfield MC application");
  appl->InitMC(configMacro);

  // Customise Geant4 setting after initialization:
  ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");

  // Visualization setting
  set_g4_vis();

  // Enter in Geant4 interactive session
  //((TGeant4*)gMC)->StartGeantUI();

  appl->RunMC(5);

  //delete appl;
}
