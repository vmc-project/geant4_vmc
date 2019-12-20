#ifndef EX03_STACK_H
#define EX03_STACK_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  E03/include/Ex03MCStack.h
/// \brief Definition of the Ex03MCStack class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCStack.h>

#include <stack>

class TParticle;
class TClonesArray;

/// \ingroup E03
/// \brief Implementation of the TVirtualMCStack interface
///
/// \date 06/03/2003
/// \author I. Hrivnacova; IPN, Orsay

class Ex03MCStack : public TVirtualMCStack
{
 public:
  Ex03MCStack(Int_t size);
  Ex03MCStack();
  virtual ~Ex03MCStack();

  // methods
  virtual void PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg, Double_t px,
    Double_t py, Double_t pz, Double_t e, Double_t vx, Double_t vy, Double_t vz,
    Double_t tof, Double_t polx, Double_t poly, Double_t polz, TMCProcess mech,
    Int_t& ntr, Double_t weight, Int_t is);
  virtual TParticle* PopNextTrack(Int_t& track);
  virtual TParticle* PopPrimaryForTracking(Int_t i);
  virtual void Print(Option_t* option = "") const;
  void Reset();

  // set methods
  virtual void SetCurrentTrack(Int_t track);

  // get methods
  virtual Int_t GetNtrack() const;
  virtual Int_t GetNprimary() const;
  virtual TParticle* GetCurrentTrack() const;
  virtual Int_t GetCurrentTrackNumber() const;
  virtual Int_t GetCurrentParentTrackNumber() const;
  TParticle* GetParticle(Int_t id) const;

 private:
  // data members
  std::stack<TParticle*> fStack; //!< The stack of particles (transient)
  TClonesArray* fParticles;      ///< The array of particle (persistent)
  Int_t fCurrentTrack;           ///< The current track number
  Int_t fNPrimary;               ///< The number of primaries

  ClassDef(Ex03MCStack, 1) // Ex03MCStack
};

#endif // EX03_STACK_H
