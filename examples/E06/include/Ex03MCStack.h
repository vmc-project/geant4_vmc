// $Id: $
//
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
//
// Class Ex03MCStack
// -----------------
// Implementation of the TVirtualMCStack interface
// (Taken from example E03)
//
// by Ivana Hrivnacova, 16.5.2005


#ifndef EX03_STACK_H
#define EX03_STACK_H

#include <TVirtualMCStack.h>

#include <stack>

class TParticle;
class TClonesArray;

class Ex03MCStack : public TVirtualMCStack
{
  public:
    Ex03MCStack(Int_t size);
    Ex03MCStack();
    virtual ~Ex03MCStack();     

    // methods
    virtual void  PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	              Double_t px, Double_t py, Double_t pz, Double_t e,
  		      Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		      Double_t polx, Double_t poly, Double_t polz,
		      TMCProcess mech, Int_t& ntr, Double_t weight,
		      Int_t is) ;
    virtual TParticle* PopNextTrack(Int_t& track);
    virtual TParticle* PopPrimaryForTracking(Int_t i); 
    void Print() const;   
    void Reset();   
   
    // set methods
    virtual void  SetCurrentTrack(Int_t track);                           

    // get methods
    virtual Int_t  GetNtrack() const;
    virtual Int_t  GetNprimary() const;
    virtual TParticle* GetCurrentTrack() const;   
    virtual Int_t  GetCurrentTrackNumber() const;
    virtual Int_t  GetCurrentParentTrackNumber() const;
    TParticle*     GetParticle(Int_t id) const;
    
  private:
    // data members
    std::stack<TParticle*>  fStack;    //!
    TClonesArray*           fParticles;
    Int_t                   fCurrentTrack;
    Int_t                   fNPrimary;
    
    ClassDef(Ex03MCStack,1) // Ex03MCStack
};

#endif //EX03_STACK_H   
   

