// $Id: TG4GunParticle.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup event
//
/// \class TG4GunParticle
/// 
/// Data type class that stores properties of a gun particle.
/// Used in TG4ParticleGun.
///
/// Author: I. Hrivnacova

#ifndef TG4_GUN_PARTICLE_H
#define TG4_GUN_PARTICLE_H

#include <G4ParticleMomentum.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class G4ParticleDefinition;

class TG4GunParticle
{
  public:
    TG4GunParticle();
    TG4GunParticle(G4ParticleDefinition* particleDef,
                   G4ParticleMomentum momentum, 
                   G4ThreeVector position,
		   G4double time, 
		   G4ThreeVector polarization );
    TG4GunParticle(G4ParticleDefinition* particleDef, 
                   G4ParticleMomentum momentumDir, 
                   G4double kinEnergy,
		   G4ThreeVector position,
		   G4double time, 
                   G4ThreeVector polarization );
    TG4GunParticle(const TG4GunParticle& right);
    ~TG4GunParticle();     

    // operators
    TG4GunParticle& operator=(const TG4GunParticle& right);
    G4int operator==(const TG4GunParticle& right) const;
    G4int operator!=(const TG4GunParticle& right) const;

    // methods
    void Print() const;  

    // set methods
    void SetParticleDefinition(G4ParticleDefinition* particleDef);
    void SetMomentum(G4ParticleMomentum  momentum);
    void SetPosition(G4ThreeVector position);
    void SetTime(G4double time);
    void SetPolarization(G4ThreeVector polarization);
    void SetMomentumDirection(G4ParticleMomentum  momentumDir);
    void SetKineticEnergy(G4double kinEnergy);
      
    // get methods  
    G4ParticleDefinition* GetParticleDefinition() const;
    G4ParticleMomentum GetMomentum() const;
    G4ThreeVector GetPosition() const;
    G4double GetTime() const;
    G4ThreeVector GetPolarization() const;
    G4ParticleMomentum GetMomentumDirection() const;
    G4double GetKineticEnergy() const;
                         
  private:
    // data members
    G4ParticleDefinition*  fParticleDefinition; //G4ParticleDefinition
    G4ParticleMomentum     fParticleMomentum;   //G4ParticleMomentum
    G4ThreeVector          fPosition;           //position
    G4double               fTime;               //time
    G4ThreeVector          fPolarization;       //polarization
};

// inline methods

inline void 
TG4GunParticle::SetParticleDefinition(G4ParticleDefinition* particleDef) { 
  /// Set particle definition
  fParticleDefinition = particleDef; 
}

inline void TG4GunParticle::SetMomentum(G4ParticleMomentum  momentum) { 
  /// Set particle momentum
  fParticleMomentum = momentum; 
}

inline void TG4GunParticle::SetPosition(G4ThreeVector position) { 
  /// Set particle position
  fPosition = position; 
}

inline void TG4GunParticle::SetTime(G4double time) { 
  /// Set particle time
  fTime = time; 
}

inline void TG4GunParticle::SetPolarization(G4ThreeVector polarization) { 
  /// Set particle polarization
  fPolarization = polarization; 
}

inline void 
TG4GunParticle::SetMomentumDirection(G4ParticleMomentum  momentumDir) { 
  /// Set particle momentum direction (and keep momentum magnitude) 
  fParticleMomentum = fParticleMomentum.mag()*momentumDir.unit(); 
}

inline G4ParticleDefinition* TG4GunParticle::GetParticleDefinition() const { 
  /// Get particle definition
  return fParticleDefinition; 
}

inline G4ParticleMomentum TG4GunParticle::GetMomentum() const { 
  /// Get particle momentum
  return fParticleMomentum; 
}

inline G4ThreeVector TG4GunParticle::GetPosition() const { 
  /// Get particle position
  return fPosition; 
}

inline G4double TG4GunParticle::GetTime() const { 
  /// Get particle time
  return fTime; 
}

inline G4ThreeVector TG4GunParticle::GetPolarization() const { 
  /// Get particle polarization
  return fPolarization; 
}

inline G4ParticleMomentum TG4GunParticle::GetMomentumDirection() const { 
  /// Set particle momentum direction (and keep momentum magnitude) 
  return fParticleMomentum.unit(); 
}

#endif //TG4_GUN_PARTICLE_H   
   

