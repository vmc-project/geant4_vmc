// $Id: TG4GunParticle.cxx,v 1.2 2004/11/10 11:39:27 brun Exp $
// Category: event
//
// Class TG4GunParticle
// --------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4GunParticle.h"

#include <G4ParticleDefinition.hh>

//_____________________________________________________________________________
TG4GunParticle::TG4GunParticle()
  : fParticleDefinition(0),
    fParticleMomentum(G4ThreeVector(0., 0., 1.)),
    fPosition(G4ThreeVector()),
    fTime(0),
    fPolarization(G4ThreeVector())
{
//
}

//_____________________________________________________________________________
TG4GunParticle::TG4GunParticle(G4ParticleDefinition* particleDef,
                               G4ParticleMomentum momentum, 
                               G4ThreeVector position, 
                               G4double time, 
                               G4ThreeVector polarization )
  : fParticleDefinition(particleDef),
    fParticleMomentum(momentum),
    fPosition(position),
    fTime(time),
    fPolarization(polarization)
{
//
}

//_____________________________________________________________________________
TG4GunParticle::TG4GunParticle(G4ParticleDefinition* particleDef, 
                               G4ParticleMomentum momentumDir,
                               G4double kinEnergy,
                               G4ThreeVector position,
                               G4double time, 
                               G4ThreeVector polarization )
  : fParticleDefinition(particleDef),
    fParticleMomentum(),
    fPosition(position),
    fTime(time),
    fPolarization(polarization)
{
//   
  G4double mass = fParticleDefinition->GetPDGMass();
  G4double energy = kinEnergy + mass;
  G4double pmag = sqrt(energy*energy - mass*mass);
  fParticleMomentum = pmag*momentumDir;
}

//_____________________________________________________________________________
TG4GunParticle::TG4GunParticle(const TG4GunParticle& right)   
  : fParticleDefinition(right.fParticleDefinition),
    fParticleMomentum(right.fParticleMomentum),
    fPosition(right.fPosition), 
    fTime(right.fTime),
    fPolarization(right.fPolarization) 
{
//
}

//_____________________________________________________________________________
TG4GunParticle::~TG4GunParticle() {
//
}

//
// operators
//

//_____________________________________________________________________________
TG4GunParticle& TG4GunParticle::operator=(const TG4GunParticle& right)
{    
  // check assignement to self
  if (this == &right) return *this;

  fParticleDefinition = right.fParticleDefinition;
  fParticleMomentum = right.fParticleMomentum;
  fPosition = right.fPosition; 
  fTime = right.fTime;
  fPolarization = right.fPolarization; 
  
  return *this;
}

//_____________________________________________________________________________
G4int TG4GunParticle::operator==(const TG4GunParticle& right) const 
{
//    
  G4int returnValue = 0;
  if (fParticleDefinition == right.fParticleDefinition 
      && fParticleMomentum == right.fParticleMomentum 
      && fPosition == right.fPosition 
      && fTime == right.fTime 
      && fPolarization == right.fPolarization)  returnValue = 1; 

  return returnValue;  
}

//_____________________________________________________________________________
G4int TG4GunParticle::operator!=(const TG4GunParticle& right) const
{
//    
  G4int returnValue = 1;
  if (*this == right) returnValue = 0; 
  
  return returnValue;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4GunParticle::Print() const
{
/// Print particle properties.

  G4cout << "    particle name: ";
  G4cout << fParticleDefinition->GetParticleName() << G4endl;
  G4cout << "    particle momentum: ";
  G4cout << fParticleMomentum << G4endl;
  G4cout << "    particle position: ";
  G4cout << fPosition << " mm" << G4endl;
  G4cout << "    particle time: ";
  G4cout << fTime     << " s" << G4endl;
  G4cout << "    particle polarization: ";
  G4cout << fPolarization << G4endl;
}  

//_____________________________________________________________________________
void TG4GunParticle::SetKineticEnergy(G4double kinEnergy)
{
/// Set kinetic energy.

  G4double mass = fParticleDefinition->GetPDGMass();
  G4double energy = kinEnergy + mass;
  G4double pmag = sqrt(energy*energy - mass*mass);
  fParticleMomentum.setMag(pmag); 
}

//_____________________________________________________________________________
G4double TG4GunParticle::GetKineticEnergy() const
{
/// Get kinetic energy.

  G4double mass = fParticleDefinition->GetPDGMass();
  G4double energy 
   = sqrt(fParticleMomentum.mag()*fParticleMomentum.mag() + mass*mass);
  return energy - mass;
}
