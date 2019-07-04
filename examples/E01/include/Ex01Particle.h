#ifndef Ex01_PARTICLE_H
#define Ex01_PARTICLE_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Ex01Particle.h
/// \brief Definition of the Ex01Particle class
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>

class TParticle;

/// \ingroup E01
/// \brief Extended TParticle with pointers to mother and daughter
/// particles
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex01Particle : public TObject
{
 public:
  Ex01Particle(Int_t id, TParticle* particle);
  Ex01Particle(Int_t id, TParticle* particle, Ex01Particle* mother);
  Ex01Particle();
  virtual ~Ex01Particle();

  // methods
  void SetMother(Ex01Particle* particle);
  void AddDaughter(Ex01Particle* particle);

  // get methods
  Int_t GetID() const;
  TParticle* GetParticle() const;
  Ex01Particle* GetMother() const;
  Int_t GetNofDaughters() const;
  Ex01Particle* GetDaughter(Int_t i) const;

 private:
  // data members
  Int_t fID;             ///< The particle Id
  TParticle* fParticle;  ///< The particle definition
  Ex01Particle* fMother; ///< The particle mother
  TObjArray* fDaughters; ///< The particle daughters

  ClassDef(Ex01Particle, 1) // Extended TParticle
};

#endif // Ex01_PARTICLE_H
