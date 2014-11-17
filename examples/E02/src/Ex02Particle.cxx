//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex02Particle.cxx 
/// \brief Implementation of the Ex02Particle class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex02Particle.h"

#include <TParticle.h>
#include <TObjArray.h>

#include <iostream>

using namespace std;

/// \cond CLASSIMP
ClassImp(Ex02Particle)
/// \endcond

//_____________________________________________________________________________
Ex02Particle::Ex02Particle(Int_t id, TParticle* particle)
  : fID(id),
    fParticle(particle),
    fMother(),
    fDaughters()    
{
/// Standard constructor
/// \param  id        The particle id
/// \param  particle  The particle definition (TParticle)
}

//_____________________________________________________________________________
Ex02Particle::Ex02Particle(Int_t id, TParticle* particle, Ex02Particle* mother)
  : fID(id),
    fParticle(particle),
    fMother(mother),
    fDaughters()    
{
/// Standard constructor
/// \param  id        The particle id
/// \param  particle  The particle definition (TParticle)
/// \param  mother    The particle mother
}

//_____________________________________________________________________________
Ex02Particle::Ex02Particle()
  : fID(0),
    fParticle(0),
    fMother(),
    fDaughters()    
{
/// Default constructor
}

//_____________________________________________________________________________
Ex02Particle::~Ex02Particle() 
{
/// Destructor

  delete fParticle;
}

//
// public methods
//

//_____________________________________________________________________________
void Ex02Particle::SetMother(Ex02Particle* particle)
{
/// Set particle mother
/// \param  particle  The mother particle

  fMother.SetObject(particle);
}  

//_____________________________________________________________________________
void Ex02Particle::AddDaughter(Ex02Particle* particle)
{
/// Add particles daughter
/// \param particle  The daughter particle

  fDaughters.Add(particle);
}  

//_____________________________________________________________________________
void Ex02Particle::Print(Option_t* /*option*/) const
{
/// Print particle properties.

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
/// Print particles daughters.

  for (Int_t i=0; i<GetNofDaughters(); i++)  {
    cout << i << "th daughter: " << endl;
    if ( GetDaughter(i) ) 
      GetDaughter(i)->Print();
    else
      cout << "0x0"; 
  }  
  cout << endl;			  
}  

//_____________________________________________________________________________
Int_t  Ex02Particle:: GetID() const
{
/// \return The particle Id.

  return fID;
}  


//_____________________________________________________________________________
TParticle*  Ex02Particle::GetParticle() const
{
/// \return The particle definition (TParticle).

  return fParticle;
}  

//_____________________________________________________________________________
Ex02Particle* Ex02Particle::GetMother() const
{
/// \return The particle mother.

  return (Ex02Particle*) fMother.GetObject();
}  

//_____________________________________________________________________________
Int_t Ex02Particle::GetNofDaughters() const
{
/// \return The number of daughters.

  return fDaughters.GetEntriesFast();
}  

//_____________________________________________________________________________
Ex02Particle* Ex02Particle::GetDaughter(Int_t i) const
{
/// \return   \em i -th daughter
/// \param i  The daughter index

  if (i < 0 || i >= GetNofDaughters())
    Fatal("GetDaughter", "Index out of range"); 

  return (Ex02Particle*) fDaughters.At(i);
}  

