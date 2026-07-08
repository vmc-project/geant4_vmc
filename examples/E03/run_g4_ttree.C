//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/run_g4.C
/// \brief Macro for running Example03 with Geant4.

#include "set_g4_vis.C"

void run_g4_ttree(const TString& configMacro = "g4Config.C")
{
/// Macro function for running Example03 with Geant4 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4.C.
/// \param configMacro configuration macro name, default \ref E03/g4Config.C

  // MC application
  Ex03dMCApplication* appl
    =  new Ex03dMCApplication("Example03", "The example03 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(20);
  appl->SetPrintModulo(1);

  appl->InitMC(configMacro, TMCRootManager::kTTree);

  // Visualization setting
  set_g4_vis();

  // Enter in Geant4 interactive session
  //((TGeant4*)gMC)->StartGeantUI();

  appl->RunMC(100);

  delete appl;
}
