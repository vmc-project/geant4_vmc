#ifndef TG4_V_USER_FAST_SIMULATION_H
#define TG4_V_USER_FAST_SIMULATION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VUserFastSimulation.h
/// \brief Definition of the TG4VUserFastSimulation class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "globals.hh"

#include <vector>

class TG4ModelConfigurationManager;

class G4VFastSimulationModel;

/// \ingroup physics_list
/// \brief The abstract base class which is used to build fast simulation models
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4VUserFastSimulation
{
  public:
    TG4VUserFastSimulation();
    virtual ~TG4VUserFastSimulation();

    ///  Method to be overriden by user
    virtual void Construct() = 0;

  protected:
    // methods
    /// Method to be utilized to register each fast simulation model by name
    void SetModel(const G4String& modelName);
    /// Method to be utilized to apply fast simulation model to selected particles
    void SetModelParticles(const G4String& modelName,
                  const G4String& particles);
    /// Method to be utilized to apply fast simulation model to selected regions
    void SetModelRegions(const G4String& modelName,
                  const G4String& regions);

    /// Method to be utilized to register each fast simulation model
    void Register(G4VFastSimulationModel* fastSimulationModel);

  private:    
    /// Not implemented
    TG4VUserFastSimulation(const TG4VUserFastSimulation& right);
    /// Not implemented
    TG4VUserFastSimulation& operator=(const TG4VUserFastSimulation& right);

    // data members

    /// the fast simulation models manager
    TG4ModelConfigurationManager* fFastModelsManager;
};

#endif //TG4_V_USER_FAST_SIMULATION_H

