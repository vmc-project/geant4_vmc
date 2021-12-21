#ifndef EX03B_CALOR_HIT_H
#define EX03B_CALOR_HIT_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03bCalorHit.h
/// \brief Definition of the Ex03bCalorHit class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo. \n
/// Id: ExN03CalorHit.hh,v 1.5 2002/01/09 17:24:11 ranjard Exp \n
/// GEANT4 tag Name: geant4-05-00
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TVector3.h>

/// \ingroup E03
/// \brief The calorimeter hit
///
/// \date 06/03/2003
/// \author I. Hrivnacova; IPN, Orsay

class Ex03bCalorHit : public TObject
{
 public:
  Ex03bCalorHit();
  virtual ~Ex03bCalorHit();

  // methods
  virtual void Print(Option_t* option = "") const;

  /// Add energy deposit and track length
  /// \param de  Enery deposit
  /// \param dl  Track length
  void Add(Double_t de, Double_t dl)
  {
    fEdep += de;
    fTrackLength += dl;
  }

  void Reset();

  // get methods
  /// \return The energy deposit
  Double_t GetEdep() { return fEdep; };
  /// \return The track length
  Double_t GetTrackLength() { return fTrackLength; };

 private:
  Double_t fEdep;        ///< Energy deposit in the absorber
  Double_t fTrackLength; ///< Track length in the absorber

  ClassDef(Ex03bCalorHit, 1) // Ex03bCalorHit
};

#endif // EX03B_CALOR_HIT_H
