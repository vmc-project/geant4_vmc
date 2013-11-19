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
#include "TMCRootMutex.h"
#include "TThread.h"
#include "TError.h"

#include <cstdio>

pthread_mutex_t counter_mutex;
pthread_mutex_t fill_lock_mutex;

//
// static data, methods
//

Int_t    TMCRootManagerMT::fgNofWorkers = 0;
Int_t    TMCRootManagerMT::fgCounter = 0; 
Bool_t   TMCRootManagerMT::fgIsFillLock = true; 
Bool_t*  TMCRootManagerMT::fgIsFillLocks = 0;

//_____________________________________________________________________________
void TMCRootManagerMT::Initialize(Int_t nofWorkers)
{
/// Initialize Root IO for threading
/// Add: protection against calling this twice
 
  if ( fgDebug ) 
    printf("TMCRootManagerMT::Initialize with %d workers\n", nofWorkers);

  // Initialize Root threading
  TThread::Initialize();

  // Initialize the global static data
  fgNofWorkers = nofWorkers;
  fgIsFillLocks = new Bool_t[nofWorkers];
  for (Int_t i=0; i< nofWorkers; ++i ) {
    fgIsFillLocks[i] = true;
  }  

  if ( fgDebug ) 
    printf("Done TMCRootManagerMT::Initialize with %d workers\n", nofWorkers);
}  

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

  // Check if TMCRootManagerMT was initialized
  if ( fgNofWorkers == 0 ) {
     Fatal("TMCRootManagerMT", "NofWorkers must be set first.");
  }    

  if ( fgDebug ) 
    printf("Going to lock for TMCRootManagerMT::TMCRootManagerMT %p \n", this);
  TMCRootMutex::Lock();

  // Set Id
  fId = fgCounter;

  if ( fgDebug ) printf("Going to new TMCRootManagerImpl in %d  %p \n", fId, this);
  fRootManager = new TMCRootManagerImpl(projectName, fileMode, fId);
  if ( fgDebug ) printf("Done new fRootManager in %d  %p \n", fId, this);

  // Increment counter
  pthread_mutex_lock(&counter_mutex);
  ++fgCounter;
  pthread_mutex_unlock(&counter_mutex);

  TMCRootMutex::UnLock();
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
  TMCRootMutex::Lock();

  // Delete Root manager
  if ( fgDebug ) printf("Going to Delete fRootManager in %d  %p \n", fId, this);
  delete fRootManager;
  if ( fgDebug ) printf("Done Delete fRootManager in %d  %p \n", fId, this);

  TMCRootMutex::UnLock();
  if ( fgDebug ) 
    printf("Released lock for Delete fRootManager in %d  %p \n", fId, this);

  // Global cleanup 
  pthread_mutex_lock(&counter_mutex);
  --fgCounter;
  if ( fgCounter == 0 ) {
    delete [] fgIsFillLocks;
  }  
  pthread_mutex_unlock(&counter_mutex);

  if ( fgDebug ) 
    printf("Done TMCRootManagerMT::~TMCRootManagerMT %p \n", this);
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
  TMCRootMutex::Lock();

  if ( fgDebug ) printf("Register %s  in %d  %p \n", name, fId, this);
  fRootManager->Register(name, className, objAddress);
  if ( fgDebug ) printf("Done Register %s  in %d  %p \n", name, fId, this);

  TMCRootMutex::UnLock();
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
/*
  if ( fgIsFillLock ) {
    if ( fgDebug ) printf("Going to lock for Fill in %d  %p \n", fId, this);
    TMCRootMutex::Lock();
  }  
*/
  if ( fgDebug ) printf("Fill in %d  %p \n", fId, this);
  fRootManager->Fill();
  if ( fgDebug ) printf("Done Fill in %d  %p \n", fId, this);
/*  
  if ( fgIsFillLock ) {
    TMCRootMutex::UnLock();
    if ( fgDebug ) printf("Released lock for Fill in %d  %p \n", fId, this);
    // the access to TFile and TTree needs to be locked only until 
    // __after__ the first Fill
    fgIsFillLocks[fId] = false;
    Bool_t isDoneAll = true;
    Int_t counter = 0;
    while ( isDoneAll && counter < fgNofWorkers ) {
      isDoneAll = ! fgIsFillLocks[counter++];
    }
    if ( isDoneAll ) {
      pthread_mutex_lock(&fill_lock_mutex);
      if ( fgDebug ) 
        printf("... Switching off locking of Fill() in %d %p\n", fId, this);
      fgIsFillLock = false;
      pthread_mutex_unlock(&fill_lock_mutex);
      //if ( fgDebug ) printf("... Switching off locking of Fill()\n");
    }  
  }        
  if ( fgDebug ) printf("Exiting Fill in %d  %p \n", fId, this);
*/  
}  

//_____________________________________________________________________________
void TMCRootManagerMT:: WriteAll()
{
/// Write the Root tree in the file.

  if ( fgDebug ) printf("Going to lock for Write in %d  %p \n", fId, this);
  TMCRootMutex::Lock();

  if ( fgDebug ) printf("Write in %d  %p \n", fId, this);
  fRootManager->WriteAll();
  if ( fgDebug ) printf("Done Write in %d  %p \n", fId, this);

  TMCRootMutex::UnLock();
  if ( fgDebug ) printf("Released lock for Write in %d  %p \n", fId, this);
}  

//_____________________________________________________________________________
void TMCRootManagerMT:: Close()
{
/// Close the Root file.

  if ( fgDebug ) printf("Going to lock for Close in %d  %p \n", fId, this);
  TMCRootMutex::Lock();

  if ( fgDebug ) printf("Close in %d  %p \n", fId, this);
  fRootManager->Close();
  if ( fgDebug ) printf("Done Close in %d  %p \n", fId, this);

  TMCRootMutex::UnLock();
  if ( fgDebug ) printf("Released lock for Write in %d  %p \n", fId, this);
}  

//_____________________________________________________________________________
void  TMCRootManagerMT::ReadEvent(Int_t i)
{
/// Read the event data for \em i -th event for all connected branches.
/// \param i  The event to be read

  fRootManager->ReadEvent(i);
}
