//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013, 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCRootManagerMT.h
/// \brief Definition of the TMCRootManagerMT class
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef ROOT_TMCRootManagerMT
#define ROOT_TMCRootManagerMT

#include "TVirtualMCRootManager.h"

#include <vector>

class TMCRootManagerImpl;

/// \brief The Root IO manager for VMC examples for multi-threaded applications.
///
/// It implements the TVirtualMCRootManager interface.

class TMCRootManagerMT : public TVirtualMCRootManager
{
 public:
  TMCRootManagerMT(const char* projectName, FileMode fileMode = kWrite);
  virtual ~TMCRootManagerMT();

  // methods
  virtual void Register(
    const char* name, const char* className, void* objAddress);
  virtual void Register(
    const char* name, const char* className, const void* objAddress);
  virtual void Fill();
  virtual void WriteAll();
  virtual void Close();
  virtual void WriteAndClose();
  virtual void ReadEvent(Int_t i);

 private:
  // not implemented
  TMCRootManagerMT(const TMCRootManagerMT& rhs);
  TMCRootManagerMT& operator=(const TMCRootManagerMT& rhs);

  // methods
  void FillWithLock();
  void FillWithTmpLock();
  void FillWithoutLock();

  // global static data members
  static Int_t fgCounter;     // The counter of instances
  static Bool_t fgIsFillLock; // The if the Fill should be locked
  static std::vector<Bool_t>*
    fgIsFillLocks; // The info per thread if the Fill should be locked

  // data members
  Int_t fId;                        // This manager ID
  TMCRootManagerImpl* fRootManager; // The Root manager
};

#endif // ROOT_TMCRootManagerMT
