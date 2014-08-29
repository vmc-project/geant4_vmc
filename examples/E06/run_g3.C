// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007-2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E06
/// \file E06/run_g3.C
/// \brief Macro for running Example06 with Geant3 

void run_g3(const TString& configMacro = "g3tgeoConfig.C") 
{
/// Macro function for running Example06 with Geant3 from
/// Root interactive session
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g3.C.
/// \param configMacro configuration macro name, default \ref E06/g3Config.C 

  // MC application
  Ex06MCApplication* appl 
    = new Ex06MCApplication("Example06", "The example06 MC application");
    
  appl->GetPrimaryGenerator()->SetNofPrimaries(10);  
  //appl->SetVerboseLevel(3);  

  // Initialize MC
  appl->InitMC(configMacro);
  
  // Activate storing tracks
  gMC->SetCollectTracks(kTRUE);
  
  // Run MC
  appl->RunMC(5);

  delete appl;
}  
