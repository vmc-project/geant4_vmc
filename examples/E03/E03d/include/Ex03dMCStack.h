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

/// \file  E03/include/Ex03dMCStack.h
/// \brief Definition of the Ex03dMCStack class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author Radoslaw Karabowicz; GSI

#include <TVirtualMCStack.h>

#include "Ex03dParticle.h"
#include "TParticle.h"

#include <stack>
#include <vector>

/// \ingroup E03
/// \brief Implementation of the TVirtualMCStack interface
///
/// A variant of the Ex03MCStack class
/// updated for RNTuple output.
///
/// \date 07/07/2026
/// \author Radoslaw Karabowicz; GSI

class Ex03dMCStack : public TVirtualMCStack
{
 public:
  Ex03dMCStack(Int_t size);
  Ex03dMCStack();
  virtual ~Ex03dMCStack();

  // methods
  virtual void PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg, Double_t px,
    Double_t py, Double_t pz, Double_t e, Double_t vx, Double_t vy, Double_t vz,
    Double_t tof, Double_t polx, Double_t poly, Double_t polz, TMCProcess mech,
    Int_t& ntr, Double_t weight, Int_t is);
  virtual TParticle* PopNextTrack(Int_t& track);
  virtual TParticle* PopPrimaryForTracking(Int_t i);
  virtual void Print(Option_t* option = "") const;
  void Reset();

  void Register();

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
  std::stack<TParticle*> fStack; //! < The stack of particles (transient)
  std::vector<TParticle>* fParticles{
    new std::vector<TParticle>
  }; //! < The vector of particle (persistent)
  //    libc++abi: terminating due to uncaught exception of type
  //    ROOT::RException: TParticle cannot be stored natively in RNTuple
  std::vector<Ex03dParticle>* fExParticles{
    new std::vector<Ex03dParticle>
  };                         //! < The vector of particle (persistent)
  Int_t fCurrentTrack{ -1 }; /// < The current track number
  Int_t fNPrimary{ 0 };      /// < The number of primaries

  ClassDef(Ex03dMCStack, 1) // Ex03dMCStack
};

#endif // EX03_STACK_H
