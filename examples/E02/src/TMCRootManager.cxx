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
// The Root IO manager class for VMC examples for single-threaded applications.
// It implements the TVirtualMCRootManager interface.
//______________________________________________________________________________


#include "TMCRootManager.h"
#include "TMCRootManagerImpl.h"

#include <cstdio>

//
// ctors, dtor
//

//_____________________________________________________________________________
TMCRootManager::TMCRootManager(const char* projectName, 
                               TVirtualMCRootManager::FileMode fileMode)
  : TVirtualMCRootManager(),
    fRootManager(new TMCRootManagerImpl(projectName, fileMode, -1))
{
/// Standard constructor
/// \param projectName  The project name (passed as the Root tree name)
/// \param fileMode     Option for opening Root file (read or write mode)

  if ( fgDebug ) printf("TMCRootManager::TMCRootManager %p \n", this);
}

//_____________________________________________________________________________
TMCRootManager::~TMCRootManager() 
{
/// Destructor

  if ( fgDebug ) printf("TMCRootManager::~TMCRootManager %p \n", this);
  delete fRootManager;
  if ( fgDebug ) printf("Done TMCRootManager::~TMCRootManager %p \n", this);
}

//
// public methods
//

//_____________________________________________________________________________
void  TMCRootManager::Register(const char* name, const char* className, 
                                void* objAddress)
{
/// Create a branch and associates it with the given address.
/// \param name       The branch name
/// \param className  The class name of the object
/// \param objAddress The object address

  fRootManager->Register(name, className, objAddress);
}

//_____________________________________________________________________________
void  TMCRootManager::Fill()
{
/// Fill the Root tree.

  fRootManager->Fill();
}  

//_____________________________________________________________________________
void TMCRootManager:: WriteAll()
{
/// Write the Root tree in the file.

  fRootManager->WriteAll();
}  

//_____________________________________________________________________________
void TMCRootManager::Close()
{
/// Close the Root file.

  fRootManager->Close();
}  

//_____________________________________________________________________________
void  TMCRootManager::ReadEvent(Int_t i)
{
/// Read the event data for \em i -th event for all connected branches.
/// \param i  The event to be read

  fRootManager->ReadEvent(i);
}
