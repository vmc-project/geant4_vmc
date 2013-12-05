// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file E03/src/Ex03MCStack.cxx 
/// \brief Implementation of the Ex03MCStack class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \date 06/03/2002
/// \author I. Hrivnacova; IPN, Orsay

#include <TParticle.h>
#include <TClonesArray.h>
#include <TError.h>
#include <Riostream.h>

#include "Ex03MCStack.h"

/// \cond CLASSIMP
ClassImp(Ex03MCStack)
/// \endcond

//_____________________________________________________________________________
Ex03MCStack::Ex03MCStack(Int_t size)
  : fParticles(0),
    fCurrentTrack(-1),
    fNPrimary(0)
{
/// Standard constructor
/// \param size  The stack size

  fParticles = new TClonesArray("TParticle", size);
}

//_____________________________________________________________________________
Ex03MCStack::Ex03MCStack()
  : fParticles(0),
    fCurrentTrack(-1),
    fNPrimary(0)
{
/// Default constructor
}

//_____________________________________________________________________________
Ex03MCStack::~Ex03MCStack() 
{
/// Destructor

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
/// Create a new particle and push into stack;
/// adds it to the particles array (fParticles) and if not done to the 
/// stack (fStack).
/// Use TParticle::fMother[1] to store Track ID. 
/// \param toBeDone  1 if particles should go to tracking, 0 otherwise
/// \param parent    number of the parent track, -1 if track is primary
/// \param pdg       PDG encoding
/// \param px        particle momentum - x component [GeV/c]
/// \param py        particle momentum - y component [GeV/c]
/// \param pz        particle momentum - z component [GeV/c]
/// \param e         total energy [GeV]
/// \param vx        position - x component [cm]
/// \param vy        position - y component  [cm]
/// \param vz        position - z component  [cm]
/// \param tof       time of flight [s]
/// \param polx      polarization - x component
/// \param poly      polarization - y component
/// \param polz      polarization - z component
/// \param mech      creator process VMC code
/// \param ntr       track number (is filled by the stack
/// \param weight    particle weight
/// \param is        generation status code

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
  
  ntr = GetNtrack() - 1;   
}			 

//_____________________________________________________________________________
TParticle* Ex03MCStack::PopNextTrack(Int_t& itrack)
{
/// Get next particle for tracking from the stack.
/// \return       The popped particle object
/// \param track  The index of the popped track

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
/// Return \em i -th particle in fParticles.
/// \return   The popped primary particle object
/// \param i  The index of primary particle to be popped

  if (i < 0 || i >= fNPrimary)
    Fatal("GetPrimaryForTracking", "Index out of range"); 
  
  return (TParticle*)fParticles->At(i);
}     

//_____________________________________________________________________________
void Ex03MCStack::Print(Option_t* /*option*/) const 
{
/// Print info for all particles.

  cout << "Ex03MCStack Info  " << endl;
  cout << "Total number of particles:   " <<  GetNtrack() << endl;
  cout << "Number of primary particles: " <<  GetNprimary() << endl;

  for (Int_t i=0; i<GetNtrack(); i++)
    GetParticle(i)->Print();
}

//_____________________________________________________________________________
void Ex03MCStack::Reset()
{
/// Delete contained particles, reset particles array and stack.

  fCurrentTrack = -1;
  fNPrimary = 0;
  fParticles->Clear();
}       

//_____________________________________________________________________________
void  Ex03MCStack::SetCurrentTrack(Int_t track) 
{
/// Set the current track number to a given value.
/// \param  track The current track number

  fCurrentTrack = track;
}     

//_____________________________________________________________________________
Int_t  Ex03MCStack::GetNtrack() const 
{
/// \return  The total number of all tracks.

  return fParticles->GetEntriesFast();
}  

//_____________________________________________________________________________
Int_t  Ex03MCStack::GetNprimary() const 
{
/// \return  The total number of primary tracks.

  return fNPrimary;
}  

//_____________________________________________________________________________
TParticle*  Ex03MCStack::GetCurrentTrack() const 
{
/// \return  The current track particle

  TParticle* current = GetParticle(fCurrentTrack);

  if (!current)    
    Warning("GetCurrentTrack", "Current track not found in the stack");

  return current;
}  

//_____________________________________________________________________________
Int_t  Ex03MCStack::GetCurrentTrackNumber() const 
{
/// \return  The current track number

  return fCurrentTrack;
}  

//_____________________________________________________________________________
Int_t  Ex03MCStack::GetCurrentParentTrackNumber() const 
{
/// \return  The current track parent ID.

  TParticle* current = GetCurrentTrack();

  if (current) 
    return current->GetFirstMother();
  else 
    return -1;
}  

//_____________________________________________________________________________
TParticle*  Ex03MCStack::GetParticle(Int_t id) const
{
/// \return   The \em id -th particle in fParticles
/// \param id The index of the particle to be returned

  if (id < 0 || id >= fParticles->GetEntriesFast())
    Fatal("GetParticle", "Index out of range"); 
   
  return (TParticle*)fParticles->At(id);
}


