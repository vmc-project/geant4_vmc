#ifndef TG4_TRANSITION_RADIATION_PHYSICS_H
#define TG4_TRANSITION_RADIATION_PHYSICS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4TransitionRadiationPhysics.h
/// \brief Definition of the TG4TransitionRadiationPhysics class 
///
/// According to TransitionRadiationPhysics from Geant4
/// extended/electromagnetic/TestEm10 example.
///
/// \author I. Hrivnacova; IPN Orsay


#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

#include <vector>

class TG4RadiatorDescription;
class TG4TransitionRadiationMessenger;

class G4VXTRenergyLoss;

/// \ingroup physics_list
/// \brief The builder for transition radiation processes.
///
/// According to TransitionRadiationPhysics from Geant4
/// extended/electromagnetic/TestEm10 example.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4TransitionRadiationPhysics : public TG4VPhysicsConstructor
{
  public: 
    TG4TransitionRadiationPhysics(const G4String& name = "TransitionRadiation");
    TG4TransitionRadiationPhysics(G4int theVerboseLevel,
                                  const G4String& name = "TransitionRadiation");
    virtual ~TG4TransitionRadiationPhysics();

    // set methods
    void SetXtrModel(const G4String& name);

  protected:
    // methods
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    // methods
    G4bool CreateXTRProcess(TG4RadiatorDescription*);

    // static data members
    static G4ThreadLocal std::vector<G4VXTRenergyLoss*>* fXtrProcesses;
};

#endif  //TG4_TRANSITION_RADIATION_PHYSICS_H








