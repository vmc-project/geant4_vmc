// $Id: Ex02MCStack.cxx,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
//
// Geant4 ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex02MCStack
// -----------------
// Implementation of the TVirtualMCStack interface
//
// by Ivana Hrivnacova, 5.4.2002

#include "Ex02MCStack.h"

#include <TParticle.h>
#include <TObjArray.h>
#include <TError.h>

#include <iostream>

using namespace std;

ClassImp(Ex02MCStack)

//_____________________________________________________________________________
Ex02MCStack::Ex02MCStack(Int_t size)
  : fParticles(0),
    fCurrentTrack(-1),
    fNPrimary(0)
{
//
  fParticles = new TObjArray(size);
}

//_____________________________________________________________________________
Ex02MCStack::Ex02MCStack()
  : fParticles(0),
    fCurrentTrack(-1),
    fNPrimary(0)
{
//
}

//_____________________________________________________________________________
Ex02MCStack::~Ex02MCStack() 
{
//
  if (fParticles) fParticles->Delete();
  delete fParticles;
}

// private methods

// public methods

//_____________________________________________________________________________
void  Ex02MCStack::SetTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	                 Double_t px, Double_t py, Double_t pz, Double_t e,
  		         Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		         Double_t polx, Double_t poly, Double_t polz,
		         TMCProcess mech, Int_t& ntr, Double_t weight,
		         Int_t is) 
{
// Creates a new particle with specified properties,
// adds it to the particles array (fParticles) and if not done to the 
// stack (fStack).
// ---

  const Int_t kFirstDaughter=-1;
  const Int_t kLastDaughter=-1;
  
  TParticle* particleDef
    = new TParticle(pdg, is, parent, -1, kFirstDaughter, kLastDaughter,
		     px, py, pz, e, vx, vy, vz, tof);
   
  particleDef->SetPolarisation(polx, poly, polz);
  particleDef->SetWeight(weight);
  particleDef->SetUniqueID(mech);

  Ex02Particle* mother = 0;
  if (parent>=0) 
    mother = GetParticle(parent);
  else
    fNPrimary++;  

  Ex02Particle* particle = new Ex02Particle(GetNtrack(), particleDef, mother);
  if (mother) mother->AddDaughter(particle);

  fParticles->Add(particle);
    
  if (toBeDone) fStack.push(particle);  
}			 

//_____________________________________________________________________________
TParticle* Ex02MCStack::GetNextTrack(Int_t& itrack)
{
// Gets next particle for tracking from the stack.
// ---

  itrack = -1;
  if  (fStack.empty()) return 0;
		      
  Ex02Particle* particle = fStack.top();
  fStack.pop();

  if (!particle) return 0;  
  
  itrack = particle->GetID();
  fCurrentTrack = itrack;

  return particle->GetParticle();
}    

//_____________________________________________________________________________
TParticle* Ex02MCStack::GetPrimaryForTracking(Int_t i)
{
// Returns i-th particle in fParticles.
// ---

  if (i < 0 || i >= fNPrimary)
    Fatal("GetPrimaryForTracking", "Index out of range"); 
  
  return ((Ex02Particle*)fParticles->At(i))->GetParticle();
}     

//_____________________________________________________________________________
void Ex02MCStack::Print() const 
{
// Prints info for all particles.
// ---

  cout << "Ex02MCStack Info  " << endl;
  cout << "Total number of particles:   " <<  GetNtrack() << endl;
  cout << "Number of primary particles: " <<  GetNprimary() << endl;

  for (Int_t i=0; i<GetNtrack(); i++) {
    GetParticle(i)->Print();
    //GetParticle(i)->PrintDaughters();
  }  
}

//_____________________________________________________________________________
void Ex02MCStack::Reset()
{
// Deletes contained particles, resets particles array and stack.
// ---

  // reset fStack
  fCurrentTrack = -1;
  fNPrimary = 0;
  fParticles->Delete();
}       

//_____________________________________________________________________________
void  Ex02MCStack::SetCurrentTrack(Int_t track) 
{
// Sets the current track to a given value.
// ---

  fCurrentTrack = track;
}     

//_____________________________________________________________________________
Int_t  Ex02MCStack::GetNtrack() const 
{
// Returns the number of all tracks.
// ---

  return fParticles->GetEntriesFast();
}  

//_____________________________________________________________________________
Int_t  Ex02MCStack::GetNprimary() const 
{
// Returns the number of primary tracks.
// ---

  return fNPrimary;
}  

//_____________________________________________________________________________
Int_t  Ex02MCStack::CurrentTrack() const 
{
// Returns the current track ID.
// ---

  return fCurrentTrack;
}  

//_____________________________________________________________________________
Ex02Particle*  Ex02MCStack::GetParticle(Int_t id) const
{
// Returns id-th particle in fParticles.
// ---

  if (id < 0 || id >= fParticles->GetEntriesFast())
    Fatal("GetParticle", "Index out of range"); 
   
  return (Ex02Particle*)fParticles->At(id);
}


