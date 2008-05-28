#ifndef TG4_EXT_DECAYER_PHYSICS_H
#define TG4_EXT_DECAYER_PHYSICS_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ExtDecayerPhysics.h 
/// \brief Definition of the TG4ExtDecayerPhysics class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class G4Decay;

/// \ingroup physics_list
/// \brief The builder for external decayer.
///
/// The external decayer is added to all instantiated decay
/// processes
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ExtDecayerPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4ExtDecayerPhysics(const G4String& name = "ExtDecayer");
    TG4ExtDecayerPhysics(G4int verboseLevel,
                         const G4String& name = "ExtDecayer");
    virtual ~TG4ExtDecayerPhysics();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
    G4Decay* fDecayProcess; ///< decay process

  private:
    /// Not implemented
    TG4ExtDecayerPhysics(const TG4ExtDecayerPhysics& right);
    /// Not implemented
    TG4ExtDecayerPhysics& operator=(const TG4ExtDecayerPhysics& right);
};

#endif //TG4_EXT_DECAYER_PHYSICS_H

