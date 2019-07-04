//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01HadCalorimeterSD.cxx
/// \brief Implementation of the A01HadCalorimeterSD class
///
/// Geant4 example A01 adapted to Virtual Monte Carlo \n
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <Riostream.h>
#include <TLorentzVector.h>
#include <TTree.h>
#include <TVirtualMC.h>
#include <TVirtualMCRootManager.h>

#include "A01HadCalorHit.h"
#include "A01HadCalorimeterSD.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(A01HadCalorimeterSD)
  /// \endcond

  using namespace std;

const Int_t A01HadCalorimeterSD::fgkNofColumns = 10;
const Int_t A01HadCalorimeterSD::fgkNofRows = 2;

//_____________________________________________________________________________
A01HadCalorimeterSD::A01HadCalorimeterSD(const char* name)
  : TNamed(name, ""),
    fCalCollection(0),
    fVolId(0),
    fWriteHits(true),
    fVerboseLevel(1)
{
  /// Standard constructor.
  /// Create hits collection and an empty hit for each layer.
  /// \param name  The calorimeter hits collection name

  fCalCollection =
    new TClonesArray("A01HadCalorHit", fgkNofColumns * fgkNofRows);
  Int_t counter = 0;
  for (Int_t iColumn = 0; iColumn < fgkNofColumns; ++iColumn) {
    for (Int_t iRow = 0; iRow < fgkNofRows; ++iRow) {
      new ((*fCalCollection)[counter++]) A01HadCalorHit();
    }
  }
}

//_____________________________________________________________________________
A01HadCalorimeterSD::A01HadCalorimeterSD(const A01HadCalorimeterSD& origin)
  : TNamed(origin),
    fCalCollection(0),
    fVolId(origin.fVolId),
    fWriteHits(origin.fWriteHits),
    fVerboseLevel(origin.fVerboseLevel)
{
  /// Copy constructor (for clonig on worker thread in MT mode).
  /// Create hits collection and an empty hit for each layer.
  /// \param origin  The source object (on master).

  fCalCollection =
    new TClonesArray("A01HadCalorHit", fgkNofColumns * fgkNofRows);
  Int_t counter = 0;
  for (Int_t iColumn = 0; iColumn < fgkNofColumns; ++iColumn) {
    for (Int_t iRow = 0; iRow < fgkNofRows; ++iRow) {
      new ((*fCalCollection)[counter++]) A01HadCalorHit();
    }
  }
}

//_____________________________________________________________________________
A01HadCalorimeterSD::A01HadCalorimeterSD()
  : TNamed(), fCalCollection(0), fVolId(0), fWriteHits(true), fVerboseLevel(1)
{
  /// Default constructor
}

//_____________________________________________________________________________
A01HadCalorimeterSD::~A01HadCalorimeterSD()
{
  /// Destructor

  if (fCalCollection) fCalCollection->Delete();
  delete fCalCollection;
}

//
// private methods
//

//_____________________________________________________________________________
A01HadCalorHit* A01HadCalorimeterSD::GetHit(Int_t i) const
{
  /// \return   The hit for the specified layer.
  /// \param i  The layer number

  return (A01HadCalorHit*)fCalCollection->At(i);
}

//_____________________________________________________________________________
void A01HadCalorimeterSD::ResetHits()
{
  /// Reset all hits in the hits collection.

  for (Int_t i = 0; i < fCalCollection->GetEntriesFast(); i++)
    GetHit(i)->Reset();
}

//
// public methods
//

//_____________________________________________________________________________
void A01HadCalorimeterSD::Initialize()
{
  /// Register hits collection in the Root manager;
  /// set sensitive volumes.

  if (TVirtualMCRootManager::Instance()) Register();

  fVolId = gMC->VolId("HadCalScintiLogical");
}

//_____________________________________________________________________________
Bool_t A01HadCalorimeterSD::ProcessHits()
{
  /// Account energy deposit for each layer in its hit.

  Int_t copyNo;
  Int_t id = gMC->CurrentVolID(copyNo);
  if (id != fVolId) return false;

  Double_t edep = gMC->Edep();
  if (edep == 0.) return false;

  Int_t rowNo;
  gMC->CurrentVolOffID(2, rowNo);
  Int_t columnNo;
  gMC->CurrentVolOffID(3, columnNo);
  // VMC adopts Root numbering of divisions starting from 1
  rowNo--;
  columnNo--;
  Int_t hitID = fgkNofRows * columnNo + rowNo;

  A01HadCalorHit* hit = GetHit(hitID);
  if (!hit) {
    std::cerr << "No hit found for layer with "
              << "rowNo = " << rowNo << " columnNo = " << columnNo << endl;
    return false;
  }

  // check if it is the first touch
  if (hit->GetColumnID() < 0) {
    // Debug printing (to check hits indexing)
    // cout << "HadCalorimeter: First Add in hit in (column, row): "
    //     << columnNo << ", " << rowNo << endl;
    // cout << "gMC->CurrentVolOffName(2), gMC->CurrentVolOffName(3): "
    //     << gMC->CurrentVolOffName(2) << ", "  << gMC->CurrentVolOffName(3) <<
    //     endl;

    // fill volume information
    hit->SetRowID(rowNo);
    hit->SetColumnID(columnNo);

    // get transformation
    // add later
  }

  // add energy deposition
  hit->AddEdep(edep);

  return true;
}

//_____________________________________________________________________________
void A01HadCalorimeterSD::EndOfEvent()
{
  /// Print hits collection (if verbose) and reset hits afterwards.

  if (fVerboseLevel > 0) PrintTotal();

  // Reset hits collection
  ResetHits();
}

//_____________________________________________________________________________
void A01HadCalorimeterSD::Register()
{
  /// Register the hits collection in Root manager.

  if (fWriteHits) {
    TVirtualMCRootManager::Instance()->Register(
      GetName(), "TClonesArray", &fCalCollection);
  }
}

//_____________________________________________________________________________
void A01HadCalorimeterSD::Print(Option_t* /*option*/) const
{
  /// Print the hits collection.

  Int_t nofHits = fCalCollection->GetEntriesFast();

  cout << "\n-------->Hits Collection: in this event: " << endl;

  if (fVerboseLevel > 1) {
    for (Int_t i = 0; i < nofHits; i++) (*fCalCollection)[i]->Print();
  }
}

//_____________________________________________________________________________
void A01HadCalorimeterSD::PrintTotal() const
{
  /// Print the total values for all layers.

  Int_t nofHits = 0;
  Double_t totalEdep = 0.;
  for (Int_t i = 0; i < fgkNofColumns * fgkNofRows; ++i) {
    Double_t edep = GetHit(i)->GetEdep();
    if (edep > 0.) {
      nofHits++;
      totalEdep += edep;
    }
  }
  cout << GetName() << " has " << nofHits << " hits. Total Edep is "
       << totalEdep * 1e03 << " (MeV)" << endl;
}
