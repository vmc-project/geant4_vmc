//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/run_g3.C
/// \brief Macro for running Example03 with mixed GEANT3 and GEANT4

#include "set_vis.C"

void run_multi(const TString& configMacro1 = "g3tgeoConfig.C", const TString& configMacro2 = "g4tgeoConfig4.C")
{
/// Macro function for running Example03 with Geant3 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref E03/g3Config.C

#ifdef G4MULTITHREADED
   std::cerr << "G4 compiled with multithreading enabled. Not running with multi-engines."
   exit(0);
#endif

// MC application
  auto appl = new Ex03MCApplication("Example03", "The example03 MC application", kTRUE, kTRUE);
  appl->SetVerboseLevel(4);
  appl->GetPrimaryGenerator()->SetNofPrimaries(20);
  appl->SetPrintModulo(1);

  appl->InitMC({configMacro1.Data(), configMacro2.Data()});

  // visualization setting
  // set_vis();

  appl->RunMC(1);
  appl->ExportGeometry("geometry.root");

  delete appl;
}
