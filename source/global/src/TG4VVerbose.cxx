// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4VVerbose.cxx
/// \brief Implementation of the TG4VVerbose class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VVerbose.h"

#include <math.h>

// static data members
const G4int TG4VVerbose::fgkDefaultVerboseLevel = 1;

//_____________________________________________________________________________
TG4VVerbose::TG4VVerbose()
  : fVerboseLevel(fgkDefaultVerboseLevel) 
{
/// Default constructor
}
  
//_____________________________________________________________________________
TG4VVerbose::TG4VVerbose(G4int verboseLevel) 
  : fVerboseLevel(verboseLevel) 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4VVerbose::~TG4VVerbose() 
{
/// Destructor
}

