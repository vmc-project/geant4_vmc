#ifndef TG4_GFLASH_FAST_SIMULATION_MESSENGER_H
#define TG4_GFLASH_FAST_SIMULATION_MESSENGER_H 

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GflashFastSimulationMessenger.h
/// \brief Definition of the TG4GflashFastSimulationMessenger class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4GflashFastSimulation;

class G4UIcmdWithAString;

/// \ingroup physics_list
/// \brief Messenger class that defines commands for the Gflash
///        fast simulation model
///
/// Implements commands:
/// - /mcPhysics/setGflashMaterial materialName
///
/// \author I. Hrivnacova; IPN Orsay

class TG4GflashFastSimulationMessenger: public G4UImessenger
{
  public:
    TG4GflashFastSimulationMessenger(TG4GflashFastSimulation* gflashFastSimulation); 
    virtual ~TG4GflashFastSimulationMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    /// Not implemented
    TG4GflashFastSimulationMessenger();  
    /// Not implemented
    TG4GflashFastSimulationMessenger(const TG4GflashFastSimulationMessenger& right);
    /// Not implemented
    TG4GflashFastSimulationMessenger& operator=(const TG4GflashFastSimulationMessenger& right);

    //
    // data members
    
    /// associated class
    TG4GflashFastSimulation* fGflashFastSimulation;
    
    /// setExtDecayerSelection command
    G4UIcmdWithAString*    fSetGflashMaterialCmd;
};    

#endif //TG4_GFLASH_FAST_SIMULATION_MESSENGER_H
