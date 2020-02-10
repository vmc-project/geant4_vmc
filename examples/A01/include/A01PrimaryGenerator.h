#ifndef A01_PRIMARY_GENERATOR_H
#define A01_PRIMARY_GENERATOR_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01PrimaryGenerator.h
/// \brief Definition of the A01PrimaryGenerator class
///
/// Geant4 example A01 adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>

class TVirtualMCStack;
class TVector3;

class A01DetectorConstruction;

/// \ingroup A01
/// \brief The primary generator
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

class A01PrimaryGenerator : public TObject
{
 public:
  A01PrimaryGenerator(TVirtualMCStack* stack);
  A01PrimaryGenerator(
    const A01PrimaryGenerator& origin, TVirtualMCStack* stack);
  A01PrimaryGenerator();
  virtual ~A01PrimaryGenerator();

  // methods
  virtual void GeneratePrimaries();

  // set methods
  void SetNofPrimaries(Int_t nofPrimaries);
  void SetMomentum(Double_t val) { fMomentum = val; }
  void SetSigmaMomentum(Double_t val) { fSigmaMomentum = val; }
  void SetSigmaAngle(Double_t val) { fSigmaAngle = val; }
  void SetRandomize(Bool_t val) { fRandomizePrimary = val; }

  // get methods
  Double_t GetMomentum() const { return fMomentum; }
  Double_t GetSigmaMomentum() const { return fSigmaMomentum; }
  Double_t GetSigmaAngle() const { return fSigmaAngle; }
  Bool_t GetRandomize() const { return fRandomizePrimary; }

 private:
  // methods
  // void GeneratePrimary();

  // data members
  TVirtualMCStack* fStack;  ///< VMC stack
  Int_t fNofPrimaries;      ///< Number of primary particles
  Int_t fDefaultParticle;   ///< Default particle PDG
  Double_t fMomentum;       ///< Default particle momentum
  Double_t fSigmaMomentum;  ///< The sigma of particle momentum
  Double_t fSigmaAngle;     ///< The sigma of particle direction
  Bool_t fRandomizePrimary; ///< Option to randomize primary particle type

  ClassDef(A01PrimaryGenerator, 1) // A01PrimaryGenerator
};

// inline functions

/// Set the number of particles to be generated
/// \param nofPrimaries The number of particles to be generated
inline void A01PrimaryGenerator::SetNofPrimaries(Int_t nofPrimaries)
{
  fNofPrimaries = nofPrimaries;
}

#endif // A01_PRIMARY_GENERATOR_H
