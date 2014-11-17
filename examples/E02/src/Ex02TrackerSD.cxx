//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex02TrackerSD.cxx 
/// \brief Implementation of the Ex02TrackerSD class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo \n
/// Id: ExN02TrackerSD.cc,v 1.6 2002/01/09 17:24:10 ranjard Exp \n
/// GEANT4 tag Name: geant4-04-00-patch-02 
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include <iostream>

#include <TVirtualMC.h>
#include <TVirtualMCRootManager.h>
#include <TLorentzVector.h>
#include <TTree.h>

#include "Ex02TrackerSD.h"
#include "Ex02TrackerHit.h"

/// \cond CLASSIMP
ClassImp(Ex02TrackerSD)
/// \endcond

using namespace std;

//_____________________________________________________________________________
Ex02TrackerSD::Ex02TrackerSD(const char* name)
  : TNamed(name, ""),
    fTrackerCollection(0),
    fSensitiveVolumeID(-1),
    fVerboseLevel(1)
{
/// Standard constructor
/// \param name  The tracker hits collection name
}

//_____________________________________________________________________________
Ex02TrackerSD::Ex02TrackerSD(const Ex02TrackerSD& origin)
  : TNamed(origin.GetName(), origin.GetTitle()),
    fTrackerCollection(0),
    fSensitiveVolumeID(-1),
    fVerboseLevel(1)
{
/// Copy constructor (for clonig on worker thread in MT mode).
/// \param origin  The source object (on master).
}

//_____________________________________________________________________________
Ex02TrackerSD::Ex02TrackerSD()
  : TNamed(),
    fTrackerCollection(0),
    fSensitiveVolumeID(-1),
    fVerboseLevel(1)
{
/// Default constructor
}

//_____________________________________________________________________________
Ex02TrackerSD::~Ex02TrackerSD()
{
/// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
Ex02TrackerHit* Ex02TrackerSD::AddHit()
{
/// Create a new hit in the TClonesArray.
/// \return  The new hit

  TClonesArray& ref = *fTrackerCollection;
  Int_t size = ref.GetEntriesFast();

  return new(ref[size]) Ex02TrackerHit();
}

//
// public methods
//

//_____________________________________________________________________________
void Ex02TrackerSD::Initialize()
{
/// Register hits collection in the Root manager;
/// set sensitive volumes.
  
  static __thread Bool_t registered = false;
  if ( ! registered ) {
    //cout << "... creating TClonesArray" << endl;

    // Lock Root when creating data - seems not to be needed ?
    fTrackerCollection = new TClonesArray("Ex02TrackerHit");

    // Register to Root IO only if RootManager is instantiated
    if ( TVirtualMCRootManager::Instance() ) Register();
    registered = true;
  }   

  fSensitiveVolumeID = gMC->VolId("CHMB");
}

//_____________________________________________________________________________
Bool_t Ex02TrackerSD::ProcessHits()
{
/// Create hits (in stepping).

  Int_t copyNo;
  Int_t id = gMC->CurrentVolID(copyNo);

  if (id != fSensitiveVolumeID) return false;

  Double_t edep = gMC->Edep();

  if (edep==0.) return false;
  
  Ex02TrackerHit* newHit = AddHit();

  // Track ID
  newHit->SetTrackID (gMC->GetStack()->GetCurrentTrackNumber());

  // Chamber no
  newHit->SetChamberNb(copyNo);

  // Energy deposit
  newHit->SetEdep(edep);

  // Position
  TLorentzVector pos;
  gMC->TrackPosition(pos);
  newHit->SetPos (TVector3(pos.X(), pos.Y(), pos.Z()));
  
  //newHit->Print();
  //newHit->Draw();

  return true;
}

//_____________________________________________________________________________
void Ex02TrackerSD::EndOfEvent()
{
/// Print hits collection (if verbose)
/// and delete hits afterwards.

  if (fVerboseLevel>0)  Print();
    
  // Reset hits collection
  fTrackerCollection->Clear();  
}

//_____________________________________________________________________________
void Ex02TrackerSD::Register()
{
/// Register the hits collection in the Root manager.
  
  TVirtualMCRootManager::Instance()
    ->Register("hits", "TClonesArray", &fTrackerCollection);
}

//_____________________________________________________________________________
void Ex02TrackerSD::Print(const Option_t* /*option*/) const
{
/// Print the hits collection.
  
   Int_t nofHits = fTrackerCollection->GetEntriesFast();
     
   cout << "\n-------->Hits Collection: in this event they are " << nofHits 
        << " hits in the tracker chambers: " << endl;
	    
   for (Int_t i=0; i<nofHits; i++) (*fTrackerCollection)[i]->Print();          
}
