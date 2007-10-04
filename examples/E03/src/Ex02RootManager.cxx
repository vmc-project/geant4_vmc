// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 novice ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex02RootManager
// ------------------
// Class that takes care of Root IO.
//
// by Ivana Hrivnacova, 5.4.2002

#include "Ex02RootManager.h"

#include <TParticle.h>
#include <TObjArray.h>

ClassImp(Ex02RootManager)

Ex02RootManager* Ex02RootManager::fgInstance = 0;

//_____________________________________________________________________________
Ex02RootManager::Ex02RootManager(const char* projectName, FileMode fileMode)
  : TObject()
{
//
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

  fgInstance = this;
}

//_____________________________________________________________________________
Ex02RootManager::Ex02RootManager()
  : TObject(),
    fFile(0),
    fTree(0) 
{
//   

  if (fgInstance) {
    Fatal("Ex02RootManager", "Singleton instance already exists.");
    return;
  }  

  fgInstance = this;
}

//_____________________________________________________________________________
Ex02RootManager::~Ex02RootManager() 
{
//
  delete fFile;
  fgInstance = 0;
}

//
// static methods
//

//_____________________________________________________________________________
Ex02RootManager* Ex02RootManager::Instance()
{
// Returns singleton instance.
// ---

  return fgInstance;
}  

//
// public methods
//

//_____________________________________________________________________________
void  Ex02RootManager::Register(const char* name, const char* className, 
                                void* objAddress)
{
// Creates a branch of the given name and associates it with
// the given address.
// ---

  if (!fTree->GetBranch(name)) 
    fTree->Branch(name, className, objAddress, 32000, 99);
  else  
    fTree->GetBranch(name)->SetAddress(objAddress);
}

//_____________________________________________________________________________
void  Ex02RootManager::Fill()
{
// Fills the tree.
// ---

  fTree->Fill();
}  

//_____________________________________________________________________________
void Ex02RootManager:: WriteAll()
{
// Erites the tree in the file.
// ---

  fTree->Write();
}  

//_____________________________________________________________________________
void  Ex02RootManager::ReadEvent(Int_t i)
{
// Reads the event data for i-th event for all connected branches.
// ---

  fTree->GetEntry(i);
}
