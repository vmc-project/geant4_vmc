#ifndef EX02_PARTICLE_H
#define EX02_PARTICLE_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Ex02Particle.h
/// \brief Definition of the Ex02Particle class
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TRef.h>
#include <TRefArray.h>

class TParticle;

/// \ingroup E02
/// \brief Extended TParticle with pointers to mother and daughter
/// particles
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

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
    Int_t       fID;        ///< The particle Id
    TParticle*  fParticle;  ///< The particle definition
    TRef        fMother;    ///< The particle mother 
    TRefArray   fDaughters; ///< The particle daughters
    
    ClassDef(Ex02Particle,1) // Extended TParticle
};

#endif //EX02_PARTICLE_H   
   

