//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Garfield
/// \file Garfield/run_g4.C
/// \brief Macro for loading libraries to run ExampleGarfield with Geant4. 

#include "../macro/g4libs.C"

void load_g4() 
{
  // Load Geant4 + VMC libraries
  g4libs();

  // Load Garfield library
  gSystem->Load("libGarfield");

  // Load this example library
  gSystem->Load("libvmc_Garfield");

  // Load library with a user run configuration
  gSystem->Load("libgeant4_Garfield");
}  
