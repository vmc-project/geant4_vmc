#ifndef Ex01_STACK_H
#define Ex01_STACK_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file  Ex01MCStack.h
/// \brief Definition of the Ex01MCStack class 
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex01Particle.h"

#include <TVirtualMCStack.h>

#include <stack>

/// \ingroup E01
/// \brief Implementation of the TVirtualMCStack interface
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex01MCStack : public TVirtualMCStack
{
  public:
    Ex01MCStack(Int_t size);
    Ex01MCStack();
    virtual ~Ex01MCStack();     

    // methods
    virtual void  PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	              Double_t px, Double_t py, Double_t pz, Double_t e,
  		      Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		      Double_t polx, Double_t poly, Double_t polz,
		      TMCProcess mech, Int_t& ntr, Double_t weight,
		      Int_t is) ;
    virtual TParticle* PopNextTrack(Int_t& itrack);
    virtual TParticle* PopPrimaryForTracking(Int_t i);    
    
    // set methods
    virtual void  SetCurrentTrack(Int_t itrack);                           

    // get methods
    virtual Int_t  GetNtrack() const;
    virtual Int_t  GetNprimary() const;
    virtual TParticle* GetCurrentTrack() const;    
    virtual Int_t  GetCurrentTrackNumber() const;
    virtual Int_t  GetCurrentParentTrackNumber() const;
    
  private:
    // methods
    Ex01Particle* GetParticle(Int_t id) const;
  
    // data members
    std::stack<Ex01Particle*>  fStack;        //!< The stack of particles (transient)
    TObjArray*                 fParticles;    ///< The array of particle (persistent)
    Int_t                      fCurrentTrack; ///< The current track number
    Int_t                      fNPrimary;     ///< The number of primaries
    
    ClassDef(Ex01MCStack,1) // Ex01MCStack
};

#endif //Ex01_STACK_H   
   

