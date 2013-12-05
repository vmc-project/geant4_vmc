// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

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
    virtual void  Register(const char* name, const char* className, const void* objAddress);
    virtual void  Fill();
    virtual void  Close();
    virtual void  WriteAll();
    virtual void  WriteAndClose();
    virtual void  ReadEvent(Int_t i);
    
  private:
    // not implemented
    TMCRootManager(const TMCRootManager& rhs);
    TMCRootManager& operator=(const TMCRootManager& rhs);

    // data members 
    TMCRootManagerImpl*  fRootManager;  // The Root manager
};

#endif //ROOT_TMCRootManager
