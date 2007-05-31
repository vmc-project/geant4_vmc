// $Id: TG4PhysicsConstructorGeneral.h,v 1.7 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsConstructorGeneral
/// \brief Constructor of general physics.
///
/// According to ExN04GeneralPhysics, GEANT4 tag Name: geant4-06-00
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PHYSICS_CONSTRUCTOR_GENERAL_H
#define TG4_PHYSICS_CONSTRUCTOR_GENERAL_H

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class G4Decay;

class TG4PhysicsConstructorGeneral: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorGeneral(const G4String& name = "General");
    TG4PhysicsConstructorGeneral(G4int verboseLevel,
                                 const G4String& name = "General");
    virtual ~TG4PhysicsConstructorGeneral();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
    G4Decay* fDecayProcess; // decay process

  private:
    TG4PhysicsConstructorGeneral(const TG4PhysicsConstructorGeneral& right);
    TG4PhysicsConstructorGeneral& operator=(const TG4PhysicsConstructorGeneral& right);
};

#endif //TG4_PHYSICS_CONSTRUCTOR_GENERAL_H

