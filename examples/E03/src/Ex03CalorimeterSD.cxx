// $Id: Ex03CalorimeterSD.cxx,v 1.1 2003/03/17 14:56:51 brun Exp $
//
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
//
// Id: ExN03CalorimeterSD.cc,v 1.6 2002/01/09 17:24:12 ranjard Exp 
// GEANT4 tag $Name:  $
//
// by Ivana Hrivnacova, 6.3.2003

#include <Riostream.h>
#include <TVirtualMC.h>
#include <TLorentzVector.h>
#include <TTree.h>

#include "Ex03CalorimeterSD.h"
#include "Ex03DetectorConstruction.h"
#include "Ex03CalorHit.h"
#include "Ex02RootManager.h"

ClassImp(Ex03CalorimeterSD)

using namespace std;

//_____________________________________________________________________________
Ex03CalorimeterSD::Ex03CalorimeterSD(const char* name, 
                                     Ex03DetectorConstruction* detector)
  : TNamed(name, ""),
    fDetector(detector),
    fCalCollection(0),
    fAbsorberVolId(0),
    fGapVolId(0),
    fVerboseLevel(1)
{
  // Create hits collection and an empty hit for each layer
  
  fCalCollection = new TClonesArray("Ex03CalorHit", 500);
  for (Int_t i=0; i<fDetector->GetNbOfLayers(); i++) 
    new ((*fCalCollection)[i]) Ex03CalorHit();
}

//_____________________________________________________________________________
Ex03CalorimeterSD::Ex03CalorimeterSD()
  : TNamed(),
    fDetector(0),
    fCalCollection(0),
    fAbsorberVolId(0),
    fGapVolId(0),
    fVerboseLevel(1)
{}

//_____________________________________________________________________________
Ex03CalorimeterSD::~Ex03CalorimeterSD()
{
  if (fCalCollection) fCalCollection->Delete();
  delete fCalCollection;
}

//
// private methods
//

//_____________________________________________________________________________
Ex03CalorHit* Ex03CalorimeterSD::GetHit(Int_t i) const
{
// Returns the hit for the specified layer.
// ---

  return (Ex03CalorHit*)fCalCollection->At(i);
}

//_____________________________________________________________________________
void  Ex03CalorimeterSD::ResetHits()
{
// Resets all hits in the hits collection.
// ---

  for (Int_t i=0; i<fCalCollection->GetEntriesFast(); i++) 
    GetHit(i)->Reset();
} 

//
// public methods
//

//_____________________________________________________________________________
void Ex03CalorimeterSD::Initialize()
{
// Registers hits collection in Root manager;
// sets sensitive volumes.
// ---
  
  Register();
  
  fAbsorberVolId = gMC->VolId("ABSO");
  fGapVolId = gMC->VolId("GAPX");
}

//_____________________________________________________________________________
Bool_t Ex03CalorimeterSD::ProcessHits()
{
// Accounts energy deposit and track lengths for each layer in its hit.
// ---

  Int_t copyNo;
  Int_t id = gMC->CurrentVolID(copyNo);

  if (id != fAbsorberVolId  &&  id != fGapVolId ) 
    return false;

  gMC->CurrentVolOffID(2, copyNo);
  
  Double_t edep = gMC->Edep();

  Double_t step = 0.;
  if (gMC->TrackCharge() != 0.) step = gMC->TrackStep();
  
  if (id == fAbsorberVolId)
    GetHit(copyNo-1)->AddAbs(edep,step);
    
  if (id == fGapVolId)
    GetHit(copyNo-1)->AddGap(edep,step);

  return true;
}

//_____________________________________________________________________________
void Ex03CalorimeterSD::EndOfEvent()
{
// Prints hits collection (if verbose) and resets hits afterwards.
// ---

  if (fVerboseLevel>1)  Print();
    
  // Reset hits collection
  ResetHits();  
}

//_____________________________________________________________________________
void Ex03CalorimeterSD::Register()
{
// Registers the hits collection in Root manager.
// ---
  
  Ex02RootManager::Instance()
    ->Register("hits", "TClonesArray", &fCalCollection);
}

//_____________________________________________________________________________
void Ex03CalorimeterSD::Print() const
{
// Prints the hits collection.
// ---
  
   Int_t nofHits = fCalCollection->GetEntriesFast();
     
   cout << "\n-------->Hits Collection: in this event: " << endl;
	    
   for (Int_t i=0; i<nofHits; i++) (*fCalCollection)[i]->Print();          
}

//_____________________________________________________________________________
void Ex03CalorimeterSD::PrintTotal() const
{
// Prints the total values for all layers.
// ---
  
  Double_t totEAbs=0.;
  Double_t totLAbs=0.;
  Double_t totEGap=0.;
  Double_t totLGap=0.;

  Int_t nofHits = fCalCollection->GetEntriesFast();
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
