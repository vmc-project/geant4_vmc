// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007-2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup A01
/// \file A01/load_g4.C
/// \brief Macro for loading libraries to run example A01 with Geant4. 

void load_g4() 
{
  // Load Geant4 + VMC libraries
  gROOT->LoadMacro("../macro/g4libs.C");
  g4libs();

  // Load this example library
  gSystem->Load("libmtroot");
  gSystem->Load("libvmc_A01");

  // Load library with a user run configuration
  gSystem->Load("libgeant4_A01");
}  
