// $Id: Ex01Particle.h,v 1.1 2002/06/20 11:52:02 hristov Exp $
//
// Geant4 novice ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex01Particle
// ------------------
// Extended TParticle with pointers to mother and daughters
// particles
//
// by Ivana Hrivnacova, 5.4.2002


#ifndef Ex01_PARTICLE_H
#define Ex01_PARTICLE_H

#include <TObject.h>

class TParticle;

class Ex01Particle : public TObject
{
  public:
    Ex01Particle(Int_t id, TParticle* particle);
    Ex01Particle(Int_t id, TParticle* particle, Ex01Particle* mother);
    Ex01Particle();
    virtual ~Ex01Particle();     

    // methods
    void SetMother(Ex01Particle* particle);
    void AddDaughter(Ex01Particle* particle);

    // get methods  
    Int_t         GetID() const;
    TParticle*    GetParticle() const;
    Ex01Particle* GetMother() const;
    Int_t         GetNofDaughters() const;
    Ex01Particle* GetDaughter(Int_t i) const;
    
  private:
    // data members
    Int_t         fID;
    TParticle*    fParticle;
    Ex01Particle* fMother;
    TObjArray*    fDaughters;
    
    ClassDef(Ex01Particle,1) // Extended TParticle
};

#endif //Ex01_PARTICLE_H   
   

