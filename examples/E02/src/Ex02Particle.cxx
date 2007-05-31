// $Id: Ex02Particle.cxx,v 1.2 2005/11/19 07:07:47 brun Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 novice ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex02Particle
// ------------------
// Extended TParticle with pointers to mother and daughters
// particles
//
// by Ivana Hrivnacova, 5.4.2002

#include "Ex02Particle.h"

#include <TParticle.h>
#include <TObjArray.h>

#include <iostream>

using namespace std;

ClassImp(Ex02Particle)

//_____________________________________________________________________________
Ex02Particle::Ex02Particle(Int_t id, TParticle* particle)
  : fID(id),
    fParticle(particle),
    fMother(),
    fDaughters()    
{
//
}

//_____________________________________________________________________________
Ex02Particle::Ex02Particle(Int_t id, TParticle* particle, Ex02Particle* mother)
  : fID(id),
    fParticle(particle),
    fMother(mother),
    fDaughters()    
{
//
}

//_____________________________________________________________________________
Ex02Particle::Ex02Particle()
  : fID(0),
    fParticle(0),
    fMother(),
    fDaughters()    
{
//   
}

//_____________________________________________________________________________
Ex02Particle::~Ex02Particle() 
{
//
  delete fParticle;
}

//
// public methods
//

//_____________________________________________________________________________
void Ex02Particle::SetMother(Ex02Particle* particle)
{
// Adds particles daughter
// ---

  fMother.SetObject(particle);
}  

//_____________________________________________________________________________
void Ex02Particle::AddDaughter(Ex02Particle* particle)
{
// Adds particles daughter
// ---

  fDaughters.Add(particle);
}  

//_____________________________________________________________________________
void Ex02Particle::Print(Option_t* /*option*/) const
{
// Prints particle properties.
// ---

  cout << "Track ID:  " << fID << endl;

  fParticle->Print();  
  
  if (GetMother()) {
    cout << "Mother:    " << GetMother()->GetParticle()->GetName() 
                          << "  with ID: " << GetMother()->GetID() << endl;
  }
  else			    
    cout << "Primary    " << endl; 
    
  cout << "Number of daughters: " << GetNofDaughters() << endl;
  cout << endl;			  
}  

//_____________________________________________________________________________
void Ex02Particle::PrintDaughters() const
{
// Prints particles daughters.
// ---

  for (Int_t i=0; i<GetNofDaughters(); i++)  {
    cout << i << "th daughter: " << endl;
    GetDaughter(i)->Print();
  }  
}  

//_____________________________________________________________________________
Int_t  Ex02Particle:: GetID() const
{
// Returs particle ID.
// ---

  return fID;
}  


//_____________________________________________________________________________
TParticle*  Ex02Particle::GetParticle() const
{
// Returns particle definition (TParticle).
// ---

  return fParticle;
}  

//_____________________________________________________________________________
Ex02Particle* Ex02Particle::GetMother() const
{
// Returns particle definition (TParticle).
// ---

  return (Ex02Particle*) fMother.GetObject();
}  

//_____________________________________________________________________________
Int_t Ex02Particle::GetNofDaughters() const
{
// Returns number of daughters.
// ---

  return fDaughters.GetEntriesFast();
}  

//_____________________________________________________________________________
Ex02Particle* Ex02Particle::GetDaughter(Int_t i) const
{
// Returns i-th daughter.
// ---

  if (i < 0 || i >= GetNofDaughters())
    Fatal("GetDaughter", "Index out of range"); 

  return (Ex02Particle*) fDaughters.At(i);
}  

