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
// The common implementation of the TVirtualMCRootManager interface
// for the Root IO managers for VMC examples.
// It is used in TMCRootManager (for single-threaded applications)
// and TMCRootManagerMT (for multi-threaded applications)
//______________________________________________________________________________


#include "TMCRootManagerImpl.h"
#include "TVirtualMCRootManager.h"
#include "TTree.h"
#include "TFile.h"
#include "TError.h"
#include "TThread.h"
#include "Riostream.h"

#include <cstdio>

//
// ctors, dtor
//

//_____________________________________________________________________________
TMCRootManagerImpl::TMCRootManagerImpl(const char* projectName, 
                                       TVirtualMCRootManager::FileMode fileMode,
                                       Int_t threadRank)
  : fFile(0),
    fTree(0),
    fIsClosed(false)
{
/// Standard constructor
/// \param projectName  The project name (passed as the Root tree name)
/// \param fileMode     Option for opening Root file (read or write mode)

  if ( TVirtualMCRootManager::GetDebug() ) 
    printf("TMCRootManagerImpl::TMCRootManagerImpl %p \n", this);

  TString fileName(projectName); 
  if ( threadRank >= 0 ) {
    fileName += "_";  
    fileName += threadRank;
  }  
  fileName += ".root";

  TString treeTitle(projectName);
  treeTitle += " tree";

  switch (fileMode) {
    case TVirtualMCRootManager::kRead:
      fFile = new TFile(fileName);
      fTree = (TTree*) fFile->Get(projectName);
      break;
      
    case TVirtualMCRootManager::kWrite:  
      if ( TVirtualMCRootManager::GetDebug() ) 
        printf("Going to create Root file \n");
      fFile = new TFile(fileName, "recreate");
      if ( TVirtualMCRootManager::GetDebug() ) 
        printf("Done: file %p \n", fFile);

      if ( TVirtualMCRootManager::GetDebug() ) 
        printf("Going to create TTree \n");
      fTree = new TTree(projectName, treeTitle);
      if ( TVirtualMCRootManager::GetDebug() ) 
        printf("Done: TTree %p \n", fTree);
      ;;  
  }

  if ( TVirtualMCRootManager::GetDebug() ) 
    printf("Done TMCRootManagerImpl::TMCRootManagerImpl %p \n", this);
}

//_____________________________________________________________________________
TMCRootManagerImpl::~TMCRootManagerImpl() 
{
/// Destructor

  if ( TVirtualMCRootManager::GetDebug() ) 
    printf("TMCRootManagerImpl::~TMCRootManagerImpl %p \n", this);

  if ( fFile && ! fIsClosed ) fFile->Close();
  delete fFile;

  if ( TVirtualMCRootManager::GetDebug() ) 
    printf("Done TMCRootManagerImpl::~TMCRootManagerImpl %p \n", this);
}

//
// public methods
//

//_____________________________________________________________________________
void  TMCRootManagerImpl::Register(const char* name, const char* className, 
                                void* objAddress)
{
/// Create a branch and associates it with the given address.
/// \param name       The branch name
/// \param className  The class name of the object
/// \param objAddress The object address

  fFile->cd();
  if ( ! fTree->GetBranch(name) ) 
    fTree->Branch(name, className, objAddress, 32000, 99);
  else  
    fTree->GetBranch(name)->SetAddress(objAddress);
}

//_____________________________________________________________________________
void  TMCRootManagerImpl::Register(const char* name, const char* className, 
                                const void* objAddress)
{
/// Create a branch and associates it with the given address.
/// \param name       The branch name
/// \param className  The class name of the object
/// \param objAddress The object address

  Register(name, className, const_cast<void*>(objAddress));
}

//_____________________________________________________________________________
void  TMCRootManagerImpl::Fill()
{
/// Fill the Root tree.

  fFile->cd();
  fTree->Fill();
}  

//_____________________________________________________________________________
void TMCRootManagerImpl:: WriteAll()
{
/// Write the Root tree in the file.

  fFile->cd();
  fFile->Write();
}  

//_____________________________________________________________________________
void TMCRootManagerImpl::Close()
{
/// Close the Root file.

  if ( fIsClosed ) {
    Error("Close", "The file was alerady closed.");
    return;
  }  
    
  fFile->cd();
  fFile->Close();
  fIsClosed = true;
}  

//_____________________________________________________________________________
void  TMCRootManagerImpl::ReadEvent(Int_t i)
{
/// Read the event data for \em i -th event for all connected branches.
/// \param i  The event to be read

  fTree->GetEntry(i);
}
