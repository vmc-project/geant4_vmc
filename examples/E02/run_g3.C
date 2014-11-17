//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E02
/// \file E02/run_g3.C
/// \brief Macro for running Example02 with Geant3 

void run_g3(const TString& configMacro = "g3tgeoConfig.C") 
{
/// Macro function for running Example02 with Geant3 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref E02/g3Config.C 

  // MC application
  Ex02MCApplication* appl 
    = new Ex02MCApplication("Example02", "The example02 MC application");

  // Initialize MC
  appl->InitMC(configMacro);
  
  gMC->SetCollectTracks(kTRUE);
  
  // Run MC
  appl->RunMC(5);

  delete appl;
}  
