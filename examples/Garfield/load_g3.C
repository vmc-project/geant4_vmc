//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Garfield
/// \file Garfield/load_g3.C
/// \brief Macro for loading libraries to run Garfield example with Geant3. 

#include "../macro/g3libs.C"

void load_g3()
{
  // Load Geant4 libraries
  g3libs();

  // Load this example library
  gSystem->Load("libmtroot");
  gSystem->Load("libvmc_Garfield");
}  
