// $Id: TG4ComposedPhysicsList.h,v 1.12 2007/05/31 10:24:32 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4ComposedPhysicsList
/// \brief The Geant4 VMC physics list builder
///
/// The helper class which allows to combine more physics list
/// together.    
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_COMPOSED_PHYSICS_LIST_H
#define TG4_COMPOSED_PHYSICS_LIST_H

#include "TG4Verbose.h"

#include <G4VUserPhysicsList.hh>
#include <globals.hh>

class TG4ComposedPhysicsList: public G4VUserPhysicsList,
                              public TG4Verbose
{
  public:
    TG4ComposedPhysicsList();
    virtual ~TG4ComposedPhysicsList();
  
    // methods  
    void AddPhysicsList(G4VUserPhysicsList* physicsList);
    
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    virtual void SetCuts();

    void PrintAllProcesses() const;
    void DumpAllProcesses() const;

    // set methods
    void SetProcessActivation();
    
  private:
    TG4ComposedPhysicsList(const TG4ComposedPhysicsList& right);
    TG4ComposedPhysicsList& operator=(const TG4ComposedPhysicsList& right);
    
    std::vector<G4VUserPhysicsList*>  fPhysicsLists;
};

#endif //TG4_COMPOSED_MODULAR_PHYSICS_H

