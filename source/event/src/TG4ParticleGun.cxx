// $Id: TG4ParticleGun.cxx,v 1.7.6.1 2002/06/11 12:29:41 hristov Exp $
// Category: event
//
// Author: I. Hrivnacova
//
// Class TG4ParticleGun
// --------------------
// See the class description in the header file.

#include "TG4ParticleGun.h"
#include "TG4ParticleGunMessenger.h"
#include "TG4GunParticle.h"
#include "TG4Globals.h"

#include <G4ParticleDefinition.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4Event.hh>

//_____________________________________________________________________________
TG4ParticleGun::TG4ParticleGun() 
  : G4VPrimaryGenerator(),
    TG4Verbose("particleGun"),
    fMessenger(this) {
//
}

//_____________________________________________________________________________
TG4ParticleGun::TG4ParticleGun(const TG4ParticleGun& right)
  : G4VPrimaryGenerator(right),
    TG4Verbose("particleGun"),
    fMessenger(this)
{
  // copy stuff
  *this = right;
}

//_____________________________________________________________________________
TG4ParticleGun::~TG4ParticleGun() {
//
  // delete gun particles
  Reset();
}

// operators

//_____________________________________________________________________________
TG4ParticleGun& TG4ParticleGun::operator=(const TG4ParticleGun& right)
{
  // check assignement to self
  if (this == &right) return *this;

  // base class assignment
  this->G4VPrimaryGenerator::operator=(right);

  // delete gun particles
  Reset();

  // copy  gun particles
  GunParticleConstIterator it;
  for (it = (right.fGunParticleVector).begin();
       it != (right.fGunParticleVector).end(); it++)
    fGunParticleVector.push_back(new TG4GunParticle(**it));

  return *this;  
}
  
// public methods

//_____________________________________________________________________________
void TG4ParticleGun::AddParticle(TG4GunParticle* particle)
{ 
// Adds particle.
// ---

  fGunParticleVector.push_back(particle); 
}

//_____________________________________________________________________________
void TG4ParticleGun::RemoveParticle(G4int iParticle)
{ 
// Removes particle.
// ---

  GunParticleIterator it = fGunParticleVector.begin();
  it += iParticle;
  //advance(it,iParticle);
   
  delete *it;
  fGunParticleVector.erase(it); 
}

//_____________________________________________________________________________
void TG4ParticleGun::GeneratePrimaryVertex(G4Event* event)
{
// Generates primary vertices.
// ---

  G4PrimaryVertex* previousVertex = 0;
  G4ThreeVector previousPosition = G4ThreeVector(); 
  G4double previousTime = 0.; 

  GunParticleIterator it;
  for (it = fGunParticleVector.begin(); it != fGunParticleVector.end(); it++) {    

    TG4GunParticle* particle = *it;

    G4ParticleDefinition* particleDefinition
      = particle->GetParticleDefinition();
    if (particleDefinition==0) {
      TG4Globals::Exception(
        "TG4ParticleGun::GeneratePrimaryVertex: Unknown particle definition.");
    }	

    G4ThreeVector position = particle->GetPosition(); 
    G4double time = particle->GetTime(); 
    G4PrimaryVertex* vertex;
    if (it == fGunParticleVector.begin() || 
        position != previousPosition || time != previousTime ) {   
      // create a new vertex 
      // in case position and time of gun particle are different from 
      // previous values
      // (vertex objects are destroyed in G4EventManager::ProcessOneEvent()
      // when event is deleted)  
      vertex = new G4PrimaryVertex(position, time);
      event->AddPrimaryVertex(vertex);

      previousVertex = vertex;
      previousPosition = position;
      previousTime = time;
    }
    else 
      vertex = previousVertex;

    // create a primary particle and add it to the vertex
    // (primaryParticle objects are destroyed in G4EventManager::ProcessOneEvent()
    // when event and then vertex is deleted)
    G4double px = particle->GetMomentum().x();
    G4double py = particle->GetMomentum().y();
    G4double pz = particle->GetMomentum().z();
    G4PrimaryParticle* primaryParticle 
      = new G4PrimaryParticle(particleDefinition, px, py, pz);
      
    G4double mass =  particleDefinition->GetPDGMass();
    primaryParticle->SetMass(mass);
    primaryParticle->SetPolarization(particle->GetPolarization().x(),
                                     particle->GetPolarization().y(),
				     particle->GetPolarization().z());    
    
    vertex->SetPrimary(primaryParticle);
  }
  
  if (VerboseLevel() > 0) {
    G4cout << "TG4ParticleGun::GeneratePrimaryVertex:" << G4endl;
    G4cout << "   " 
           << event->GetNumberOfPrimaryVertex()  << " of primary vertices,"
           << "   " << fGunParticleVector.size() << " of primary particles " 
  	   << G4endl;  
  }	   

  // delete gun particles
  Reset();
}

//_____________________________________________________________________________
void TG4ParticleGun::Reset()
{ 
// Resets the particle gun.
// ---

  GunParticleIterator it;
  for (it = fGunParticleVector.begin(); it != fGunParticleVector.end(); it++)
    delete *it;

  fGunParticleVector.clear(); 
}

//_____________________________________________________________________________
void TG4ParticleGun::List()
{
// Lists the particle gun.
// ---

  G4cout << "Particle Gun: " << G4endl;

  if (fGunParticleVector.size() == 0) 
    G4cout << "   No particles are defined." << G4endl; 
  else {
    G4int i = 0;
    GunParticleIterator it;
    for (it = fGunParticleVector.begin(); 
         it != fGunParticleVector.end(); it++) {    
	 
      G4cout << i++ << " th particle properties: " << G4endl;
      G4cout << "============================" << G4endl;
      (*it)->Print();
    }
  }
}

