#ifndef TG4_FAST_SIMULATION_PHYSICS_H
#define TG4_FAST_SIMULATION_PHYSICS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4FastSimulationPhysics.h
/// \brief Definition of the TG4FastSimulationPhysics class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"

#include <G4Threading.hh>

#include <map>

class TG4VUserFastSimulation;
class TG4ModelConfiguration;

class G4FastSimulationManagerProcess;

/// \ingroup physics_list
/// \brief Physics builder which activates selected fast simulation models.
///
/// The fast simulation models are constructed via user fast simulation class
/// derived from TG4VUserFastSimulation, created via user run conficuration class.
/// The call is triggered from TG4RunManager, after geometry and physics manager
/// have been instantiated.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4FastSimulationPhysics : public TG4VPhysicsConstructor
{
  public:
    typedef std::map<G4String, G4FastSimulationManagerProcess*> ProcessMap;

  public:
    TG4FastSimulationPhysics(const G4String& name = "FastSimulation");
    TG4FastSimulationPhysics(G4int theVerboseLevel,
                             const G4String& name = "FastSimulation");
    virtual ~TG4FastSimulationPhysics();

    // set methods
    void SetUserFastSimulation(TG4VUserFastSimulation* userFastSimulation);

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
  public: 
    virtual void ConstructProcess();

  private:
    /// Not implemented
    TG4FastSimulationPhysics(const TG4FastSimulationPhysics& right);
    /// Not implemented
    TG4FastSimulationPhysics& operator=(const TG4FastSimulationPhysics& right);

    // methods
    G4FastSimulationManagerProcess* 
      GetOrCreateFastSimulationProcess(const G4String& modelName);

    void UpdateRegions(const std::vector<TG4ModelConfiguration*>& models);
    void AddFastSimulationProcess(const std::vector<TG4ModelConfiguration*>& models);

    // static data members
    /// fast simulation processes map
    static G4ThreadLocal ProcessMap* fgProcessMap;

    // data members

    /// the user fast simulation
    TG4VUserFastSimulation* fUserFastSimulation;
};

#endif //TG4_FAST_SIMULATION_PHYSICS_H

