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

/// \file  Ex03bCalorimeterSD.h
/// \brief Definition of the Ex03bCalorimeterSD class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo: \n
/// Id: ExN03CalorimeterSD.hh,v 1.4 2002/01/09 17:24:11 ranjard Exp
/// GEANT4 tag Name: geant4-05-00
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TClonesArray.h>
#include <TVirtualMCSensitiveDetector.h>

class Ex03bDetectorConstruction;
class Ex03bCalorHit;
class TVirtualMC;

/// \ingroup E03
/// \brief The calorimeter sensitive detector
///
/// A variant of the Ex03CalorimeterSD:
/// a sensitive detector class derived fron the new
/// TVirtualMCSensitiveDetector interface
///
/// \author I. Hrivnacova; IPN, Orsay

class Ex03bCalorimeterSD : public TVirtualMCSensitiveDetector
{
 public:
  Ex03bCalorimeterSD(const char* name, Ex03bDetectorConstruction* detector);
  Ex03bCalorimeterSD(
    const Ex03bCalorimeterSD& origin, Ex03bDetectorConstruction* detector);
  Ex03bCalorimeterSD();
  virtual ~Ex03bCalorimeterSD();

  // methods
  void Initialize();
  virtual void ProcessHits();
  void EndOfEvent();
  void Register();
  virtual void Print(Option_t* option = "") const;
  void PrintTotal() const;

  // set methods
  void SetVerboseLevel(Int_t level);
  void SetPrintModulo(Int_t value);

  // get methods
  Ex03bCalorHit* GetHit(Int_t i) const;

 private:
  // methods
  void ResetHits();

  // data members
  TVirtualMC* fMC;                      ///< The VMC implementation
  Ex03bDetectorConstruction* fDetector; ///< Detector construction
  TClonesArray* fCalCollection;         ///< Hits collection
  Int_t fAbsorberVolId;                 ///< The absorber volume Id
  Int_t fGapVolId;                      ///< The gap volume Id
  Int_t fVerboseLevel;                  ///< Verbosity level
  Int_t fPrintModulo; ///< The event modulus number to be printed

  ClassDef(Ex03bCalorimeterSD, 1) // Ex03bCalorimeterSD
};

/// Set verbose level
/// \param level The new verbose level value
inline void Ex03bCalorimeterSD::SetVerboseLevel(Int_t level)
{
  fVerboseLevel = level;
}

/// Set the event modulus number to be printed
/// \param value  The new event modulus number value
inline void Ex03bCalorimeterSD::SetPrintModulo(Int_t value)
{
  fPrintModulo = value;
}

#endif // EX02_CALORIMETER_SD_H
