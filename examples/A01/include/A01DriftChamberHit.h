#ifndef A01_DRIFT_CHAMBER_HIT_H
#define A01_DRIFT_CHAMBER_HIT_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01DriftChamberHit.h
/// \brief Definition of the A01DriftChamberHit class
///
/// Geant4 example A01 adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TMatrix.h>
#include <TObject.h>
#include <TVector3.h>

/// \ingroup A01
/// \brief The drift chamber hit
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

class A01DriftChamberHit : public TObject
{
 public:
  A01DriftChamberHit(Int_t z);
  A01DriftChamberHit();
  virtual ~A01DriftChamberHit();

  // methods
  virtual void Print(Option_t* option = "") const;
  void Reset();

  // set methods
  void SetLayerID(Int_t id) { fLayerID = id; }
  void SetTime(Double_t t) { fTime = t; }
  void SetLocalPos(const TVector3& pos) { fLocalPos = pos; }
  void SetWorldPos(const TVector3& pos) { fWorldPos = pos; }

  // get methods
  Int_t GetLayerID() const { return fLayerID; }
  Double_t GetTime() const { return fTime; }
  TVector3 GetLocalPos() const { return fLocalPos; };
  TVector3 GetWorldPos() const { return fWorldPos; };

 private:
  Int_t fLayerID;     ///< The layer ID
  Double_t fTime;     ///< The hit time
  TVector3 fLocalPos; ///< The local hit position
  TVector3 fWorldPos; ///< The global hit position

  ClassDef(A01DriftChamberHit, 1) // A01DriftChamberHit
};

#endif // A01_DRIFT_CHAMBER_HIT_H
