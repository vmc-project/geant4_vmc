#ifndef TG4_GFLASH_FAST_SIMULATION_H
#define TG4_GFLASH_FAST_SIMULATION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GflashFastSimulation.h
/// \brief Definition of the TG4GflashFastSimulation class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VUserFastSimulation.h"

/// \ingroup physics_list
/// \brief Special class for definition of Gflash fast simulation model.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4GflashFastSimulationMessenger;

class GFlashShowerModel;

class TG4GflashFastSimulation : public TG4VUserFastSimulation
{
  public:
    TG4GflashFastSimulation();
    virtual ~TG4GflashFastSimulation();

    // methods
    virtual void Construct();

    // set methods
    void SetMaterialName(const G4String& materialName);

  private:
  	// data members
    TG4GflashFastSimulationMessenger* fMessenger; ///< Messenger

  	/// The name of material for shower parameterisation
  	G4String  fMaterialName; 

    /// Gflash shower model
    GFlashShowerModel* fGflashShowerModel; 
};

// inline functions

/// Set the name of material for shower parameterisation
inline void TG4GflashFastSimulation::SetMaterialName(const G4String& materialName)
{ fMaterialName = materialName; }

#endif //TG4_GFLASH_FAST_SIMULATION_H

