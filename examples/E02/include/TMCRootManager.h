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

#ifndef ROOT_TMCRootManager
#define ROOT_TMCRootManager

#include "TVirtualMCRootManager.h"

class TMCRootManagerImpl;

//
// Class TMCRootManager
// --------------------
// The Root IO manager for VMC examples for single-threaded applications.
// It implements the TVirtualMCRootManager interface.

class TMCRootManager : public TVirtualMCRootManager
{
  public:
    TMCRootManager(const char* projectName, 
                   FileMode fileMode = TVirtualMCRootManager::kWrite);
    virtual ~TMCRootManager();     

    // methods
    virtual void  Register(const char* name, const char* className, void* objAddress);
    virtual void  Fill();
    virtual void  Close();
    virtual void  WriteAll();
    virtual void  ReadEvent(Int_t i);
    
  private:
    // not implemented
    TMCRootManager(const TMCRootManager& rhs);
    TMCRootManager& operator=(const TMCRootManager& rhs);

    // data members 
    TMCRootManagerImpl*  fRootManager;  // The Root manager
};

#endif //ROOT_TMCRootManager
