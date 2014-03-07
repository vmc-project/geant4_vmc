// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//______________________________________________________________________________
//
// The Root IO manager class for VMC examples for multi-threaded applications.
// It implements the TVirtualMCRootManager interface.
//______________________________________________________________________________

#include "TMCRootManagerMT.h"
#include "TMCRootManagerImpl.h"
#include "TMCAutoLock.h"
#include "TThread.h"
#include "TError.h"

#include <cstdio>

namespace {
  TMCMutex createMutex = TMCMUTEX_INITIALIZER;
  TMCMutex deleteMutex = TMCMUTEX_INITIALIZER;
  TMCMutex registerMutex = TMCMUTEX_INITIALIZER;
  TMCMutex fillMutex  = TMCMUTEX_INITIALIZER;
  TMCMutex tmpFillMutex  = TMCMUTEX_INITIALIZER;
  TMCMutex writeMutex = TMCMUTEX_INITIALIZER;
  TMCMutex closeMutex = TMCMUTEX_INITIALIZER;
}  

pthread_mutex_t counter_mutex;
pthread_mutex_t fill_lock_mutex;

//
// static data, methods
//

Int_t   TMCRootManagerMT::fgCounter = 0; 
Bool_t  TMCRootManagerMT::fgIsFillLock = true; 
std::vector<Bool_t>* TMCRootManagerMT::fgIsFillLocks = 0;

//
// ctors, dtor
//

//_____________________________________________________________________________
TMCRootManagerMT::TMCRootManagerMT(const char* projectName, 
                                   TVirtualMCRootManager::FileMode fileMode)
  : TVirtualMCRootManager(),
    fId(0),
    fRootManager(0)
{
/// Standard constructor
/// \param projectName  The project name (passed as the Root tree name)
/// \param fileMode     Option for opening Root file (read or write mode)

  // Check if TThread was initialized
  //if ( ! TThread::IsInitialized() ) {
  //   Fatal("TMCRootManagerMT", "TThread::Initialize() must be called first.");
  //}

  if ( fgDebug ) 
    printf("Going to lock for TMCRootManagerMT::TMCRootManagerMT %p \n", this);
  TMCAutoLock lk(&createMutex);

  // Set Id
  fId = fgCounter;

  if ( fgDebug ) printf("Going to new TMCRootManagerImpl in %d  %p \n", fId, this);
  fRootManager = new TMCRootManagerImpl(projectName, fileMode, fId);
  if ( fgDebug ) printf("Done new fRootManager in %d  %p \n", fId, this);

  // Increment counter
  //pthread_mutex_lock(&counter_mutex);
  if ( ! fgCounter ) {
    fgIsFillLocks = new std::vector<Bool_t>();
  } 
  ++fgCounter;
  fgIsFillLocks->push_back(true);  
  //pthread_mutex_unlock(&counter_mutex);

  lk.unlock();
  if ( fgDebug ) 
    printf("Released lock for TMCRootManagerMT::TMCRootManagerMT in %d  %p \n", fId, this);

  if ( fgDebug ) 
    printf("Done TMCRootManagerMT::TMCRootManagerMT %p \n", this);

}

//_____________________________________________________________________________
TMCRootManagerMT::~TMCRootManagerMT() 
{
/// Destructor

  if ( fgDebug ) 
    printf("TMCRootManagerMT::~TMCRootManagerMT %d %p \n", fId, this);

  if ( fgDebug ) 
    printf("Going to lock for Delete fRootManager in %d  %p \n", fId, this);
  TMCAutoLock lk(&deleteMutex);

  // Delete Root manager
  if ( fgDebug ) printf("Going to Delete fRootManager in %d  %p \n", fId, this);
  delete fRootManager;
  if ( fgDebug ) printf("Done Delete fRootManager in %d  %p \n", fId, this);

  if ( fgDebug ) 
    printf("Released lock for Delete fRootManager in %d  %p \n", fId, this);

  // Global cleanup 
  //pthread_mutex_lock(&counter_mutex);
  --fgCounter;
  //fgIsFillLocks->pop_back();
    // we do not care of the content of the vector at this stage
    // but we update the size for consistency
  if ( ! fgCounter ) {
    delete fgIsFillLocks;
    fgIsFillLocks = 0;
  }  
  lk.unlock();

  if ( fgDebug ) 
    printf("Done TMCRootManagerMT::~TMCRootManagerMT %p \n", this);
}

//
// private methods
//

//_____________________________________________________________________________
void  TMCRootManagerMT::FillWithTmpLock()
{
/// Fill the Root tree.

  if ( fgDebug ) printf("Going to lock for Fill in %d  %p \n", fId, this);
  TMCAutoLock lk(&tmpFillMutex);

  if ( fgDebug ) printf("Fill in %d  %p \n", fId, this);
  fRootManager->Fill();
  if ( fgDebug ) printf("Done Fill in %d  %p \n", fId, this);
  
  if ( fgIsFillLock ) {
    // the access to TFile and TTree needs to be locked only until 
    // __after__ the first Fill
    (*fgIsFillLocks)[fId] = false;
    Bool_t isDoneAll = true;
    Int_t counter = 0;
    while ( isDoneAll && counter < fgCounter ) {
      isDoneAll = ! (*fgIsFillLocks)[counter++];
    }
    if ( isDoneAll ) {
      if ( fgDebug ) 
        printf("... Switching off locking of Fill() in %d %p\n", fId, this);
      fgIsFillLock = false;
    }
  }        
  if ( fgDebug ) printf("Exiting Fill in %d  %p \n", fId, this);
  lk.unlock();
  if ( fgDebug ) printf("Released lock for Fill in %d  %p \n", fId, this);
}  

//_____________________________________________________________________________
void  TMCRootManagerMT::FillWithLock()
{
/// Fill the Root tree.

  if ( fgDebug ) printf("Going to lock for Fill in %d  %p \n", fId, this);
  TMCAutoLock lk(&fillMutex);

  if ( fgDebug ) printf("Fill in %d  %p \n", fId, this);
  fRootManager->Fill();
  if ( fgDebug ) printf("Done Fill in %d  %p \n", fId, this);
  
  lk.unlock();
  if ( fgDebug ) printf("Released lock for Fill in %d  %p \n", fId, this);
}  

//_____________________________________________________________________________
void  TMCRootManagerMT::FillWithoutLock()
{
  if ( fgDebug ) printf("Fill in %d  %p \n", fId, this);
  fRootManager->Fill();
  if ( fgDebug ) printf("Done Fill in %d  %p \n", fId, this);
}

//
// public methods
//

//_____________________________________________________________________________
void  TMCRootManagerMT::Register(const char* name, const char* className, 
                                void* objAddress)
{
/// Create a branch and associates it with the given address.
/// \param name       The branch name
/// \param className  The class name of the object
/// \param objAddress The object address

  if ( fgDebug ) printf("Going to lock for Register in %d  %p \n", fId, this);
  TMCAutoLock lk(&registerMutex);

  if ( fgDebug ) printf("Register %s  in %d  %p \n", name, fId, this);
  fRootManager->Register(name, className, objAddress);
  if ( fgDebug ) printf("Done Register %s  in %d  %p \n", name, fId, this);

  lk.unlock();
  if ( fgDebug ) printf("Released lock for Register in %d  %p \n", fId, this);
}

//_____________________________________________________________________________
void  TMCRootManagerMT::Register(const char* name, const char* className, 
                                const void* objAddress)
{
/// Create a branch and associates it with the given address.
/// \param name       The branch name
/// \param className  The class name of the object
/// \param objAddress The object address

  Register(name, className, const_cast<void*>(objAddress));
}

//_____________________________________________________________________________
void  TMCRootManagerMT::Fill()
{
/// Fill the Root tree.

  // Fill with lack untill first call on all threads
  if ( fgIsFillLock ) {
    FillWithTmpLock();
  }  
  else {
    FillWithoutLock();
  }

  // Fill with lock during the whole run
  // FillWithLock();
}  

//_____________________________________________________________________________
void TMCRootManagerMT:: WriteAll()
{
/// Write the Root tree in the file.

  if ( fgDebug ) printf("Going to lock for Write in %d  %p \n", fId, this);
  TMCAutoLock lk(&writeMutex);

  if ( fgDebug ) printf("Write in %d  %p \n", fId, this);
  fRootManager->WriteAll();
  if ( fgDebug ) printf("Done Write in %d  %p \n", fId, this);

  lk.unlock();
  if ( fgDebug ) printf("Released lock for Write in %d  %p \n", fId, this);
}  

//_____________________________________________________________________________
void TMCRootManagerMT:: Close()
{
/// Close the Root file.

  if ( fgDebug ) printf("Going to lock for Close in %d  %p \n", fId, this);
  TMCAutoLock lk(&closeMutex);

  if ( fgDebug ) printf("Close in %d  %p \n", fId, this);
  fRootManager->Close();
  if ( fgDebug ) printf("Done Close in %d  %p \n", fId, this);

  lk.unlock();
  if ( fgDebug ) printf("Released lock for Write in %d  %p \n", fId, this);
}  

//_____________________________________________________________________________
void TMCRootManagerMT:: WriteAndClose()
{
/// Write the Root tree in the file and close the file.

  if ( fgDebug ) printf("Going to lock for WriteAndClose in %d  %p \n", fId, this);
  TMCAutoLock lk(&writeMutex);

  if ( fgDebug ) printf("Write in %d  %p \n", fId, this);
  fRootManager->WriteAll();
  if ( fgDebug ) printf("Done Write in %d  %p \n", fId, this);
  if ( fgDebug ) printf("Close in %d  %p \n", fId, this);
  fRootManager->Close();
  if ( fgDebug ) printf("Done Close in %d  %p \n", fId, this);

  lk.unlock();
  if ( fgDebug ) printf("Released lock for WriteAndClose in %d  %p \n", fId, this);
}  

//_____________________________________________________________________________
void  TMCRootManagerMT::ReadEvent(Int_t i)
{
/// Read the event data for \em i -th event for all connected branches.
/// \param i  The event to be read

  fRootManager->ReadEvent(i);
}
