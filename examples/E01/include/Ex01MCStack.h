// $Id: Ex01MCStack.h,v 1.3 2003/02/26 13:36:00 brun Exp $
//
// Geant4 ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex01MCStack
// -----------------
// Implementation of the TVirtualMCStack interface
//
// by Ivana Hrivnacova, 5.4.2002


#ifndef Ex01_STACK_H
#define Ex01_STACK_H

#include "Ex01Particle.h"

#include <TVirtualMCStack.h>

#include <stack>

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
    virtual TParticle* PopNextTrack(Int_t& track);
    virtual TParticle* PopPrimaryForTracking(Int_t i);    
    
    // set methods
    virtual void  SetCurrentTrack(Int_t track);                           

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
    std::stack<Ex01Particle*>  fStack;    //!
    TObjArray*                 fParticles;
    Int_t                      fCurrentTrack;
    Int_t                      fNPrimary;
    
    ClassDef(Ex01MCStack,1) // Ex01MCStack
};

#endif //Ex01_STACK_H   
   

