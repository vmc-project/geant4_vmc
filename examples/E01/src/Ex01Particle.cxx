// $Id: Ex01Particle.cxx,v 1.1.1.1 2002/06/16 15:57:36 hristov Exp $
//
// Geant4 novice ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex01Particle
// ------------------
// Extended TParticle with pointers to mother and daughters
// particles
//
// by Ivana Hrivnacova, 5.4.2002

#include "Ex01Particle.h"

#include <TParticle.h>
#include <TObjArray.h>

ClassImp(Ex01Particle)

//_____________________________________________________________________________
Ex01Particle::Ex01Particle(Int_t id, TParticle* particle)
  : fID(id),
    fParticle(particle),
    fMother(0),
    fDaughters(0)    
{
//
}

//_____________________________________________________________________________
Ex01Particle::Ex01Particle(Int_t id, TParticle* particle, Ex01Particle* mother)
  : fID(id),
    fParticle(particle),
    fMother(mother),
    fDaughters(0)    
{
//
}

//_____________________________________________________________________________
Ex01Particle::Ex01Particle()
  : fID(0),
    fParticle(0),
    fMother(0),
    fDaughters(0)    
{
//   
}

//_____________________________________________________________________________
Ex01Particle::~Ex01Particle() 
{
//

  if (fDaughters) delete fDaughters;
}

// operators

// public methods

//_____________________________________________________________________________
void Ex01Particle::AddDaughter(Ex01Particle* particle)
{
// Add particles daughter
// ---

  if (!fDaughters) fDaughters = new TObjArray();
  
  fDaughters->Add(particle);
}  

//_____________________________________________________________________________
void Ex01Particle::SetMother(Ex01Particle* particle)
{
// Add particles daughter
// ---

  fMother = particle;
}  

//_____________________________________________________________________________
Int_t  Ex01Particle:: GetID() const
{
// Returs particle ID.
// ---

  return fID;
}  


//_____________________________________________________________________________
TParticle*  Ex01Particle::GetParticle() const
{
// Returns particle definition (TParticle).
// ---

  return fParticle;
}  

//_____________________________________________________________________________
Ex01Particle* Ex01Particle::GetMother() const
{
// Returns particle definition (TParticle).
// ---

  return fMother;
}  

//_____________________________________________________________________________
Int_t Ex01Particle::GetNofDaughters() const
{
// Returns number of daughters.
// ---

  if (!fDaughters) return 0;
  
  return fDaughters->GetEntriesFast();
}  

//_____________________________________________________________________________
Ex01Particle* Ex01Particle::GetDaughter(Int_t i) const
{
// Returns i-th daughter.
// ---

  // add test if i

  return (Ex01Particle*) fDaughters->At(i);
}  

