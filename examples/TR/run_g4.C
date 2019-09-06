//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup TR
/// \file TR/run_g4.C
/// \brief Macro for running Example TR with Geant4.

#include "TVirtualMC.h"

#include "set_g4_vis.C"

void run_g4(const TString& configMacro = "g4Config.C")
{
/// Macro function for running Example TR with Geant4 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4.C.
/// \param configMacro configuration macro name, default \ref TR/g4Config.C

  // MC application
  VMC::TR::MCApplication* appl
    =  new VMC::TR::MCApplication("ExampleTR", "The exampleTR MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(1);
  appl->SetPrintModulo(10000);

  appl->InitMC(configMacro);

  // Customise Geant4 setting after initialization:
  ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");

  // Visualization setting
  // set_g4_vis();

  // Enter in Geant4 interactive session
  //((TGeant4*)gMC)->StartGeantUI();

  appl->RunMC(5000);

  delete appl;
}
