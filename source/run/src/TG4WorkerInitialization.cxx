//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4WorkerInitialization.cxx
/// \brief Implementation of the TG4WorkerInitialization class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4WorkerInitialization.h"
#include "TG4RunManager.h"

#include <TVirtualMCApplication.h>
#include <RVersion.h>

#include <G4AutoLock.hh>

#ifdef G4MULTITHREADED
namespace {
  // Mutex to lock MCApplication::FinishRun
  G4Mutex finishRunMutex = G4MUTEX_INITIALIZER;
  // Mutex to lock deleting MC application
  // G4Mutex deleteMutex = G4MUTEX_INITIALIZER;
  // Mutex to lock FinishRun & deleting MC application
  G4Mutex stopWorkerMutex = G4MUTEX_INITIALIZER;
}
#endif

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

  //G4cout << "TG4WorkerInitialization::WorkerRunStart() " << G4endl;

  TG4RunManager::Instance()->LateInitialize();
#ifdef G4MULTITHREADED
  TVirtualMCApplication::Instance()->BeginWorkerRun();  // deprecated
#if ( ROOT_VERSION_CODE >= ROOT_VERSION(6,10,0) )
  TVirtualMCApplication::Instance()->BeginRunOnWorker(); // new
#endif
  //G4cout << "TG4WorkerInitialization::WorkerRunStart() end " << G4endl;
#endif
}   


//_____________________________________________________________________________
void TG4WorkerInitialization::WorkerRunEnd() const
{
// This method is called for each thread, when the local event loop has
// finished but before the synchronization over threads.

  //G4cout << "TG4WorkerInitialization::WorkerRunEnd() " << G4endl;

#ifdef G4MULTITHREADED
  G4AutoLock lm(&finishRunMutex);
  TVirtualMCApplication::Instance()->FinishWorkerRun();  // deprecated
#if ( ROOT_VERSION_CODE >= ROOT_VERSION(6,10,0) )
  TVirtualMCApplication::Instance()->FinishRunOnWorker(); // new
#endif
  lm.unlock();
#endif

  //G4cout << "TG4WorkerInitialization::WorkerRunEnd() end " << G4endl;
}

//_____________________________________________________________________________
void TG4WorkerInitialization::WorkerStop() const
{
/// This method is called once at the end of simulation job.
/// It implements a clean up action, which is the clean-up of MC application
/// in our case.

  //G4cout << "TG4WorkerInitialization::WorkerStop() " << G4endl;

#ifdef G4MULTITHREADED
  G4AutoLock lm(&stopWorkerMutex);
  // TVirtualMCApplication::Instance()->FinishWorkerRun();  // deprecated
  // TVirtualMCApplication::Instance()->FinishRunOnWorker(); // new
  delete TVirtualMCApplication::Instance();
  lm.unlock();
#endif

  //G4cout << "TG4WorkerInitialization::WorkerStop() end " << G4endl;
}
