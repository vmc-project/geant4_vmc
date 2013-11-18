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
// The class which defines a mutex for TMCRootManagerMT class
// which can be also used directly from VMC application if needed.
//______________________________________________________________________________


#include "TMCRootMutex.h"
#include "TVirtualMCRootManager.h"

#include <cstdio>

//
// static data, methods
//

pthread_mutex_t TMCRootMutex::fgRootIOMutex;

//_____________________________________________________________________________
void TMCRootMutex::Lock()
{
  if ( TVirtualMCRootManager::GetDebug() ) 
    printf("$$$$ Locking RootIOMutex \n");

  pthread_mutex_lock(&fgRootIOMutex);
}  


//_____________________________________________________________________________
void TMCRootMutex::UnLock() 
{
  if ( TVirtualMCRootManager::GetDebug() ) 
    printf("$$$$ Unlocking RootIOMutex \n");

  pthread_mutex_unlock(&fgRootIOMutex);
}    
