// @(#)root/vmc:$Name:  $:$Id$
// Author: Ivana Hrivnacova, IPN Orsay 17/02/2012

/*************************************************************************
 * Copyright (C) 2006, Rene Brun and Fons Rademakers.                    *
 * Copyright (C) 2012, Ivana Hrivnacova.                         *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

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
