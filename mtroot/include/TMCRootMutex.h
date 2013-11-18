// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

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
   

