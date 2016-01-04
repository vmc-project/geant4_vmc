//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/src/SensitiveDetector.cxx 
/// \brief Implementation of the SensitiveDetector class 
///
/// Geant4 TestEm10 adapted to Virtual Monte Carlo.
///
/// \date 18/12/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "SensitiveDetector.h"

#include <TVirtualMC.h>
#include <Riostream.h>

/// \cond CLASSIMP
ClassImp(VMC::TR::SensitiveDetector)
/// \endcond

using namespace std;

namespace VMC
{
namespace TR
{

//_____________________________________________________________________________
SensitiveDetector::SensitiveDetector(const char* name)
  : TNamed(name, ""),
    fEdep(0.),
    fAbsorberVolId(0),
    fVerboseLevel(1)
{
/// Standard constructor.
/// \param name      The sensitive detector name
}

//_____________________________________________________________________________
SensitiveDetector::SensitiveDetector(const SensitiveDetector& origin)
  : TNamed(origin),
    fEdep(0.),
    fAbsorberVolId(origin.fAbsorberVolId),
    fVerboseLevel(origin.fVerboseLevel)
{
/// Copy constructor (for clonig on worker thread in MT mode).
/// Create hits collection.
/// \param origin    The source object (on master).
}

//_____________________________________________________________________________
SensitiveDetector::SensitiveDetector()
  : TNamed(),
    fEdep(0.),
    fAbsorberVolId(0),
    fVerboseLevel(1)
{
/// Default constructor
}

//_____________________________________________________________________________
SensitiveDetector::~SensitiveDetector()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void SensitiveDetector::Initialize()
{
/// Set sensitive volumes.
  
  fAbsorberVolId = gMC->VolId("Absorber");
}

//_____________________________________________________________________________
Bool_t SensitiveDetector::ProcessHits()
{
/// Account energy deposit.

  Int_t copyNo;
  Int_t id = gMC->CurrentVolID(copyNo);

  if (id != fAbsorberVolId ) 
    return false;

  fEdep += gMC->Edep();

  return true;
}

//_____________________________________________________________________________
void SensitiveDetector::EndOfEvent()
{
/// Print the enrgy deposit (if verbose) and reset hits afterwards.

  if (fVerboseLevel>1)  Print();
    
  // Reset the accumulated values.
  fEdep = 0.;  
}

//_____________________________________________________________________________
void SensitiveDetector::Print(Option_t* /*option*/) const
{
/// Print the energy deposit.
  
  cout << "\n-------->Edep: in this event [MeV]: " << fEdep*1e+03 
       << endl << endl;
}

}
}
