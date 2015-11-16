//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E05
/// \file E05/run_g3.C
/// \brief Macro for running Example05 with Geant3 

#include "set_vis.C"

void run_g3(const TString& configMacro = "g3tgeoConfig.C") 
{
/// Macro function for running Example05 with Geant3 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref E05/g3Config.C 

  // MC application
  Ex05MCApplication* appl 
    =  new Ex05MCApplication("Example05", "The example05 MC application");
  appl->InitMC(configMacro);

  // visualization setting
  set_vis();

  appl->RunMC(10);
  
  delete appl;
}  
