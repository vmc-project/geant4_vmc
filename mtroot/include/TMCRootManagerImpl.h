// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

#ifndef ROOT_TMCRootManagerImpl
#define ROOT_TMCRootManagerImpl

#include "TVirtualMCRootManager.h"

class TParticle;
class TFile;
class TTree;

//
// Class TMCRootManagerImpl
// ------------------------
// The common implementation of the TVirtualMCRootManager interface
// for the Root IO managers for VMC examples.
// It is used in TMCRootManager (for single-threaded applications)
// and TMCRootManagerMT (for multi-threaded applications)

class TMCRootManagerImpl
{
  public:
    TMCRootManagerImpl(const char* projectName, 
                       TVirtualMCRootManager::FileMode fileMode 
                         = TVirtualMCRootManager::kWrite, 
                       Int_t threadRank = -1);
    virtual ~TMCRootManagerImpl();     
  
    // methods
    void  Register(const char* name, const char* className, void* objAddress);
    void  Register(const char* name, const char* className, const void* objAddress);
    void  Fill();
    void  WriteAll();
    void  Close();
    void  ReadEvent(Int_t i);
    
  private:
    // not implemented
    TMCRootManagerImpl(const TMCRootManagerImpl& rhs);
    TMCRootManagerImpl& operator=(const TMCRootManagerImpl& rhs);
    
    // data members
    TFile*  fFile;       // Root output file
    TTree*  fTree;       // Root output tree 
    Bool_t  fIsClosed;   // Info whether its file was closed
};

#endif //ROOT_TMCRootManagerImpl
