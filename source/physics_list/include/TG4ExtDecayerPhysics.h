// $Id: TG4ExtDecayerPhysics.h,v 1.8 2007/05/31 10:24:32 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4ExtDecayerPhysics
/// \brief The builder for external decayer.
///
/// The external decayer is added to all instantiated decay
/// processes
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_EXT_DECAYER_PHYSICS_H
#define TG4_EXT_DECAYER_PHYSICS_H

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class G4Decay;

class TG4ExtDecayerPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4ExtDecayerPhysics(const G4String& name = "extDecayer");
    TG4ExtDecayerPhysics(G4int verboseLevel,
                         const G4String& name = "extDecayer");
    virtual ~TG4ExtDecayerPhysics();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
    G4Decay* fDecayProcess; // decay process

  private:
    TG4ExtDecayerPhysics(const TG4ExtDecayerPhysics& right);
    TG4ExtDecayerPhysics& operator=(const TG4ExtDecayerPhysics& right);
};

#endif //TG4_EXT_DECAYER_PHYSICS_H

