// $Id: Ex02RootManager.cxx,v 1.1.1.1 2002/06/16 15:57:36 hristov Exp $
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
Ex02RootManager::Ex02RootManager(FileMode fileMode)
  : TObject()
{
//
  if (fgInstance) {
    Fatal("Ex02RootManager", "Singleton instance already exists.");
    return;
  }  

  switch (fileMode) {
    case kRead:
      fFile = new TFile("example02.root");
      fTree = (TTree*) fFile->Get("example02");
      break;
      
    case kWrite:  
      fFile = new TFile("example02.root", "recreate");
      fTree = new TTree("example02", "example02 tree");
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
void  Ex02RootManager::Register(const char* name,  void* clonesAddress)
{
// Creates a branch of the given name and associates it with
// the given address.
// ---

  if (!fTree->GetBranch(name)) 
    fTree->Branch(name, clonesAddress, 32000, 99);
  else  
    fTree->GetBranch(name)->SetAddress(clonesAddress);
}

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
void Ex02RootManager:: Write()
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
