// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2011 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/test_E03_pl.C
/// \brief Macro for running Example03 with Geant4.

void test_E03_pl(const TString& configMacro = "g4ConfigEnv.C")
{
/// Macro function for running Example03 with Geant4 from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E03/g4Config.C 

  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();

  // Load this example library
  gSystem->Load("libexample03");
 
  // MC application
  Ex03MCApplication* appl 
    =  new Ex03MCApplication("Example03", "The example03 MC application");
  appl->InitMC(configMacro);
  
  appl->RunMC(1);
  
  delete appl;
}  
