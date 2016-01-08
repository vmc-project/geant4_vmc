//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Garfield/src/SensitiveDetector.cxx 
/// \brief Implementation of the Garfield::SensitiveDetector class 
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "SensitiveDetector.h"
#include "Hit.h"

#include "GarfieldPhysics.h"

#include <Riostream.h>
#include <TVirtualMC.h>
#include <TVirtualMCRootManager.h>
#include <TLorentzVector.h>
#include <TTree.h>

/// \cond CLASSIMP
ClassImp(VMC::Garfield::SensitiveDetector)
/// \endcond

namespace VMC
{
namespace Garfield
{

//_____________________________________________________________________________
SensitiveDetector::SensitiveDetector(const char* name)
  : TNamed(name, ""),
    fHit(0),
    fAbsorberVolId(),
    fGasVolId(),
    fVerboseLevel(1)
{
/// Standard constructor.
/// Create hits collection.
/// \param name      The calorimeter hits collection name

  fHit = new Hit();
}

//_____________________________________________________________________________
SensitiveDetector::SensitiveDetector(const SensitiveDetector& origin)
  : TNamed(origin),
    fHit(0),
    fAbsorberVolId(),
    fGasVolId(),
    fVerboseLevel(origin.fVerboseLevel)
{
/// Copy constructor (for cloning on worker thread in MT mode).
/// Create hits collection.
/// \param origin    The source object (on master).
/// \param detector  The detector construction

  fHit = new Hit();
}

//_____________________________________________________________________________
SensitiveDetector::SensitiveDetector()
  : TNamed(),
    fHit(0),
    fAbsorberVolId(),
    fGasVolId(),
    fVerboseLevel(0)
{
/// Default constructor
}

//_____________________________________________________________________________
SensitiveDetector::~SensitiveDetector()
{
/// Destructor

  delete fHit;
}

//
// public methods
//

//_____________________________________________________________________________
void SensitiveDetector::Initialize()
{
/// Register hits collection in the Root manager;
/// set sensitive volumes.
  
  if ( TVirtualMCRootManager::Instance() ) Register();  
  fAbsorberVolId = gMC->VolId("Absorber");
  fGasVolId = gMC->VolId("Gas");
}

//_____________________________________________________________________________
Bool_t SensitiveDetector::ProcessHits()
{
/// Account energy deposit and track lengths for each layer in its hit.

  Int_t copyNo;
  Int_t id = gMC->CurrentVolID(copyNo);

  if (id != fAbsorberVolId  &&  id != fGasVolId ) 
    return false;

  Double_t edep = gMC->Edep();
  Double_t step = 0.;
  if (gMC->TrackCharge() != 0.) step = gMC->TrackStep();
  
  if (id == fAbsorberVolId) {
    fHit->AddEdepAbs(edep);
    fHit->AddTrackLengthAbs(step);
  }
    
  if (id == fGasVolId) {
    fHit->AddEdepGas(edep);
  }

  return true;
}

//_____________________________________________________________________________
Bool_t  SensitiveDetector::UpdateFromGarfield()
{
/// Update the collected hit information from Garfield interface

  GarfieldPhysics* garfieldPhysics = GarfieldPhysics::GetInstance();

  // get energy deposit from Garfield, convert it in GeV
  Double_t edep = garfieldPhysics->GetEnergyDeposit_MeV()*1e-03;
  Double_t avalancheSize =  garfieldPhysics->GetAvalancheSize();
  Double_t gain = garfieldPhysics->GetGain();

  // update hit 
  fHit->AddEdepGas(edep);
  fHit->AddAvalancheSize(avalancheSize);
  fHit->AddGain(gain);

  return true;
}

//_____________________________________________________________________________
void SensitiveDetector::EndOfEvent()
{
/// Print hits collection (if verbose) and reset hits afterwards.


  if (fVerboseLevel>0)  Print();
    
  // Reset hits collection
  fHit->Reset();

  // Reset data collected in Garfield
  GarfieldPhysics* garfieldPhysics = GarfieldPhysics::GetInstance();
  garfieldPhysics->Clear();
}

//_____________________________________________________________________________
void SensitiveDetector::Register()
{
/// Register the hits collection in Root manager.
  
  TVirtualMCRootManager::Instance()
    ->Register("hit", "Garfield::Hit", &fHit);
}

//_____________________________________________________________________________
void SensitiveDetector::Print(Option_t* /*option*/) const
{
/// Print the hit.
  
   fHit->Print();        
}

}
}

