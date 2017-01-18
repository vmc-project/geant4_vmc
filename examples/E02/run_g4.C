//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E02
/// \file E02/run_g4.C
/// \brief Macro for running Example02 with Geant4. 

#include "TVirtualMC.h"

void run_g4(const TString& configMacro = "g4Config.C") 
{
/// Macro function for running Example02 with Geant4 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref E02/g4Config.C 

  // MC application
  Ex02MCApplication* appl 
    =  new Ex02MCApplication("Example02", "The example02 MC application");

  appl->InitMC(configMacro);
  
  // TODO: pass this infor from master to workers
  //gMC->SetCollectTracks(kTRUE);
 
  // Setting Geant4 visualization
  ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");

  // Enter in Geant4 interactive session
  //((TGeant4*)gMC)->StartGeantUI();
  
  appl->RunMC(5);

  delete appl;
}  
