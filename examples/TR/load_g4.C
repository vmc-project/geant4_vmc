//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup TR
/// \file TR/run_g4.C
/// \brief Macro for loading libraries to run ExamplTR with Geant4. 

#include "../macro/g4libs.C"

void load_g4() 
{
  // Load Geant4 + VMC libraries
  g4libs();

  // Load this example library
  gSystem->Load("libmtroot");
  gSystem->Load("libvmc_TR");
}  
