// $Id: TG4ComposedPhysicsList.h,v 1.1 2007/08/09 13:43:34 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
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
#include "TG4ComposedPhysicsMessenger.h"

#include <G4VUserPhysicsList.hh>
#include <globals.hh>

#include <vector>

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

    void PrintAllProcesses() const;
    void DumpAllProcesses() const;

    virtual G4int VerboseLevel() const;
    virtual void  VerboseLevel(G4int level);
    
  private:
    TG4ComposedPhysicsList(const TG4ComposedPhysicsList& right);
    TG4ComposedPhysicsList& operator=(const TG4ComposedPhysicsList& right);
    
    // static data members
    static const G4double  fgkDefautCut;  

    // data members
    TG4ComposedPhysicsMessenger       fMessenger;    // messenger
    std::vector<G4VUserPhysicsList*>  fPhysicsLists; // physics lists
    G4double  fCutForGamma;    // cut for gamma
    G4double  fCutForElectron; // cut for electron 
    G4double  fCutForPositron; // cut for positron 
};

#endif //TG4_COMPOSED_MODULAR_PHYSICS_H

