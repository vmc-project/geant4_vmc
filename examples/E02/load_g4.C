// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007-2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E02
/// \file E02/run_g4.C
/// \brief Macro for loading libraries to run Example02 with Geant4. 

void load_g4() 
{
  // Load Geant4 + VMC libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();

  // Load this example library
  gSystem->Load("libmtroot");
  gSystem->Load("libvmc_E02");
}  
