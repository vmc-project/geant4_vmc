//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01HodoscopeSD.cxx
/// \brief Implementation of the A01HodoscopeSD class
///
/// Geant4 example A01 adapted to Virtual Monte Carlo \n
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>
#include <TLorentzVector.h>
#include <TMCRootManager.h>
#include <TTree.h>
#include <TVirtualMC.h>

#include "A01HodoscopeHit.h"
#include "A01HodoscopeSD.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(A01HodoscopeSD)
  /// \endcond

  using namespace std;

//_____________________________________________________________________________
A01HodoscopeSD::A01HodoscopeSD(const char* name, const char* volName)
  : TNamed(name, ""),
    fHitsCollection(0),
    fVolName(volName),
    fVolId(0),
    fWriteHits(true),
    fVerboseLevel(1)
{
  /// Standard constructor.
  /// Create hits collection and an empty hit for each layer.
  /// \param name      The calorimeter hits collection name
  /// \param volName   The sensitive volume name

  // Create hits collection and an empty hit for each layer
  fHitsCollection = new TClonesArray("A01HodoscopeHit", 500);
  // cout << "Hodoscope nofHits: " << fHitsCollection->GetEntriesFast() << endl;
}

//_____________________________________________________________________________
A01HodoscopeSD::A01HodoscopeSD(const A01HodoscopeSD& origin)
  : TNamed(origin),
    fHitsCollection(0),
    fVolName(origin.fVolName),
    fVolId(origin.fVolId),
    fWriteHits(origin.fWriteHits),
    fVerboseLevel(origin.fVerboseLevel)
{
  /// Copy constructor (for clonig on worker thread in MT mode).
  /// Create hits collection and an empty hit for each layer.
  /// \param origin  The source object (on master).

  fHitsCollection = new TClonesArray("A01HodoscopeHit", 500);
  // cout << "Hodoscope nofHits: " << fHitsCollection->GetEntriesFast() << endl;
}

//_____________________________________________________________________________
A01HodoscopeSD::A01HodoscopeSD()
  : TNamed(),
    fHitsCollection(0),
    fVolName(),
    fVolId(0),
    fWriteHits(true),
    fVerboseLevel(1)
{
  /// Default constructor
}

//_____________________________________________________________________________
A01HodoscopeSD::~A01HodoscopeSD()
{
  /// Destructor

  if (fHitsCollection) fHitsCollection->Delete();
  delete fHitsCollection;
}

//
// private methods
//

//_____________________________________________________________________________
A01HodoscopeHit* A01HodoscopeSD::GetHit(Int_t i) const
{
  /// \return   The hit for the specified layer.
  /// \param i  The layer number

  return (A01HodoscopeHit*)fHitsCollection->At(i);
}

//
// public methods
//

//_____________________________________________________________________________
void A01HodoscopeSD::Initialize()
{
  /// Register hits collection in the Root manager;
  /// set sensitive volumes.

  if (TMCRootManager::Instance()) Register();

  fVolId = gMC->VolId(fVolName.Data());
}

//_____________________________________________________________________________
Bool_t A01HodoscopeSD::ProcessHits()
{
  /// Account hit time; create a new hit per detector cell if it does not yet
  /// exist

  Int_t copyNo;
  Int_t id = gMC->CurrentVolID(copyNo);
  if (id != fVolId) return false;

  Double_t edep = gMC->Edep();
  if (edep == 0.) return false;

  Double_t hitTime = gMC->TrackTime();

  // check if this finger already has a hit
  Int_t ix = -1;
  Int_t nofHits = fHitsCollection->GetEntriesFast();
  for (Int_t i = 0; i < nofHits; i++) {
    A01HodoscopeHit* hit = GetHit(i);
    if (hit->GetID() == copyNo) {
      ix = i;
      break;
    }
  }

  if (ix >= 0) {
    // if it has, then take the earlier time
    A01HodoscopeHit* hit = GetHit(ix);
    if (hit->GetTime() > hitTime) {
      hit->SetTime(hitTime);
    }
  }
  else {
    // Debug printing
    // cout << "** Hodoscope: Create hit in nofHits, copyNo, hitTime[s] "
    //     << nofHits << ", " << copyNo << ", " << hitTime << endl;
    // cout << "gMC->CurrentVolName(): " << gMC->CurrentVolName() << endl;

    // if not, create a new hit and set it to the collection
    A01HodoscopeHit* hit =
      new ((*fHitsCollection)[nofHits]) A01HodoscopeHit(copyNo, hitTime);
    hit->SetVolId(id);

    // get transformation
    // add later
  }

  return true;
}

//_____________________________________________________________________________
void A01HodoscopeSD::EndOfEvent()
{
  /// Print hits collection (if verbose) and reset hits afterwards.

  if (fVerboseLevel > 0) Print();

  // Reset hits collection
  fHitsCollection->Clear();
}

//_____________________________________________________________________________
void A01HodoscopeSD::Register()
{
  /// Register the hits collection in Root manager.

  if (fWriteHits) {
    TMCRootManager::Instance()->Register(
      GetName(), "TClonesArray", &fHitsCollection);
  }
}

//_____________________________________________________________________________
void A01HodoscopeSD::Print(Option_t* /*option*/) const
{
  /// Print the hits collection.

  Int_t nofHits = fHitsCollection->GetEntriesFast();
  cout << GetName() << " has " << nofHits << " hits." << endl;

  if (fVerboseLevel > 1) {
    for (Int_t i = 0; i < nofHits; i++) (*fHitsCollection)[i]->Print();
  }
}

/*
//_____________________________________________________________________________
void A01HodoscopeSD::PrintTotal() const
{
/// Print the total values for all layers.

  Double_t totEAbs=0.;
  Double_t totLAbs=0.;
  Double_t totEGap=0.;
  Double_t totLGap=0.;

  Int_t nofHits = fHitsCollection->GetEntriesFast();
  for (Int_t i=0; i<nofHits; i++) {
    totEAbs += GetHit(i)->GetEdepAbs();
    totLAbs += GetHit(i)->GetTrakAbs();
    totEGap += GetHit(i)->GetEdepGap();
    totLGap += GetHit(i)->GetTrakGap();
  }

  cout << "   Absorber: total energy (MeV): "
       << setw(7) << totEAbs * 1.0e03
       << "       total track length (cm):  "
       << setw(7) << totLAbs
       << endl
       << "   Gap:      total energy (MeV): "
       << setw(7) << totEGap * 1.0e03
       << "       total track length (cm):  "
       << setw(7) << totLGap
       << endl;
}
*/
