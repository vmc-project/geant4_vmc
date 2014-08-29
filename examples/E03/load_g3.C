// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007-2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/load_g3.C
/// \brief Macro for loading libraries to run Example03 with Geant3. 

void load_g3()
{
  // Load Geant4 libraries
  gROOT->LoadMacro("../macro/g3libs.C");
  g3libs();

  // Load this example library
  gSystem->Load("libmtroot");
  gSystem->Load("libexample03");
}  
