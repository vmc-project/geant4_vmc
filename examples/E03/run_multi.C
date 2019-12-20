//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/run_multi.C
/// \brief Macro for running Example03 with mixed Geant3 and Geant4.

// #include "set_g3_vis.C"
// #include "set_g4_vis.C"

void run_multi(const TString& configMacro1 = "g3tgeoConfig.C", const TString& configMacro2 = "g4tgeoConfig4Seq.C")
{
/// Macro function for running Example03c with Geant3 and Geant4 together from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_multi.C.
/// \param configMacro configuration macro names, default \ref E03/g3tgeoConfig.C and E03/g4tgeoConfig4Seq.C


// MC application
  auto appl = new Ex03MCApplication("Example03", "The example03 MC application", kTRUE, kTRUE);
  // appl->SetVerboseLevel(4);
  appl->GetPrimaryGenerator()->SetNofPrimaries(20);
  appl->SetPrintModulo(1);

  appl->InitMC({configMacro1.Data(), configMacro2.Data()});

  // visualization setting
  // Not available with multi engine run
  // set_g3_vis();
  // set_g4_vis();

  appl->RunMC(5);

  delete appl;
}
