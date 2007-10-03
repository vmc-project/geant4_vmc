// $Id: TG4ProcessMCMapPhysics.h,v 1.8 2007/05/31 10:24:32 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4ProcessMCMapPhysics
/// \brief The special builder which fills the VMC process maps
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PROCESS_MC_MAP_PHYSICS_H
#define TG4_PROCESS_MC_MAP_PHYSICS_H

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class G4Decay;

class TG4ProcessMCMapPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4ProcessMCMapPhysics(const G4String& name = "ProcessMCMap");
    TG4ProcessMCMapPhysics(G4int verboseLevel,
                           const G4String& name = "ProcessMCMap");
    virtual ~TG4ProcessMCMapPhysics();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    TG4ProcessMCMapPhysics(const TG4ProcessMCMapPhysics& right);
    TG4ProcessMCMapPhysics& operator=(const TG4ProcessMCMapPhysics& right);

    void FillMap();
};

#endif //TG4_PROCESS_MAP_PHYSICS_H

