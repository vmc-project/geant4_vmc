// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 ExampleN03 adapted to Virtual Monte Carlo 
//
// Id: ExN03PrimaryGeneratorAction.hh,v 1.5 2002/01/09 17:24:11 ranjard Exp
// GEANT4 tag Name: geant4-05-00
//
// by Ivana Hrivnacova, 6.3.2003

#ifndef EX03_PRIMARY_GENERATOR_H
#define EX03_PRIMARY_GENERATOR_H

#include <TVirtualMCApplication.h>

class TVirtualMCStack;
class TVector3;

class Ex03DetectorConstruction;

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
    void  SetNofPrimaries(Int_t nofPrimaries);
 
  private:
    // methods
    void GeneratePrimary(const TVector3& origin);
  
    // data members
    TVirtualMCStack*  fStack;    
    Bool_t            fIsRandom;
    Int_t             fNofPrimaries;

  ClassDef(Ex03PrimaryGenerator,1)  //Ex03PrimaryGenerator
};

// inline functions

inline void  Ex03PrimaryGenerator::SetIsRandom(Bool_t isRandom)
{ fIsRandom = isRandom; }

inline void  Ex03PrimaryGenerator::SetNofPrimaries(Int_t nofPrimaries)
{ fNofPrimaries = nofPrimaries; }

#endif //EX03_PRIMARY_GENERATOR_H

