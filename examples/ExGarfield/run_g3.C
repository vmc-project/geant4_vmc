//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup ExGarfield
/// \file ExGarfield/run_g3.C
/// \brief Macro for running Example ExGarfield with Geant3

#include "set_g3_vis.C"

void run_g3(const TString& configMacro = "g3tgeoConfig.C")
{
/// Macro function for running Example ExGarfield with Geant3 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref ExGarfield/g3tgeoConfig.C

  // MC application
  VMC::Garfield::MCApplication* appl
    =  new VMC::ExGarfield::MCApplication("ExampleExGarfield", "The example ExGarfield MC application");
  appl->InitMC(configMacro);

  // visualization setting
  set_g3_vis();

  appl->RunMC(5);

  delete appl;
}
