//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Gflash/src/SensitiveDetector.cxx
/// \brief Implementation of the Gflash::SensitiveDetector class
///
/// Geant4 gflash example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "SensitiveDetector.h"
#include "Hit.h"

#include <Riostream.h>
#include <TVirtualMC.h>
#include <TMCRootManager.h>
#include <TLorentzVector.h>
#include <TTree.h>

/// \cond CLASSIMP
ClassImp(VMC::Gflash::SensitiveDetector)
/// \endcond

using namespace std;

namespace VMC
{
namespace Gflash
{

//_____________________________________________________________________________
SensitiveDetector::SensitiveDetector(const char* name)
  : TNamed(name, ""),
    fCaloHitsCollection(0),
    fCrystalVolId(),
    fVerboseLevel(1)
{
/// Standard constructor.
/// Create hits collection.
/// \param name      The calorimeter name

  fCaloHitsCollection = new TClonesArray("VMC::Gflash::Hit", 500);
}

//_____________________________________________________________________________
SensitiveDetector::SensitiveDetector(const SensitiveDetector& origin)
  : TNamed(origin),
    fCaloHitsCollection(0),
    fCrystalVolId(),
    fVerboseLevel(origin.fVerboseLevel)
{
/// Copy constructor (for cloning on worker thread in MT mode).
/// Create hits collection.
/// \param origin    The source object (on master).

  fCaloHitsCollection = new TClonesArray("Gflash::Hit");
}

//_____________________________________________________________________________
SensitiveDetector::SensitiveDetector()
  : TNamed(),
    fCaloHitsCollection(0),
    fCrystalVolId(),
    fVerboseLevel(0)
{
/// Default constructor
}

//_____________________________________________________________________________
SensitiveDetector::~SensitiveDetector()
{
/// Destructor

  if (fCaloHitsCollection) fCaloHitsCollection->Delete();
  delete fCaloHitsCollection;
}

//
// private methods
//

//_____________________________________________________________________________
Hit* SensitiveDetector::GetHit(Int_t i) const
{
/// \return   The hit for the specified layer.
/// \param i  The layer number

  return (Hit*)fCaloHitsCollection->At(i);
}

//
// public methods
//

//_____________________________________________________________________________
void SensitiveDetector::Initialize()
{
/// Register hits collection in the Root manager;
/// set sensitive volumes.

  if ( TMCRootManager::Instance() ) Register();
  fCrystalVolId = gMC->VolId("Crystal_log");
}

//_____________________________________________________________________________
Bool_t SensitiveDetector::ProcessHits()
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
  Hit* caloHit = new ((*fCaloHitsCollection)[next]) Hit();
  caloHit->SetEdep(edep);
  caloHit->SetPos(TVector3(xpos, ypos, zpos));
  caloHit->SetCrystalNum(copyNo);

  return true;
}

//_____________________________________________________________________________
void SensitiveDetector::EndOfEvent()
{
/// Print hits collection (if verbose) and reset hits afterwards.

  if (fVerboseLevel>1)  Print();

  // Reset hits collection
  fCaloHitsCollection->Clear();
}

//_____________________________________________________________________________
void SensitiveDetector::Register()
{
/// Register the hits collection in Root manager.

  TMCRootManager::Instance()
    ->Register("hits", "TClonesArray", &fCaloHitsCollection);
}

//_____________________________________________________________________________
void SensitiveDetector::Print(Option_t* /*option*/) const
{
/// Print the hits collection.

   Int_t nofHits = fCaloHitsCollection->GetEntriesFast();

   cout << "\n-------->Hits Collection: in this event: " << endl;

   for (Int_t i=0; i<nofHits; i++) (*fCaloHitsCollection)[i]->Print();
}

//_____________________________________________________________________________
void SensitiveDetector::PrintTotal() const
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

}
}
