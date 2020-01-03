#ifndef EX03_CALORIMETER_SD_H
#define EX03_CALORIMETER_SD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Ex03cCalorimeterSD.h
/// \brief Definition of the Ex03cCalorimeterSD class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo: \n
/// Id: ExN03CalorimeterSD.hh,v 1.4 2002/01/09 17:24:11 ranjard Exp
/// GEANT4 tag Name: geant4-05-00
///
/// \author Benedikt Volkel, CERN

#include <TClonesArray.h>
#include <TNamed.h>

class Ex03cDetectorConstruction;
class Ex03CalorHit;
class TVirtualMC;

/// \ingroup E03
/// \brief The calorimeter sensitive detector
///
/// A variant of the Ex03CalorimeterSD class
/// updated for multiple engine runs.
///
/// \date 21/08/2019
/// \author Benedikt Volkel, CERN

class Ex03cCalorimeterSD : public TNamed
{
 public:
  Ex03cCalorimeterSD(const char* name, Ex03cDetectorConstruction* detector);
  Ex03cCalorimeterSD(
    const Ex03cCalorimeterSD& origin, Ex03cDetectorConstruction* detector);
  Ex03cCalorimeterSD();
  virtual ~Ex03cCalorimeterSD();

  // methods
  void Initialize();
  Bool_t ProcessHits();
  void EndOfEvent();
  void Register();
  virtual void Print(Option_t* option = "") const;
  void PrintTotal() const;

  // set methods
  void SetVerboseLevel(Int_t level);

  // get methods
  Ex03CalorHit* GetHit(Int_t i) const;

 private:
  // methods
  void ResetHits();

  // data members
  TVirtualMC* fMC;                      ///< The VMC implementation
  Ex03cDetectorConstruction* fDetector; ///< Detector construction
  TClonesArray* fCalCollection;         ///< Hits collection
  Int_t fAbsorberVolId;                 ///< The absorber volume Id
  Int_t fGapVolId;                      ///< The gap volume Id
  Int_t fVerboseLevel;                  ///< Verbosity level

  ClassDef(Ex03cCalorimeterSD, 1) // Ex03cCalorimeterSD
};

/// Set verbose level
/// \param level The new verbose level value
inline void Ex03cCalorimeterSD::SetVerboseLevel(Int_t level)
{
  fVerboseLevel = level;
}

#endif // EX02_CALORIMETER_SD_H
