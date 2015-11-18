//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Gflash
/// \file Gflash/run_g3.C
/// \brief Macro for running ExampleGflash with Geant3 

#include "set_vis.C"

void run_g3(const TString& configMacro = "g3tgeoConfig.C") 
{
/// Macro function for running ExampleGflash with Geant3 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref Gflash/g3Config.C 

  // MC application
  Gflash::MCApplication* appl 
    =  new Gflash::MCApplication("ExampleGflash", "The exampleGflash MC application");
  appl->InitMC(configMacro);

  // visualization setting
  set_vis();

  appl->RunMC(10);
  
  delete appl;
}  
