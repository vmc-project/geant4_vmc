//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01EmCalorimeterSD.cxx 
/// \brief Implementation of the A01EmCalorimeterSD class 
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

#include "A01EmCalorimeterSD.h"
#include "A01EmCalorHit.h"

/// \cond CLASSIMP
ClassImp(A01EmCalorimeterSD)
/// \endcond

using namespace std;

const Int_t A01EmCalorimeterSD::fgkNofColumns = 20; 
const Int_t A01EmCalorimeterSD::fgkNofRows = 4; 
 
//_____________________________________________________________________________
A01EmCalorimeterSD::A01EmCalorimeterSD(const char* name)
  : TNamed(name, ""),
    fCalCollection(0),
    fVolId(0),
    fWriteHits(true),
    fVerboseLevel(1)
{
/// Standard constructor.
/// Create hits collection and an empty hit for each layer.
/// \param name      The calorimeter hits collection name

  fCalCollection = new TClonesArray("A01EmCalorHit", fgkNofColumns*fgkNofRows);
  for (Int_t i=0; i<fgkNofColumns*fgkNofRows; i++) 
    new ((*fCalCollection)[i]) A01EmCalorHit();
}

//_____________________________________________________________________________
A01EmCalorimeterSD::A01EmCalorimeterSD(const A01EmCalorimeterSD& origin)
  : TNamed(origin),
    fCalCollection(0),
    fVolId(origin.fVolId),
    fWriteHits(origin.fWriteHits),
    fVerboseLevel(origin.fVerboseLevel)
{
/// Copy constructor (for clonig on worker thread in MT mode).
/// Create hits collection and an empty hit for each layer
/// \param origin  The source object (on master).

  fCalCollection = new TClonesArray("A01EmCalorHit", fgkNofColumns*fgkNofRows);
  for (Int_t i=0; i<fgkNofColumns*fgkNofRows; i++) 
    new ((*fCalCollection)[i]) A01EmCalorHit();
}

//_____________________________________________________________________________
A01EmCalorimeterSD::A01EmCalorimeterSD()
  : TNamed(),
    fCalCollection(0),
    fVolId(0),
    fWriteHits(true),
    fVerboseLevel(1)
{
/// Default constructor
}

//_____________________________________________________________________________
A01EmCalorimeterSD::~A01EmCalorimeterSD()
{
/// Destructor

  if (fCalCollection) fCalCollection->Delete();
  delete fCalCollection;
}

//
// private methods
//

//_____________________________________________________________________________
void  A01EmCalorimeterSD::ResetHits()
{
/// Reset all hits in the hits collection.

  for (Int_t i=0; i<fCalCollection->GetEntriesFast(); i++) 
    GetHit(i)->Reset();
} 

//
// public methods
//

//_____________________________________________________________________________
void A01EmCalorimeterSD::Initialize()
{
/// Register hits collection in the Root manager;
/// set sensitive volumes.
  
  if ( TVirtualMCRootManager::Instance() ) Register();
  
  fVolId = gMC->VolId("cellLogical");
}

//_____________________________________________________________________________
Bool_t A01EmCalorimeterSD::ProcessHits()
{
/// Account energy deposit for each layer in its hit.

  Int_t copyNo;
  Int_t id = gMC->CurrentVolID(copyNo);
  if (id != fVolId ) return false;

  Double_t edep = gMC->Edep();
  if ( edep == 0. ) return false;
  
  Int_t rowNo = copyNo;
  Int_t columnNo;
  gMC->CurrentVolOffID(1, columnNo);
  // VMC adopts Root numbering of divisions starting from 1 
  rowNo--;
  columnNo--;
  Int_t hitID = fgkNofRows*columnNo + rowNo;
  
  A01EmCalorHit* hit = GetHit(hitID);
  if ( ! hit ) {
    std::cerr << "No hit found for layer with copyNo = " << copyNo << endl;
    return false;
  }  
  
  // check if it is the first touch
  if ( hit->GetVolId() < 0 ) {
    // Debug printing (to check hits indexing)
    //cout << "EmCalorimeter: First Add in hit in (column, row): " 
    //     << columnNo << ", " << rowNo << endl;
    //cout << "gMC->CurrentVolName(), gMC->CurrentVolOffName(1): " 
    //     << gMC->CurrentVolName() << ", "  << gMC->CurrentVolOffName(1) << endl;

    // fill volume information
    hit->SetVolId(fVolId);
    // get transformation
    // add later
  }

  //cout << "Adding edep [MeV]" << edep*1e03 << " in copyNo " << copyNo << endl;

  // add energy deposition
  hit->AddEdep(edep);

  return true;
}

//_____________________________________________________________________________
void A01EmCalorimeterSD::EndOfEvent()
{
/// Print hits collection (if verbose) and reset hits afterwards.

  if ( fVerboseLevel > 0 )  PrintTotal();
    
  // Reset hits collection
  ResetHits();  
}

//_____________________________________________________________________________
void A01EmCalorimeterSD::Register()
{
/// Register the hits collection in Root manager.
  
  if ( fWriteHits ) {
    TVirtualMCRootManager::Instance()
      ->Register(GetName(), "TClonesArray", &fCalCollection);
  }    
}

//_____________________________________________________________________________
void A01EmCalorimeterSD::Print(Option_t* /*option*/) const
{
/// Print the hits collection.
  
   Int_t nofHits = fCalCollection->GetEntriesFast();
     
   cout << "\n-------->Hits Collection: in this event: " << endl;
	    
   if ( fVerboseLevel > 1 ) {
     for (Int_t i=0; i<nofHits; i++) (*fCalCollection)[i]->Print(); 
   }           
}

//_____________________________________________________________________________
void A01EmCalorimeterSD::PrintTotal() const
{
/// Print the total values for all layers.
  
  Int_t nofHits = 0;
  Double_t totalEdep = 0.;
  for (Int_t i=0; i<fgkNofColumns*fgkNofRows; ++i) {
    Double_t edep = GetHit(i)->GetEdep();
    if ( edep > 0. ) {
      nofHits++;
      totalEdep += edep;
    }
  }
  cout << GetName() << " has " << nofHits << " hits. Total Edep is "
       << totalEdep*1e03 << " (MeV)" << endl;
}

//_____________________________________________________________________________
A01EmCalorHit* A01EmCalorimeterSD::GetHit(Int_t i) const
{
/// \return   The hit for the specified layer.
/// \param i  The layer number

  return (A01EmCalorHit*)fCalCollection->At(i);
}

