// $Id: Ex02Particle.h,v 1.1 2002/06/20 11:52:35 hristov Exp $
//
// Geant4 novice ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex02Particle
// ------------------
// Extended TParticle with persistent pointers to mother and daughters
// particles
//
// by Ivana Hrivnacova, 5.4.2002


#ifndef EX02_PARTICLE_H
#define EX02_PARTICLE_H

#include <TObject.h>
#include <TRef.h>
#include <TRefArray.h>

class TParticle;

class Ex02Particle : public TObject
{
  public:
    Ex02Particle(Int_t id, TParticle* particle);
    Ex02Particle(Int_t id, TParticle* particle, Ex02Particle* mother);
    Ex02Particle();
    virtual ~Ex02Particle();     

    // methods
    void SetMother(Ex02Particle* particle);
    void AddDaughter(Ex02Particle* particle);
    void Print() const;
    void PrintDaughters() const;

    // get methods  
    Int_t         GetID() const;
    TParticle*    GetParticle() const;
    Ex02Particle* GetMother() const;
    Int_t         GetNofDaughters() const;
    Ex02Particle* GetDaughter(Int_t i) const;
    
  private:
    // data members
    Int_t       fID;
    TParticle*  fParticle;
    TRef        fMother;
    TRefArray   fDaughters;
    
    ClassDef(Ex02Particle,1) // Extended TParticle
};

#endif //Ex01_PARTICLE_H   
   

