// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2013 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4WorkerInitialization.cxx
/// \brief Implementation of the TG4WorkerInitialization class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4WorkerInitialization.h"
#include "TG4RunManager.h"

//_____________________________________________________________________________
TG4WorkerInitialization::TG4WorkerInitialization()
  : G4UserWorkerInitialization()
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4WorkerInitialization::~TG4WorkerInitialization()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4WorkerInitialization::WorkerRunStart() const
{
/// Call post initialization on workers

  G4cout << "TG4WorkerInitialization::WorkerRunStart() " << G4endl;

  TG4RunManager::Instance()->LateInitialize();

  G4cout << "TG4WorkerInitialization::WorkerRunStart() end " << G4endl;
}   
