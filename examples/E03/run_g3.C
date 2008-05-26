// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/run_g3.C
/// \brief Macro for running Example03 with Geant3 

void run_g3(const TString& configMacro = "g3tgeoConfig.C") 
{
/// Macro function for running Example03 with Geant3 from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E03/g3Config.C 

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();
  
  // Load Geant3 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();
  
  // Load this example library
  gSystem->Load("libexample03");

  // MC application
  Ex03MCApplication* appl 
    =  new Ex03MCApplication("Example03", "The example03 MC application");
  appl->GetPrimaryGenerator()->SetNofPrimaries(20);
  appl->SetPrintModulo(1);

  appl->InitMC(configMacro);

  // visualization setting
  gROOT->LoadMacro("set_vis.C");
  set_vis();

  appl->RunMC(5);
  
  delete appl;
}  
