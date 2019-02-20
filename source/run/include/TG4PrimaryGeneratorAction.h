#ifndef TG4_PRIMARY_GENERATOR_ACTION_H
#define TG4_PRIMARY_GENERATOR_ACTION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4PrimaryGeneratorAction.h
/// \brief Definition of the TG4PrimaryGeneratorAction class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"

#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

class TG4PrimaryGeneratorMessenger;

class G4Event;

/// \ingroup run
/// \brief Primary generator action defined via TVirtualMCStack 
/// and TVirtualMCApplication.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction,
                                  public TG4Verbose
{
  public:
    TG4PrimaryGeneratorAction();
    virtual ~TG4PrimaryGeneratorAction();

    // methods
    virtual void GeneratePrimaries(G4Event* event);

    // set methods
    void SetSkipUnknownParticles(G4bool value);

    // get methods
    G4bool GetSkipUnknownParticles() const;

  private:
    // methods
    void TransformPrimaries(G4Event* event);

    // data members
    /// Messenger
    TG4PrimaryGeneratorMessenger* fMessenger;

    /// Option to skip particles which do not exist in Geant4 
    G4bool  fSkipUnknownParticles;
};

// inline functions

/// Set the option to skip particles which do not exist in Geant4 
inline void TG4PrimaryGeneratorAction::SetSkipUnknownParticles(G4bool value)
{ fSkipUnknownParticles = value; }

/// Return the option to skip particles which do not exist in Geant4 
inline  G4bool TG4PrimaryGeneratorAction::GetSkipUnknownParticles() const
{ return fSkipUnknownParticles; }

#endif //TG4_PRIMARY_GENERATOR_ACTION_H
