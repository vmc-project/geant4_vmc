// $Id: Ex03MCStack.cxx,v 1.1 2005/05/17 13:52:01 brun Exp $
//
// Geant4 ExampleN06 adapted to Virtual Monte Carlo 
//
// Class Ex03MCStack
// Implementation of the TVirtualMCStack interface
// (Taken from example E03)
//
// by Ivana Hrivnacova, 16.5.2005

#include <TParticle.h>
#include <TClonesArray.h>
#include <TError.h>
#include <Riostream.h>

#include "Ex03MCStack.h"

ClassImp(Ex03MCStack)

//_____________________________________________________________________________
Ex03MCStack::Ex03MCStack(Int_t size)
  : fParticles(0),
    fCurrentTrack(-1),
    fNPrimary(0)
{
//
  fParticles = new TClonesArray("TParticle", size);
}

//_____________________________________________________________________________
Ex03MCStack::Ex03MCStack()
  : fParticles(0),
    fCurrentTrack(-1),
    fNPrimary(0)
{
//
}

//_____________________________________________________________________________
Ex03MCStack::~Ex03MCStack() 
{
//
  if (fParticles) fParticles->Delete();
  delete fParticles;
}

// private methods

// public methods

//_____________________________________________________________________________
void  Ex03MCStack::PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	                 Double_t px, Double_t py, Double_t pz, Double_t e,
  		         Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		         Double_t polx, Double_t poly, Double_t polz,
		         TMCProcess mech, Int_t& ntr, Double_t weight,
		         Int_t is) 
{
// Creates a new particle with specified properties,
// adds it to the particles array (fParticles) and if not done to the 
// stack (fStack).
// Use TParticle::fMother[1] to store Track ID. 
// ---

  const Int_t kFirstDaughter=-1;
  const Int_t kLastDaughter=-1;
  
  TClonesArray& particlesRef = *fParticles;
  Int_t trackId = GetNtrack();
  TParticle* particle
    = new(particlesRef[trackId]) 
      TParticle(pdg, is, parent, trackId, kFirstDaughter, kLastDaughter,
		px, py, pz, e, vx, vy, vz, tof);
   
  particle->SetPolarisation(polx, poly, polz);
  particle->SetWeight(weight);
  particle->SetUniqueID(mech);

  if (parent<0) fNPrimary++;  
    
  if (toBeDone) fStack.push(particle);  
}			 

//_____________________________________________________________________________
TParticle* Ex03MCStack::PopNextTrack(Int_t& itrack)
{
// Gets next particle for tracking from the stack.
// ---

  itrack = -1;
  if  (fStack.empty()) return 0;
		      
  TParticle* particle = fStack.top();
  fStack.pop();

  if (!particle) return 0;  
  
  fCurrentTrack = particle->GetSecondMother();
  itrack = fCurrentTrack;
  
  return particle;
}    

//_____________________________________________________________________________
TParticle* Ex03MCStack::PopPrimaryForTracking(Int_t i)
{
// Returns i-th particle in fParticles.
// ---

  if (i < 0 || i >= fNPrimary)
    Fatal("GetPrimaryForTracking", "Index out of range"); 
  
  return (TParticle*)fParticles->At(i);
}     

//_____________________________________________________________________________
void Ex03MCStack::Print(Option_t* /*option*/) const 
{
// Prints info for all particles.
// ---

  cout << "Ex03MCStack Info  " << endl;
  cout << "Total number of particles:   " <<  GetNtrack() << endl;
  cout << "Number of primary particles: " <<  GetNprimary() << endl;

  for (Int_t i=0; i<GetNtrack(); i++)
    GetParticle(i)->Print();
}

//_____________________________________________________________________________
void Ex03MCStack::Reset()
{
// Deletes contained particles, resets particles array and stack.
// ---

  fCurrentTrack = -1;
  fNPrimary = 0;
  fParticles->Clear();
}       

//_____________________________________________________________________________
void  Ex03MCStack::SetCurrentTrack(Int_t track) 
{
// Sets the current track to a given value.
// ---

  fCurrentTrack = track;
}     

//_____________________________________________________________________________
Int_t  Ex03MCStack::GetNtrack() const 
{
// Returns the number of all tracks.
// ---

  return fParticles->GetEntriesFast();
}  

//_____________________________________________________________________________
Int_t  Ex03MCStack::GetNprimary() const 
{
// Returns the number of primary tracks.
// ---

  return fNPrimary;
}  

//_____________________________________________________________________________
TParticle*  Ex03MCStack::GetCurrentTrack() const 
{
// Returns the current track parent ID.
// ---

  TParticle* current = GetParticle(fCurrentTrack);

  if (!current)    
    Warning("GetCurrentTrack", "Current track not found in the stack");

  return current;
}  

//_____________________________________________________________________________
Int_t  Ex03MCStack::GetCurrentTrackNumber() const 
{
// Returns the current track ID.
// ---

  return fCurrentTrack;
}  

//_____________________________________________________________________________
Int_t  Ex03MCStack::GetCurrentParentTrackNumber() const 
{
// Returns the current track parent ID.
// ---

  TParticle* current = GetCurrentTrack();

  if (current) 
    return current->GetFirstMother();
  else 
    return -1;
}  

//_____________________________________________________________________________
TParticle*  Ex03MCStack::GetParticle(Int_t id) const
{
// Returns id-th particle in fParticles.
// ---

  if (id < 0 || id >= fParticles->GetEntriesFast())
    Fatal("GetParticle", "Index out of range"); 
   
  return (TParticle*)fParticles->At(id);
}


