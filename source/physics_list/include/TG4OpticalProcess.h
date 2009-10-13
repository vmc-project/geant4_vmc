#ifndef TG4_OPTICAL_PROCESS_H
#define TG4_OPTICAL_PROCESS_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4OpticalProcess.h
/// \brief Definition of the enumeration TG4OpticalProcess
///
/// \author I. Hrivnacova; IPN, Orsay

/// \ingroup physics_list
/// \brief Enumeration for processes defined in TG4OpticalPhysics
///
/// The enumeration constants are used as the indices of instatianted 
/// processes in the vectors of processes, their activations and
/// verbose levels.

#include <globals.hh>

enum TG4OpticalProcess { 
  kCerenkov,      ///< Cerenkov process index
  kScintillation, ///< Scintillation process index
  kAbsorption,    ///< Absorption process index
  kRayleigh,      ///< RayleighScattering process index
  kBoundary,      ///< Boundary process index
  kNoProcess      ///< Number of processes, no selected process
};

/// Return name for given optical process
G4String TG4OpticalProcessName(G4int processNumber);

#endif //TG4_OPTICAL_PROCESS_H
