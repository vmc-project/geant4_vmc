//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup A01
/// \file A01/run_g3.C
/// \brief Macro for running Example A01 with Geant3

#include "set_g3_vis.C"

void run_g3(const TString& configMacro = "g3tgeoConfig.C")
{
/// Macro function for running Example A01 with Geant3 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4.C.
/// \param configMacro configuration macro name, default \ref E03/g3Config.C

  // MC application
  A01MCApplication* appl
    =  new A01MCApplication("ExampleA01", "The exampleA01 MC application");
  appl->GetPrimaryGenerator()->SetRandomize(false);
  appl->SetWriteStack(true);
  appl->SetWriteHits(true);

  appl->InitMC(configMacro);

  // visualization setting
  set_g3_vis();

  TStopwatch timer;
  timer.Start();
  appl->RunMC(5);
  timer.Stop();
  timer.Print();

  delete appl;
}
