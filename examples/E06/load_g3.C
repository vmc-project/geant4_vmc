//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E06
/// \file E06/load_g3.C
/// \brief Macro for loading libraries to run Example06 with Geant3. 

void load_g3()
{
  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();

  // Load this example library
  gSystem->Load("libmtroot");
  gSystem->Load("libvmc_E06");
}  
