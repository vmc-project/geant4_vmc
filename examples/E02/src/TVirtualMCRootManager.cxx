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
// The abstract base class for Root IO managers for VMC examples 
// for both single-threaded and  multi-threaded applications.
//______________________________________________________________________________


#include "TVirtualMCRootManager.h"
#include "TError.h"

//
// static data, methods
//

                         Bool_t  TVirtualMCRootManager::fgDebug = false;
__thread TVirtualMCRootManager*  TVirtualMCRootManager::fgInstance = 0;

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

