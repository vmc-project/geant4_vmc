//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05SensitiveDetector.cxx 
/// \brief Implementation of the Ex05SensitiveDetector class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex05SensitiveDetector.h"
#include "Ex05Hit.h"

#include <Riostream.h>
#include <TVirtualMC.h>
#include <TVirtualMCRootManager.h>
#include <TLorentzVector.h>
#include <TTree.h>

/// \cond CLASSIMP
ClassImp(Ex05SensitiveDetector)
/// \endcond

using namespace std;

//_____________________________________________________________________________
Ex05SensitiveDetector::Ex05SensitiveDetector(const char* name)
  : TNamed(name, ""),
    fCaloHitsCollection(0),
    fCrystalVolId(),
    fVerboseLevel(1)
{
/// Standard constructor.
/// Create hits collection.
/// \param name      The calorimeter hits collection name
/// \param detector  The detector construction

  fCaloHitsCollection = new TClonesArray("Ex05Hit", 500);
}

//_____________________________________________________________________________
Ex05SensitiveDetector::Ex05SensitiveDetector(const Ex05SensitiveDetector& origin)
  : TNamed(origin),
    fCaloHitsCollection(0),
    fCrystalVolId(),
    fVerboseLevel(origin.fVerboseLevel)
{
/// Copy constructor (for cloning on worker thread in MT mode).
/// Create hits collection.
/// \param origin    The source object (on master).
/// \param detector  The detector construction

  fCaloHitsCollection = new TClonesArray("Ex05Hit", 500);
}

//_____________________________________________________________________________
Ex05SensitiveDetector::Ex05SensitiveDetector()
  : TNamed(),
    fCaloHitsCollection(0),
    fCrystalVolId(),
    fVerboseLevel(0)
{
/// Default constructor
}

//_____________________________________________________________________________
Ex05SensitiveDetector::~Ex05SensitiveDetector()
{
/// Destructor

  if (fCaloHitsCollection) fCaloHitsCollection->Delete();
  delete fCaloHitsCollection;
}

//
// private methods
//

//_____________________________________________________________________________
Ex05Hit* Ex05SensitiveDetector::GetHit(Int_t i) const
{
/// \return   The hit for the specified layer.
/// \param i  The layer number

  return (Ex05Hit*)fCaloHitsCollection->At(i);
}

//
// public methods
//

//_____________________________________________________________________________
void Ex05SensitiveDetector::Initialize()
{
/// Register hits collection in the Root manager;
/// set sensitive volumes.
  
  if ( TVirtualMCRootManager::Instance() ) Register();  
  fCrystalVolId = gMC->VolId("Crystal_log");
}

//_____________________________________________________________________________
Bool_t Ex05SensitiveDetector::ProcessHits()
{
/// Account energy deposit and track lengths for each layer in its hit.

  Int_t copyNo;
  Int_t id = gMC->CurrentVolID(copyNo);

  if (id != fCrystalVolId ) return false;

  //cout << "In crystal " << copyNo << endl; 

  Double_t edep = gMC->Edep();
  // cout << "  edep [keV]" << edep*1.e06 << endl; 
  Double_t xpos;
  Double_t ypos;
  Double_t zpos;
  gMC->TrackPosition(xpos, ypos, zpos);

  Int_t next = fCaloHitsCollection->GetEntriesFast();
  Ex05Hit* caloHit = new ((*fCaloHitsCollection)[next]) Ex05Hit();  
  caloHit->SetEdep(edep);
  caloHit->SetPos(TVector3(xpos, ypos, zpos));
  caloHit->SetCrystalNum(copyNo);
  
  return true;
}

//_____________________________________________________________________________
void Ex05SensitiveDetector::EndOfEvent()
{
/// Print hits collection (if verbose) and reset hits afterwards.

  if (fVerboseLevel>1)  Print();
    
  // Reset hits collection
  fCaloHitsCollection->Clear();  
}

//_____________________________________________________________________________
void Ex05SensitiveDetector::Register()
{
/// Register the hits collection in Root manager.
  
  TVirtualMCRootManager::Instance()
    ->Register("hits", "TClonesArray", &fCaloHitsCollection);
}

//_____________________________________________________________________________
void Ex05SensitiveDetector::Print(Option_t* /*option*/) const
{
/// Print the hits collection.
  
   Int_t nofHits = fCaloHitsCollection->GetEntriesFast();
     
   cout << "\n-------->Hits Collection: in this event: " << endl;
	    
   for (Int_t i=0; i<nofHits; i++) (*fCaloHitsCollection)[i]->Print();          
}

//_____________________________________________________________________________
void Ex05SensitiveDetector::PrintTotal() const
{
/// Print the total values for all layers.
  
  Double_t totEdep = 0.;
  
  Int_t nofHits = fCaloHitsCollection->GetEntriesFast();
  for (Int_t i=0; i<nofHits; i++) {
    totEdep += GetHit(i)->GetEdep(); 
  }

  cout << "   Calorimeter: total energy (MeV): " 
       << setw(7) << totEdep * 1.0e03 << endl;
}
