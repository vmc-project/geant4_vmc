// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

#ifndef ROOT_TVirtualMCRootManager
#define ROOT_TVirtualMCRootManager

#include <Rtypes.h>
#include <TString.h>
#include <TObject.h>

#include "TVirtualMCRootManager.h"

#include <pthread.h>

class TVirtualMCRootManager;

//
// Class TVirtualRootManager
// -------------------------
// The abstract base class for Root IO manager for VMC examples 
// for both single-threaded and  multi-threaded applications.

class TVirtualMCRootManager
{
  public:
    /// Root file mode 
    enum FileMode { 
      kRead,   // Read mode 
      kWrite   // Write mode
    };

  public:
    TVirtualMCRootManager();
    virtual ~TVirtualMCRootManager();     
  
    // static access method
    static TVirtualMCRootManager* Instance(); 
    
    // static method for activating debug mode
    static void SetDebug(Bool_t debug); 
    static Bool_t GetDebug();

    // methods
    virtual void  Register(const char* name, const char* className, void* objAddress) = 0;
    virtual void  Register(const char* name, const char* className, const void* objAddress) = 0;
    virtual void  Fill() = 0;
    virtual void  WriteAll() = 0;
    virtual void  Close() = 0;
    virtual void  WriteAndClose() = 0;
    virtual void  ReadEvent(Int_t i) = 0;
    
  protected:
    // static data members
    static  Bool_t  fgDebug; // Option to activate debug printings

  private:
    // not implemented
    TVirtualMCRootManager(const TVirtualMCRootManager& rhs);
    TVirtualMCRootManager& operator=(const TVirtualMCRootManager& rhs);
#if defined(__linux__) && !defined(__CINT__)
    static  __thread TVirtualMCRootManager* fgInstance; // singleton instance
#else
    static           TVirtualMCRootManager* fgInstance; // singleton instance
#endif 
};

// inline functions

inline void TVirtualMCRootManager::SetDebug(Bool_t debug) {
  fgDebug = debug;
}  
  
inline Bool_t TVirtualMCRootManager::GetDebug() {
  return fgDebug;
}  

#endif //ROOT_TVirtualMCRootManager   
   

