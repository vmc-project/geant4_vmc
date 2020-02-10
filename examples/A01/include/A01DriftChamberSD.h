#ifndef A01_DRIFT_CHAMBER_SD_H
#define A01_DRIFT_CHAMBER_SD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  A01DriftChamberSD.h
/// \brief Definition of the A01DriftChamberSD class
///
/// Geant4 example A01 adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TClonesArray.h>
#include <TNamed.h>

class A01DriftChamberHit;

/// \ingroup A01
/// \brief The calorimeter sensitive detector
///
/// \author I. Hrivnacova; IPN, Orsay

class A01DriftChamberSD : public TNamed
{
 public:
  A01DriftChamberSD(const char* name, const char* volName);
  A01DriftChamberSD(const A01DriftChamberSD& origin);
  A01DriftChamberSD();
  virtual ~A01DriftChamberSD();

  // methods
  void Initialize();
  Bool_t ProcessHits();
  void EndOfEvent();
  void Register();
  virtual void Print(Option_t* option = "") const;
  // void    PrintTotal() const;

  // set methods
  void SetWriteHits(Bool_t writeHits);
  void SetVerboseLevel(Int_t level);

  // get methods
  A01DriftChamberHit* GetHit(Int_t i) const;

 private:
  // data members
  TClonesArray* fHitsCollection; ///< Hits collection
  TString fVolName;              ///< The sensitive volume name
  Int_t fVolId;                  ///< The calorimeter volume Id
  Bool_t fWriteHits;             ///< Option to write hits
  Int_t fVerboseLevel;           ///< Verbosity level

  ClassDef(A01DriftChamberSD, 1) // A01DriftChamberSD
};

/// (In)Activate writing hits on file
/// \param writeHits  The new value of the option
inline void A01DriftChamberSD::SetWriteHits(Bool_t writeHits)
{
  fWriteHits = writeHits;
}

/// Set verbose level
/// \param level The new verbose level value
inline void A01DriftChamberSD::SetVerboseLevel(Int_t level)
{
  fVerboseLevel = level;
}

#endif // A01_DRIFT_CHAMBER_SD_H
