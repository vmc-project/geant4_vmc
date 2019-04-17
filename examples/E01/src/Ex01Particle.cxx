//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex01Particle.cxx
/// \brief Implementation of the Ex01Particle class
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex01Particle.h"

#include <TObjArray.h>
#include <TParticle.h>

/// \cond CLASSIMP
ClassImp(Ex01Particle)
  /// \endcond

  //_____________________________________________________________________________
  Ex01Particle::Ex01Particle(Int_t id, TParticle* particle)
  : fID(id), fParticle(particle), fMother(0), fDaughters(0)
{
  /// Standard constructor
  /// \param  id        The particle id
  /// \param  particle  The particle definition (TParticle)
}

//_____________________________________________________________________________
Ex01Particle::Ex01Particle(Int_t id, TParticle* particle, Ex01Particle* mother)
  : fID(id), fParticle(particle), fMother(mother), fDaughters(0)
{
  /// Standard constructor
  /// \param  id        The particle id
  /// \param  particle  The particle definition (TParticle)
  /// \param  mother    The particle mother
}

//_____________________________________________________________________________
Ex01Particle::Ex01Particle() : fID(0), fParticle(0), fMother(0), fDaughters(0)
{
  /// Default constructor
}

//_____________________________________________________________________________
Ex01Particle::~Ex01Particle()
{
  /// Destructor

  if (fDaughters) delete fDaughters;
}

// operators

// public methods

//_____________________________________________________________________________
void Ex01Particle::AddDaughter(Ex01Particle* particle)
{
  /// Add particles daughter
  /// \param particle  The daughter particle

  if (!fDaughters) fDaughters = new TObjArray();

  fDaughters->Add(particle);
}

//_____________________________________________________________________________
void Ex01Particle::SetMother(Ex01Particle* particle)
{
  /// Set particle mother
  /// \param  particle  The mother particle

  fMother = particle;
}

//_____________________________________________________________________________
Int_t Ex01Particle::GetID() const
{
  /// \return The particle Id.

  return fID;
}

//_____________________________________________________________________________
TParticle* Ex01Particle::GetParticle() const
{
  /// \return The particle definition (TParticle).

  return fParticle;
}

//_____________________________________________________________________________
Ex01Particle* Ex01Particle::GetMother() const
{
  /// \return The particle mother.

  return fMother;
}

//_____________________________________________________________________________
Int_t Ex01Particle::GetNofDaughters() const
{
  /// \return The number of daughters.

  if (!fDaughters) return 0;

  return fDaughters->GetEntriesFast();
}

//_____________________________________________________________________________
Ex01Particle* Ex01Particle::GetDaughter(Int_t i) const
{
  /// \return   \em i -th daughter
  /// \param i  The daughter index

  // add test if i

  return (Ex01Particle*)fDaughters->At(i);
}
