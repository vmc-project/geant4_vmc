#ifndef GFLASH_CALORIMETER_SD_H
#define GFLASH_CALORIMETER_SD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Gflash/include/SensitiveDetector.h
/// \brief Definition of the Gflash::SensitiveDetector class
///
/// Geant4 gflash example adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TNamed.h>
#include <TClonesArray.h>

namespace VMC
{
namespace Gflash
{

class Hit;

/// \ingroup Gflash
/// \brief The calorimeter sensitive detector
///
/// Geant4 gflash example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay


class SensitiveDetector : public TNamed
{
  public:
    SensitiveDetector(const char* name);
    SensitiveDetector(const SensitiveDetector& origin);
    SensitiveDetector();
    virtual ~SensitiveDetector();

    // methods
    void    Initialize();
    Bool_t  ProcessHits();
    void    EndOfEvent();
    void    Register();
    virtual void  Print(Option_t* option = "") const;
    void    PrintTotal() const;
    
    // set methods
    void SetVerboseLevel(Int_t level);

    // get methods
    Hit* GetHit(Int_t i) const;
    TClonesArray* GetHitsCollection() const;

  private:
    // data members
    TClonesArray*  fCaloHitsCollection;   ///< Hits collection 
    Int_t          fCrystalVolId;         ///< The crystal volume Id
    Int_t          fVerboseLevel;         ///< Verbosity level
   
  ClassDef(SensitiveDetector,1) //SensitiveDetector 

};

/// Set verbose level
/// \param level The new verbose level value
inline void SensitiveDetector::SetVerboseLevel(Int_t level) 
{ fVerboseLevel = level; }

/// Return the hits collection
inline TClonesArray* SensitiveDetector::GetHitsCollection() const
{ return fCaloHitsCollection; }

}
}

#endif //EX02_CALORIMETER_SD_H

