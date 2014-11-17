//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex02MCStack.cxx 
/// \brief Implementation of the Ex02MCStack class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex02MCStack.h"

#include <TParticle.h>
#include <TObjArray.h>
#include <TError.h>

#include <iostream>

using namespace std;

/// \cond CLASSIMP
ClassImp(Ex02MCStack)
/// \endcond

//_____________________________________________________________________________
Ex02MCStack::Ex02MCStack(Int_t size)
  : fParticles(0),
    fCurrentTrack(-1),
    fNPrimary(0),
    fObjectNumber(0)    
{
/// Standard constructor
/// \param size  The stack size

  fParticles = new TObjArray(size);
}

//_____________________________________________________________________________
Ex02MCStack::Ex02MCStack()
  : fParticles(0),
    fCurrentTrack(-1),
    fNPrimary(0),
    fObjectNumber(0)    
{
/// Default constructor
}

//_____________________________________________________________________________
Ex02MCStack::~Ex02MCStack() 
{
/// Destructor

  if (fParticles) fParticles->Delete();
  delete fParticles;
}

// private methods

// public methods

//_____________________________________________________________________________
void  Ex02MCStack::PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	                 Double_t px, Double_t py, Double_t pz, Double_t e,
  		         Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		         Double_t polx, Double_t poly, Double_t polz,
		         TMCProcess mech, Int_t& ntr, Double_t weight,
		         Int_t is) 
{
/// Create a new particle and push into stack;
/// adds it to the particles array (fParticles) and if not done to the 
/// stack (fStack).
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
  
  TParticle* particleDef
    = new TParticle(pdg, is, parent, -1, kFirstDaughter, kLastDaughter,
		     px, py, pz, e, vx, vy, vz, tof);
   
  particleDef->SetPolarisation(polx, poly, polz);
  particleDef->SetWeight(weight);
  particleDef->SetUniqueID(mech);

  Ex02Particle* mother = 0;
  if ( parent >= 0 ) 
    mother = GetParticle(parent);
  else
    fNPrimary++;  

  Ex02Particle* particle = new Ex02Particle(GetNtrack(), particleDef, mother);
  if (mother) mother->AddDaughter(particle);
  fParticles->Add(particle);
    
  if (toBeDone) fStack.push(particle);  

  ntr = GetNtrack() - 1;   
}			 

//_____________________________________________________________________________
TParticle* Ex02MCStack::PopNextTrack(Int_t& itrack)
{
/// Get next particle for tracking from the stack.
/// \return       The popped particle object
/// \param track  The index of the popped track

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
TParticle* Ex02MCStack::PopPrimaryForTracking(Int_t i)
{
/// Return \em i -th particle in fParticles.
/// \return   The popped primary particle object
/// \param i  The index of primary particle to be popped

  if (i < 0 || i >= fNPrimary)
    Fatal("GetPrimaryForTracking", "Index out of range"); 
  
  return ((Ex02Particle*)fParticles->At(i))->GetParticle();
}     

//_____________________________________________________________________________
void Ex02MCStack::Print(Option_t* /*option*/) const 
{
/// Print info for all particles.

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
/// Delete contained particles, reset particles array and stack.

  // reset fStack
  fCurrentTrack = -1;
  fNPrimary = 0;
  //fParticles->Delete();
  fParticles->Clear();

  //Restore Object count 
  //To save space in the table keeping track of all referenced objects
  //we assume that our events do not address each other. We reset the 
  //object count to what it was at the beginning of the event
  TProcessID::SetObjectCount(fObjectNumber);
}       

//_____________________________________________________________________________
void  Ex02MCStack::SetCurrentTrack(Int_t track) 
{
/// Set the current track number to a given value.
/// \param  track The current track number

  fCurrentTrack = track;
}     

//_____________________________________________________________________________
void  Ex02MCStack::SetObjectNumber()
{
/// Set the current value of Root object counter into fObjectNumber.
/// Tis value will be restored in Reset.

  fObjectNumber = TProcessID::GetObjectCount();
}                         

//_____________________________________________________________________________
Int_t  Ex02MCStack::GetNtrack() const 
{
/// \return  The total number of all tracks.

  return fParticles->GetEntriesFast();
}  

//_____________________________________________________________________________
Int_t  Ex02MCStack::GetNprimary() const 
{
/// \return  The total number of primary tracks.

  return fNPrimary;
}  

//_____________________________________________________________________________
TParticle* Ex02MCStack::GetCurrentTrack() const
{
/// \return  The current track particle

  Ex02Particle* current = GetParticle(fCurrentTrack);
  
  if (current) 
    return  current->GetParticle();
  else 
    return 0;
}

//_____________________________________________________________________________
Int_t  Ex02MCStack::GetCurrentTrackNumber() const 
{
/// \return  The current track number

  return fCurrentTrack;
}  

//_____________________________________________________________________________
Int_t  Ex02MCStack::GetCurrentParentTrackNumber() const 
{
/// \return  The current track parent ID.

  Ex02Particle* current = GetParticle(fCurrentTrack);
  
  if (!current) return -1; 
  
  Ex02Particle* mother = current->GetMother();
  
  if (!mother) return -1;
    
  return  mother->GetID();
}  

//_____________________________________________________________________________
Ex02Particle*  Ex02MCStack::GetParticle(Int_t id) const
{
/// \return   The \em id -th particle in fParticles
/// \param id The index of the particle to be returned

  if (id < 0 || id >= fParticles->GetEntriesFast())
    Fatal("GetParticle", "Index out of range"); 
   
  return (Ex02Particle*)fParticles->At(id);
}


