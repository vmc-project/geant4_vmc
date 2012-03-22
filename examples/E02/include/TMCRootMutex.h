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

#ifndef ROOT_TMCRootMutex
#define ROOT_TMCRootMutex

#include <pthread.h>

//
// Class TMCRootMutex
// -------------------
// The class which defines a mutex for TMCRootManagerMT class
// which can be also used directly from VMC application if needed.

class TMCRootMutex
{
  public:
    static void Lock();
    static void UnLock();
    
  private:
    // not implemented
    TMCRootMutex(const TMCRootMutex& rhs);
    TMCRootMutex& operator=(const TMCRootMutex& rhs);

#ifndef __CINT__
    static pthread_mutex_t fgRootIOMutex;
#endif    
};

#endif //ROOT_TMCRootMutex
   

