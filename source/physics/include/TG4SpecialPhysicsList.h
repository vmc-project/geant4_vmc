// $Id: TG4SpecialPhysicsList.h,v 1.12 2007/05/31 10:24:32 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4SpecialPhysicsList
/// \brief The Geant4 VMC modular physics list.
///
/// The special physics processes which implement VMC functionality: 
/// special controls, special cuts, step limiter and stack popper
/// User can modify the default selection 
/// via TG4PhysicsListOptions class.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_SPECIAL_PHYSICS_LIST_H
#define TG4_SPECIAL_PHYSICS_LIST_H

#include "TG4Verbose.h"
#include "TG4PhysicsListMessenger.h"
#include "TG4PhysicsListOptions.h"

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

class TG4PhysicsConstructorStackPopper;

class TG4SpecialPhysicsList: public G4VModularPhysicsList,
                             public TG4Verbose
{
  public:
    TG4SpecialPhysicsList(const TG4PhysicsListOptions& options);
    TG4SpecialPhysicsList();
    virtual ~TG4SpecialPhysicsList();
  
    // methods
    virtual void ConstructProcess();
    virtual void SetCuts();
    virtual G4int VerboseLevel() const;
    virtual void  VerboseLevel(G4int level);
    void PrintAllProcesses() const;
    void DumpAllProcesses() const;

    // set methods
    void SetProcessActivation();
    void SetStackPopperSelection(const G4String& selection);
    
  protected:
    // data members
    //TG4PhysicsListMessenger        fMessenger; //messenger
    TG4PhysicsConstructorStackPopper*  fPhysicsConstructorStackPopper;
    TG4PhysicsListOptions          fOptions;

  private:
    TG4SpecialPhysicsList(const TG4SpecialPhysicsList& right);
    TG4SpecialPhysicsList& operator=(const TG4SpecialPhysicsList& right);

    // methods
    void Configure();
    void SetProcessActivation(G4ProcessManager* processManager,
                              G4int processId, G4bool activation);
    void SetSpecialControlsActivation();
    void SetSpecialCutsActivation();
};

#endif //TG4_SPECIAL_PHYSICS_LIST_H

