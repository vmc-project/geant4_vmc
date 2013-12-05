// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

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
void  TMCRootManager::Register(const char* name, const char* className, 
                                const void* objAddress)
{
/// Create a branch and associates it with the given address.
/// \param name       The branch name
/// \param className  The class name of the object
/// \param objAddress The object address

  Register(name, className, const_cast<void*>(objAddress));
}

//_____________________________________________________________________________
void  TMCRootManager::Fill()
{
/// Fill the Root tree.

  fRootManager->Fill();
}  

//_____________________________________________________________________________
void TMCRootManager::WriteAll()
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
void TMCRootManager::WriteAndClose()
{
/// Write the Root tree in the file and close the file.

  WriteAll();
  Close();
}  

//_____________________________________________________________________________
void  TMCRootManager::ReadEvent(Int_t i)
{
/// Read the event data for \em i -th event for all connected branches.
/// \param i  The event to be read

  fRootManager->ReadEvent(i);
}
