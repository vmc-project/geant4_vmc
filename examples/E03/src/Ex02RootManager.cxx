// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file E03/src/Ex02RootManager.cxx 
/// \brief Implementation of the Ex02RootManager class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo. \n
/// The implementation of the Ex02RootManager taken from
/// the E02 example.
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex02RootManager.h"

#include <TParticle.h>
#include <TObjArray.h>

/// \cond CLASSIMP
ClassImp(Ex02RootManager)
/// \endcond

Ex02RootManager* Ex02RootManager::fgInstance = 0;

//_____________________________________________________________________________
Ex02RootManager::Ex02RootManager(const char* projectName, FileMode fileMode)
  : TObject()
{
/// Standard constructor
/// \param projectName  The project name (passed as the Root tree name)
/// \param fileMode     Option for opening Root file (read or write mode)

  if (fgInstance) {
    Fatal("Ex02RootManager", "Singleton instance already exists.");
    return;
  }  

  TString fileName(projectName);
  fileName += ".root";

  TString treeTitle(projectName);
  treeTitle += " tree";

  switch (fileMode) {
    case kRead:
      fFile = new TFile(fileName);
      fTree = (TTree*) fFile->Get(projectName);
      break;
      
    case kWrite:  
      fFile = new TFile(fileName, "recreate");
      fTree = new TTree(projectName, treeTitle);
      ;;  
  }
  
  fPath = gDirectory->GetPath();
  fgInstance = this;
}

//_____________________________________________________________________________
Ex02RootManager::Ex02RootManager()
  : TObject(),
    fFile(0),
    fTree(0) 
{
/// Default constructor

  if (fgInstance) {
    Fatal("Ex02RootManager", "Singleton instance already exists.");
    return;
  }  

  fgInstance = this;
}

//_____________________________________________________________________________
Ex02RootManager::~Ex02RootManager() 
{
/// Destructor

  delete  fTree->GetCurrentFile();
  fgInstance = 0;
}

//
// static methods
//

//_____________________________________________________________________________
Ex02RootManager* Ex02RootManager::Instance()
{
/// \return The singleton instance.

  return fgInstance;
}  

//
// public methods
//

//_____________________________________________________________________________
void  Ex02RootManager::Register(const char* name, const char* className, 
                                void* objAddress)
{
/// Create a branch and associates it with the given address.
/// \param name       The branch name
/// \param className  The class name of the object
/// \param objAddress The object address

  if (!fTree->GetBranch(name)) 
    fTree->Branch(name, className, objAddress, 32000, 99);
  else  
    fTree->GetBranch(name)->SetAddress(objAddress);
}

//_____________________________________________________________________________
void  Ex02RootManager::Fill()
{
/// Fill the Root tree.

  fTree->Fill();
}  

//_____________________________________________________________________________
void Ex02RootManager:: WriteAll()
{
/// Write the Root tree in the file.

  TFile* file =  fTree->GetCurrentFile();
  file->Write();
  file->Close();
}  

//_____________________________________________________________________________
void  Ex02RootManager::ReadEvent(Int_t i)
{
/// Read the event data for \em i -th event for all connected branches.
/// \param i  The event to be read

  fTree->GetEntry(i);
}
