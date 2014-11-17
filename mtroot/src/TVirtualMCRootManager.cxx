//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013, 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TVirtualMCRootManager.cxx
/// \brief Implementation of the TVirtualMCRootManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TVirtualMCRootManager.h"
#include "TError.h"

//
// static data, methods
//

                               Bool_t  TVirtualMCRootManager::fgDebug = false;
TMCThreadLocal TVirtualMCRootManager*  TVirtualMCRootManager::fgInstance = 0;

//_____________________________________________________________________________
TVirtualMCRootManager* TVirtualMCRootManager::Instance()
{
/// \return The singleton instance.

  return fgInstance;
}  

//
// ctors, dtor
//

//_____________________________________________________________________________
TVirtualMCRootManager::TVirtualMCRootManager()
{
/// Default constructor

  if ( fgInstance ) {
      Fatal("TVirtualMCRootManager",
            "Attempt to create two instances of singleton.");
    return;
  }  

  fgInstance = this;
}

//_____________________________________________________________________________
TVirtualMCRootManager::~TVirtualMCRootManager() 
{
/// Destructor

  fgInstance = 0;
}

