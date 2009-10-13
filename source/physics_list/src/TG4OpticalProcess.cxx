// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4OpticalProcess.cxx
/// \brief Definition of the enumeration TG4OpticalProcess
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4OpticalProcess.h"

G4String TG4OpticalProcessName(G4int processNumber)
{
  switch ( processNumber ) {
    case kCerenkov:      return "Cerenkov";
    case kScintillation: return "Scintillation";
    case kAbsorption:    return "Absorption"; 
    case kRayleigh:      return "Rayleigh";   
    case kBoundary:      return "Boundary";    
    default:             return "NoProcess";
  }      
}    
