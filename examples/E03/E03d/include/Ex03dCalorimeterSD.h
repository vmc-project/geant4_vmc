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

/// \file  Ex03dCalorimeterSD.h
/// \brief Definition of the Ex03dCalorimeterSD class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo: \n
/// Id: ExN03CalorimeterSD.hh,v 1.4 2002/01/09 17:24:11 ranjard Exp
/// GEANT4 tag Name: geant4-05-00
///
/// \author Radoslaw Karabowicz; GSI

#include <TNamed.h>
#include <vector>

#include "Ex03CalorHit.h"

class Ex03DetectorConstruction;
class TVirtualMC;

/// \ingroup E03
/// \brief The calorimeter sensitive detector
///
/// A variant of the Ex03CalorimeterSD class
/// updated for RNTuple output.
///
/// \date 07/07/2026
/// \author Radoslaw Karabowicz; GSI

class Ex03dCalorimeterSD : public TNamed
{
 public:
  Ex03dCalorimeterSD(const char* name, Ex03DetectorConstruction* detector);
  Ex03dCalorimeterSD(
    const Ex03dCalorimeterSD& origin, Ex03DetectorConstruction* detector);
  Ex03dCalorimeterSD();
  virtual ~Ex03dCalorimeterSD();

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
  TVirtualMC* fMC;                     ///< The VMC implementation
  Ex03DetectorConstruction* fDetector; ///< Detector construction
  std::vector<Ex03CalorHit>* fCalCollection{
    new std::vector<Ex03CalorHit>
  };                    //! < The vector of particle (persistent)
  Int_t fAbsorberVolId; ///< The absorber volume Id
  Int_t fGapVolId;      ///< The gap volume Id
  Int_t fVerboseLevel;  ///< Verbosity level

  ClassDef(Ex03dCalorimeterSD, 1) // Ex03dCalorimeterSD
};

/// Set verbose level
/// \param level The new verbose level value
inline void Ex03dCalorimeterSD::SetVerboseLevel(Int_t level)
{
  fVerboseLevel = level;
}

#endif // EX02_CALORIMETER_SD_H
