#ifndef EX03_PRIMARY_GENERATOR_H
#define EX03_PRIMARY_GENERATOR_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex03PrimaryGenerator.h 
/// \brief Definition of the Ex03PrimaryGenerator class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo: \n
/// Id: ExN03PrimaryGeneratorAction.hh,v 1.5 2002/01/09 17:24:11 ranjard Exp
/// GEANT4 tag Name: geant4-05-00
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>

class TVirtualMCStack;
class TVector3;

class Ex03DetectorConstruction;

/// \ingroup E03
/// \brief The primary generator
///
/// \date 06/03/2003
/// \author I. Hrivnacova; IPN, Orsay

class Ex03PrimaryGenerator : public TObject
{
  public:
    Ex03PrimaryGenerator(TVirtualMCStack* stack); 
    Ex03PrimaryGenerator();
    virtual ~Ex03PrimaryGenerator();

    // methods
    virtual void GeneratePrimaries(const TVector3& worldSize);

    // set methods
    void  SetIsRandom(Bool_t isRandomGenerator);
    void  SetUserParticles(Bool_t userParticles);
    void  SetNofPrimaries(Int_t nofPrimaries);
 
  private:
    // methods
    void GeneratePrimary1(const TVector3& origin);
    void GeneratePrimary2(const TVector3& origin);
 
    // data members
    TVirtualMCStack*  fStack;         ///< VMC stack
    Bool_t            fIsRandom;      ///< Switch to random generator
    Bool_t            fUserParticles; ///< Switch to user particles
    Int_t             fNofPrimaries;  ///< Number of primary particles

  ClassDef(Ex03PrimaryGenerator,1)  //Ex03PrimaryGenerator
};

// inline functions

/// Switch on/off the random generator
/// \param isRandom  If true, the position is randomized
inline void  Ex03PrimaryGenerator::SetIsRandom(Bool_t isRandom)
{ fIsRandom = isRandom; }

/// Switch on/off the user particles
/// \param userParticles  If true, the user defined particles are shooted
inline void  Ex03PrimaryGenerator::SetUserParticles(Bool_t userParticles)
{ fUserParticles = userParticles; }

/// Set the number of particles to be generated
/// \param nofPrimaries The number of particles to be generated
inline void  Ex03PrimaryGenerator::SetNofPrimaries(Int_t nofPrimaries)
{ fNofPrimaries = nofPrimaries; }

#endif //EX03_PRIMARY_GENERATOR_H

