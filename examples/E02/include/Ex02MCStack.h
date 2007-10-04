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
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
//
// Class Ex02MCStack
// -----------------
// Implementation of the TVirtualMCStack interface
//
// by Ivana Hrivnacova, 5.4.2002


#ifndef EX02_STACK_H
#define EX02_STACK_H

#include "Ex02Particle.h"

#include <TVirtualMCStack.h>

#include <stack>

class Ex02MCStack : public TVirtualMCStack
{
  public:
    Ex02MCStack(Int_t size);
    Ex02MCStack();
    virtual ~Ex02MCStack();     

    // methods
    virtual void  PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	              Double_t px, Double_t py, Double_t pz, Double_t e,
  		      Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		      Double_t polx, Double_t poly, Double_t polz,
		      TMCProcess mech, Int_t& ntr, Double_t weight,
		      Int_t is) ;
    virtual TParticle* PopNextTrack(Int_t& track);
    virtual TParticle* PopPrimaryForTracking(Int_t i); 
    virtual void Print(Option_t* option = "") const; 
    void Reset();   
   
    // set methods
    virtual void  SetCurrentTrack(Int_t track);                           

    // get methods
    virtual Int_t  GetNtrack() const;
    virtual Int_t  GetNprimary() const;
    virtual TParticle* GetCurrentTrack() const;    
    virtual Int_t  GetCurrentTrackNumber() const;
    virtual Int_t  GetCurrentParentTrackNumber() const;
    Ex02Particle*  GetParticle(Int_t id) const;
    
  private:
    // data members
    std::stack<Ex02Particle*>  fStack;    //!
    TObjArray*                 fParticles;
    Int_t                      fCurrentTrack;
    Int_t                      fNPrimary;
    
    ClassDef(Ex02MCStack,1) // Ex02MCStack
};

#endif //EX02_STACK_H   
   

