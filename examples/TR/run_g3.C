//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup TR
/// \file TR/run_g3.C
/// \brief Macro for running Example TR with Geant3

#include "set_g3_vis.C"

void run_g3(const TString& configMacro = "g3tgeoConfig.C")
{
/// Macro function for running Example TR with Geant3 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref TR/g3Config.C

  // MC application
  VMC::TR::MCApplication* appl
    =  new VMC::TR::MCApplication("ExampleTR", "The exampleTR MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(1);
  appl->SetPrintModulo(10000);

  appl->InitMC(configMacro);

  // visualization setting
  // set_g3_vis();

  appl->RunMC(5000);

  delete appl;
}
