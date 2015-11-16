#ifndef EX03_CALORIMETER_SD_H
#define EX03_CALORIMETER_SD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Ex05SensitiveDetector.h
/// \brief Definition of the Ex05SensitiveDetector class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TNamed.h>
#include <TClonesArray.h>

class Ex05CalorHit;

/// \ingroup E05
/// \brief The calorimeter sensitive detector
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

class Ex05Hit;

class Ex05SensitiveDetector : public TNamed
{
  public:
    Ex05SensitiveDetector(const char* name);
    Ex05SensitiveDetector(const Ex05SensitiveDetector& origin);
    Ex05SensitiveDetector();
    virtual ~Ex05SensitiveDetector();

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
    Ex05Hit* GetHit(Int_t i) const;
    TClonesArray* GetHitsCollection() const;

  private:
    // data members
    TClonesArray*  fCaloHitsCollection;   ///< Hits collection 
    Int_t          fCrystalVolId;         ///< The crystal volume Id
    Int_t          fVerboseLevel;         ///< Verbosity level
   
  ClassDef(Ex05SensitiveDetector,1) //Ex05SensitiveDetector 

};

/// Set verbose level
/// \param level The new verbose level value
inline void Ex05SensitiveDetector::SetVerboseLevel(Int_t level) 
{ fVerboseLevel = level; }

/// Return the hits collection
inline TClonesArray* Ex05SensitiveDetector::GetHitsCollection() const
{ return fCaloHitsCollection; }

#endif //EX02_CALORIMETER_SD_H

