#ifndef TG4_COMPOSED_PHYSICS_LIST_H
#define TG4_COMPOSED_PHYSICS_LIST_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ComposedPhysicsList.h
/// \brief Definition of the TG4ComposedPhysicsList class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"
#include "TG4ComposedPhysicsMessenger.h"

#include <G4VUserPhysicsList.hh>
#include <globals.hh>

#include <vector>

/// \ingroup physics_list
/// \brief The Geant4 VMC physics list builder
///
/// The helper class which allows to combine more physics list
/// together.    
///
/// \author I. Hrivnacova; IPN Orsay

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

    void SetCutForGamma(G4double cut);
    void SetCutForElectron(G4double cut);
    void SetCutForPositron(G4double cut);
    void SetCutForProton(G4double cut);

    void PrintAllProcesses() const;
    void DumpAllProcesses() const;

    virtual G4int VerboseLevel() const;
    virtual void  VerboseLevel(G4int level);
    
  private:
    /// Not implemented
    TG4ComposedPhysicsList(const TG4ComposedPhysicsList& right);
    /// Not implemented
    TG4ComposedPhysicsList& operator=(const TG4ComposedPhysicsList& right);
    
    // static data members
    static const G4double  fgkDefautCut; ///< the default cut value 

    // data members
    TG4ComposedPhysicsMessenger       fMessenger;    ///< messenger
    std::vector<G4VUserPhysicsList*>  fPhysicsLists; ///< physics lists
};

#endif //TG4_COMPOSED_MODULAR_PHYSICS_H

