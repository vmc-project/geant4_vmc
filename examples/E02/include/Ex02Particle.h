// $Id: Ex02Particle.h,v 1.2 2003/02/26 13:36:44 brun Exp $
//
// Geant4 novice ExampleN02 adapted to Virtual Monte Carlo 
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
    virtual void Print(Option_t* option = "") const;
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

#endif //EX02_PARTICLE_H   
   

