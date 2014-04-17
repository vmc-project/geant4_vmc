// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2012 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file A01DriftChamberSD.cxx 
/// \brief Implementation of the A01DriftChamberSD class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo \n
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>
#include <TVirtualMC.h>
#include <TVirtualMCRootManager.h>
#include <TLorentzVector.h>
#include <TTree.h>

#include "A01DriftChamberSD.h"
#include "A01DriftChamberHit.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(A01DriftChamberSD)
/// \endcond

using namespace std;

//_____________________________________________________________________________
A01DriftChamberSD::A01DriftChamberSD(const char* name, const char* volName)
  : TNamed(name, ""),
    fHitsCollection(0),
    fVolName(volName),
    fVolId(0),
    fWriteHits(true),
    fVerboseLevel(1)
{
/// Standard constructor.
/// Creates hits collection.
/// \param name      The calorimeter hits collection name
/// \param volName   The sensitive volume name

  fHitsCollection = new TClonesArray("A01DriftChamberHit", 500);
}

//_____________________________________________________________________________
A01DriftChamberSD::A01DriftChamberSD(const A01DriftChamberSD& origin)
  : TNamed(origin),
    fHitsCollection(0),
    fVolName(origin.fVolName),
    fVolId(origin.fVolId),
    fWriteHits(origin.fWriteHits),
    fVerboseLevel(origin.fVerboseLevel)
{
/// Copy constructor (for clonig on worker thread in MT mode).
/// Creates hits collection.
/// \param origin  The source object (on master).

  fHitsCollection = new TClonesArray("A01DriftChamberHit", 500);
}

//_____________________________________________________________________________
A01DriftChamberSD::A01DriftChamberSD()
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
A01DriftChamberSD::~A01DriftChamberSD()
{
/// Destructor

  if (fHitsCollection) fHitsCollection->Delete();
  delete fHitsCollection;
}

//
// private methods
//

//_____________________________________________________________________________
A01DriftChamberHit* A01DriftChamberSD::GetHit(Int_t i) const
{
/// \return   The hit for the specified layer.
/// \param i  The layer number

  return (A01DriftChamberHit*)fHitsCollection->At(i);
}

//
// public methods
//

//_____________________________________________________________________________
void A01DriftChamberSD::Initialize()
{
/// Register hits collection in the Root manager;
/// set sensitive volumes.
  
  if ( TVirtualMCRootManager::Instance() ) Register();
  
  fVolId = gMC->VolId(fVolName.Data());
}

//_____________________________________________________________________________
Bool_t A01DriftChamberSD::ProcessHits()
{
/// Account the hit time, local and global position for each layer in its hit.

  Int_t copyNo;
  Int_t id = gMC->CurrentVolID(copyNo);
  if (id != fVolId ) return false;

  Double_t charge = gMC->TrackCharge();
  if ( charge == 0. ) return false;
  
  if ( ! gMC->IsTrackEntering() ) return false;
  
  // get copyNo in mother
  gMC->CurrentVolOffID(1,copyNo);

  // global and local hit position
  Double_t globalPos[3];
  Double_t localPos[3];
  gMC->TrackPosition(globalPos[0], globalPos[1], globalPos[2]);
  gMC->Gmtod(globalPos, localPos, 1);
  
  // Debug printing
  //cout << "** Drift Chamber: Create hit in DriftChamber copyNo, worldPos[cm] " 
  //     << copyNo << ", " 
  //     << globalPos[0] << ", " << globalPos[1] << ", " << globalPos[2] << endl;
  //cout << "gMC->CurrentVolOffName(1): " << gMC->CurrentVolOffName(1) << endl;
  //cout << "localPos[cm] "     
  //     << localPos[0] << ", " << localPos[1] << ", " << localPos[2] << endl;

  Int_t nofHits = fHitsCollection->GetEntriesFast();
  A01DriftChamberHit* hit 
    = new ((*fHitsCollection)[nofHits]) A01DriftChamberHit(copyNo);

  hit->SetWorldPos(TVector3(globalPos[0], globalPos[1], globalPos[2]));
  hit->SetLocalPos(TVector3(localPos[0], localPos[1], localPos[2]));
  hit->SetTime(gMC->TrackTime());

  return true;
}

//_____________________________________________________________________________
void A01DriftChamberSD::EndOfEvent()
{
/// Print hits collection (if verbose) and reset hits afterwards.

  if ( fVerboseLevel > 0 )  Print();
    
  // Reset hits collection
  fHitsCollection->Clear();  
}

//_____________________________________________________________________________
void A01DriftChamberSD::Register()
{
/// Register the hits collection in Root manager.
  
  if ( fWriteHits ) {
    TVirtualMCRootManager::Instance()
      ->Register(GetName(), "TClonesArray", &fHitsCollection);
  }    
}

//_____________________________________________________________________________
void A01DriftChamberSD::Print(Option_t* /*option*/) const
{
/// Print the hits collection.
  
   Int_t nofHits = fHitsCollection->GetEntriesFast();
   cout << GetName() << " has " << nofHits << " hits." << endl;
   if ( fVerboseLevel > 1 ) {
     for (Int_t i2=0; i2<5; ++i2) {
       for (Int_t i=0; i<nofHits; ++i) {
         A01DriftChamberHit* hit = GetHit(i);
         if (hit->GetLayerID()==i2) hit->Print();
       }
     }
   }      
}
