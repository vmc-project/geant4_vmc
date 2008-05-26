#ifndef TG4_PRIMARY_GENERATOR_ACTION_H
#define TG4_PRIMARY_GENERATOR_ACTION_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4PrimaryGeneratorAction.h
/// \brief Definition of the TG4PrimaryGeneratorAction class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"

#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

class G4ParticleGun;
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
    
  private:
    // methods
    void TransformPrimaries(G4Event* event);
};

#endif //TG4_PRIMARY_GENERATOR_ACTION_H


