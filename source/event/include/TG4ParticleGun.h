// $Id: TG4ParticleGun.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: event
//
// Author: I. Hrivnacova
//
// Class TG4ParticleGun
// --------------------
// Particle gun that can be interactively composed by a user.

#ifndef TG4_PARTICLE_GUN_H
#define TG4_PARTICLE_GUN_H

#include "TG4Verbose.h"
#include "TG4ParticleGunMessenger.h"

#include <G4VPrimaryGenerator.hh>
#include <globals.hh>
#include <vector>


class TG4GunParticle;

class G4Event;

class TG4ParticleGun : public G4VPrimaryGenerator,
                       public TG4Verbose
{
  typedef std::vector<TG4GunParticle*>      GunParticleVector;
  typedef GunParticleVector::iterator       GunParticleIterator;
  typedef GunParticleVector::const_iterator GunParticleConstIterator;

  public:
     TG4ParticleGun();
     TG4ParticleGun(const TG4ParticleGun &right);
     virtual ~TG4ParticleGun();

     // operators
     TG4ParticleGun& operator=(const TG4ParticleGun& right);

     // methods
     void AddParticle(TG4GunParticle* particle);
     void RemoveParticle(G4int iParticle);
     virtual void GeneratePrimaryVertex(G4Event* evt);
     void Reset();
     void List();

     // get methods
     G4int GetNofGunParticles() const;
  
  private:
     // data members
     GunParticleVector        fGunParticleVector; //vector of TG4GunParticle
     TG4ParticleGunMessenger  fMessenger;         //messenger
};

// inline methods

inline G4int TG4ParticleGun::GetNofGunParticles() const
{ return fGunParticleVector.size(); }

#endif //TG4_PARTICLE_GUN_H







