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

/// \file  Ex03cMCStack.h
/// \brief Definition of the Ex03cMCStack class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author Benedikt Volkel, CERN

#include <TVirtualMCStack.h>

#include <stack>

class TParticle;
class TClonesArray;

/// \ingroup E03
/// \brief Implementation of the TVirtualMCStack interface
///
/// A variant of the Ex03CalorimeterSD class
/// updated for multiple engine runs.
///
/// \date 21/08/2019
/// \author Benedikt Volkel, CERN

class Ex03cMCStack : public TVirtualMCStack
{
 public:
  Ex03cMCStack(Int_t size);
  Ex03cMCStack();
  virtual ~Ex03cMCStack();

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

  ClassDef(Ex03cMCStack, 1) // Ex03cMCStack
};

#endif // EX03_STACK_H
