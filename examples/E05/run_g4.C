//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E05
/// \file E05/run_g4.C
/// \brief Macro for running Example05 with Geant4. 

#include "set_vis.C"

void run_g4(const TString& configMacro = "g4Config.C")
{
/// Macro function for running Example05 with Geant4 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4.C.
/// \param configMacro configuration macro name, default \ref E05/g4Config.C 

  // MC application
  Ex05MCApplication* appl 
    =  new Ex05MCApplication("Example05", "The example05 MC application");
  appl->InitMC(configMacro);

  // Visualization setting
  set_vis();

  // Enter in Geant4 interactive session
  //((TGeant4*)gMC)->StartGeantUI();

  appl->RunMC(10);
  
  delete appl;
}  
