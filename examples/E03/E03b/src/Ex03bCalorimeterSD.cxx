//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03bCalorimeterSD.cxx
/// \brief Implementation of the Ex03bCalorimeterSD class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
/// Id: ExN03CalorimeterSD.cc,v 1.6 2002/01/09 17:24:12 ranjard Exp \n
/// GEANT4 tag $Name:  $
///
/// \date 18/12/2018
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex03bCalorimeterSD.h"
#include "Ex03CalorHit.h"
#include "Ex03DetectorConstruction.h"

#include <Riostream.h>
#include <TLorentzVector.h>
#include <TMCRootManager.h>
#include <TTree.h>
#include <TVirtualMC.h>

/// \cond CLASSIMP
ClassImp(Ex03bCalorimeterSD)
  /// \endcond

  using namespace std;

//_____________________________________________________________________________
Ex03bCalorimeterSD::Ex03bCalorimeterSD(
  const char* name, Ex03DetectorConstruction* detector)
  : TVirtualMCSensitiveDetector(name, ""),
    fMC(0),
    fDetector(detector),
    fCalCollection(0),
    fAbsorberVolId(0),
    fGapVolId(0),
    fVerboseLevel(1),
    fPrintModulo(1)
{
  /// Standard constructor.
  /// Create hits collection and an empty hit for each layer
  /// As the copy numbers may start from 0 or 1 (depending on
  /// geometry model, we create one more layer for this case.)
  /// \param name      The calorimeter hits collection name
  /// \param detector  The detector construction

  fCalCollection = new TClonesArray("Ex03CalorHit", 500);
  for (Int_t i = 0; i < fDetector->GetNbOfLayers() + 1; i++)
    new ((*fCalCollection)[i]) Ex03CalorHit();
}

//_____________________________________________________________________________
Ex03bCalorimeterSD::Ex03bCalorimeterSD(
  const Ex03bCalorimeterSD& origin, Ex03DetectorConstruction* detector)
  : TVirtualMCSensitiveDetector(origin),
    fMC(0),
    fDetector(detector),
    fCalCollection(0),
    fAbsorberVolId(origin.fAbsorberVolId),
    fGapVolId(origin.fGapVolId),
    fVerboseLevel(origin.fVerboseLevel),
    fPrintModulo(origin.fPrintModulo)
{
  /// Copy constructor (for clonig on worker thread in MT mode).
  /// Create hits collection and an empty hit for each layer
  /// As the copy numbers may start from 0 or 1 (depending on
  /// geometry model, we create one more layer for this case.)
  /// \param origin    The source object (on master).
  /// \param detector  The detector construction

  fCalCollection = new TClonesArray("Ex03CalorHit", 500);
  for (Int_t i = 0; i < fDetector->GetNbOfLayers() + 1; i++)
    new ((*fCalCollection)[i]) Ex03CalorHit();
}

//_____________________________________________________________________________
Ex03bCalorimeterSD::Ex03bCalorimeterSD()
  : TVirtualMCSensitiveDetector(),
    fDetector(0),
    fCalCollection(0),
    fAbsorberVolId(0),
    fGapVolId(0),
    fVerboseLevel(1)
{
  /// Default constructor
}

//_____________________________________________________________________________
Ex03bCalorimeterSD::~Ex03bCalorimeterSD()
{
  /// Destructor

  if (fCalCollection) fCalCollection->Delete();
  delete fCalCollection;
}

//
// private methods
//

//_____________________________________________________________________________
Ex03CalorHit* Ex03bCalorimeterSD::GetHit(Int_t i) const
{
  /// \return   The hit for the specified layer.
  /// \param i  The layer number

  return (Ex03CalorHit*)fCalCollection->At(i);
}

//_____________________________________________________________________________
void Ex03bCalorimeterSD::ResetHits()
{
  /// Reset all hits in the hits collection.

  for (Int_t i = 0; i < fCalCollection->GetEntriesFast(); i++)
    GetHit(i)->Reset();
}

//
// public methods
//

//_____________________________________________________________________________
void Ex03bCalorimeterSD::Initialize()
{
  /// Register hits collection in the Root manager;
  /// set sensitive volumes.

  if (TMCRootManager::Instance()) Register();

  // Keep the pointer to TVirtualMC object as a data member
  // to avoid a possible performance penalty due to a frequent retrieval
  // from the thread-local storage
  fMC = gMC;

  fAbsorberVolId = fMC->VolId("ABSO");
  fGapVolId = fMC->VolId("GAPX");

  if (fAbsorberVolId == 0 && fGapVolId == 0) {
    // Volume names are different in B4 detector construction
    fAbsorberVolId = fMC->VolId("Abso");
    fGapVolId = fMC->VolId("Gap");
  }
}

//_____________________________________________________________________________
void Ex03bCalorimeterSD::ProcessHits()
{
  /// Account energy deposit and track lengths for each layer in its hit.

  Int_t copyNo;
  Int_t id = fMC->CurrentVolID(copyNo);

  if (id != fAbsorberVolId && id != fGapVolId) return;

  fMC->CurrentVolOffID(2, copyNo);

  Double_t edep = fMC->Edep();

  Double_t step = 0.;
  if (fMC->TrackCharge() != 0.) step = fMC->TrackStep();

  if (!GetHit(copyNo)) {
    return;
  }

  if (id == fAbsorberVolId) {
    GetHit(copyNo)->AddAbs(edep, step);
  }

  if (id == fGapVolId) {
    GetHit(copyNo)->AddGap(edep, step);
  }
}

//_____________________________________________________________________________
void Ex03bCalorimeterSD::EndOfEvent()
{
  /// Print hits collection (if verbose) and reset hits afterwards.

  if (gMC->CurrentEvent() % fPrintModulo == 0) {
    PrintTotal();
  }

  if (fVerboseLevel > 1) Print();

  // Reset hits collection
  ResetHits();
}

//_____________________________________________________________________________
void Ex03bCalorimeterSD::Register()
{
  /// Register the hits collection in Root manager.

  TMCRootManager::Instance()->Register("hits", "TClonesArray", &fCalCollection);
}

//_____________________________________________________________________________
void Ex03bCalorimeterSD::Print(Option_t* /*option*/) const
{
  /// Print the hits collection.

  Int_t nofHits = fCalCollection->GetEntriesFast();

  cout << "\n-------->Hits Collection: in this event: " << endl;

  for (Int_t i = 0; i < nofHits; i++) (*fCalCollection)[i]->Print();
}

//_____________________________________________________________________________
void Ex03bCalorimeterSD::PrintTotal() const
{
  /// Print the total values for all layers.

  Double_t totEAbs = 0.;
  Double_t totLAbs = 0.;
  Double_t totEGap = 0.;
  Double_t totLGap = 0.;

  Int_t nofHits = fCalCollection->GetEntriesFast();
  for (Int_t i = 0; i < nofHits; i++) {
    totEAbs += GetHit(i)->GetEdepAbs();
    totLAbs += GetHit(i)->GetTrakAbs();
    totEGap += GetHit(i)->GetEdepGap();
    totLGap += GetHit(i)->GetTrakGap();
  }

  cout << "   Absorber: total energy (MeV): " << setw(7) << totEAbs * 1.0e03
       << "       total track length (cm):  " << setw(7) << totLAbs << endl
       << "   Gap:      total energy (MeV): " << setw(7) << totEGap * 1.0e03
       << "       total track length (cm):  " << setw(7) << totLGap << endl;
}
