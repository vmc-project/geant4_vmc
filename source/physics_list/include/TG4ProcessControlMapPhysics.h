// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4ProcessControlMapPhysics
/// \brief Special builder which fills the VMC process control map
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PROCESS_MAP_PHYSICS_H
#define TG4_PROCESS_MAP_PHYSICS_H

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class G4Decay;

class TG4ProcessControlMapPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4ProcessControlMapPhysics(const G4String& name = "ProcessMCMap");
    TG4ProcessControlMapPhysics(G4int verboseLevel,
                         const G4String& name = "ProcessMCMap");
    virtual ~TG4ProcessControlMapPhysics();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    TG4ProcessControlMapPhysics(const TG4ProcessControlMapPhysics& right);
    TG4ProcessControlMapPhysics& operator=(const TG4ProcessControlMapPhysics& right);

    void FillMap();
};

#endif //TG4_PROCESS_MAP_PHYSICS_H

