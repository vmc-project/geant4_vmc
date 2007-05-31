// $Id: TG4VPhysicsConstructor.h,v 1.5 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4VPhysicsConstructor
/// \brief Abstract base class for physics constructors with verbose.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_V_PHYSICS_CONSTRUCTOR_H
#define TG4_V_PHYSICS_CONSTRUCTOR_H

#include "TG4Verbose.h"

#include <G4VPhysicsConstructor.hh>
#include <globals.hh>

class TG4VPhysicsConstructor: public G4VPhysicsConstructor,
                              public TG4Verbose
{
  public:
    TG4VPhysicsConstructor(const G4String& name);
    TG4VPhysicsConstructor(const G4String& name, G4int verboseLevel);
    virtual ~TG4VPhysicsConstructor();

    // methods
          // construct particle and physics
    virtual void ConstructParticle() = 0;
    virtual void ConstructProcess() = 0;

  protected:
    // overridden verbose methods
    virtual void  VerboseLevel(G4int level);
    virtual G4int VerboseLevel() const;

  private:
    TG4VPhysicsConstructor();
    TG4VPhysicsConstructor(const TG4VPhysicsConstructor& right);

};

#endif //TG4_V_PHYSICS_CONSTRUCTOR_H

