//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E06
/// \file E06/run_g4.C
/// \brief Macro for running Example06 with Geant4. 

#include "TVirtualMC.h"

//#include "optPhoton.C"

void run_g4(const TString& configMacro = "g4Config.C") 
{
/// Macro function for running Example06 with Geant4 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4.C.
/// \param configMacro configuration macro name, default \ref E06/g4Config.C 

  // MC application
  Ex06MCApplication* appl 
    = new Ex06MCApplication("Example06", "The example06 MC application");
    
  appl->GetPrimaryGenerator()->SetNofPrimaries(10);  
  //appl->SetVerboseLevel(3);  
  
  // Macro to run with primary = polarized optical photon
  //optPhoton();  

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Customise Geant4 setting after initialization:
  // Physics list
  ((TGeant4*)gMC)->ProcessGeantMacro("g4config2.in");
  // Visualization settings
  ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");

  // Enter in Geant4 interactive session
  //((TGeant4*)gMC)->StartGeantUI();

  // Run MC
  appl->RunMC(5);

  delete appl;
}  
