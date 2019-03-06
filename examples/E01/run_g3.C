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
/// \brief Macro for running Example01 with Geant3

void run_g3(const TString& configMacro = "g3tgeoConfig.C")
{
/// Macro function for running Example01 with Geant3 from
/// Root interactive session.
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref E01/g3Config.C

  // MC application
  Ex01MCApplication* appl
    = new Ex01MCApplication("Example01", "The example01 MC application");

  // Initialize MC
  appl->InitMC(configMacro);

  // Run MC
  appl->RunMC(1);

  delete appl;
}
