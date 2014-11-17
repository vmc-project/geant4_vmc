#ifndef TG4_HADRON_PHYSICS_LIST_H
#define TG4_HADRON_PHYSICS_LIST_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4HadronPhysicsList.h
/// \brief Definition of the TG4HadronPhysicsList class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"

#include <G4VUserPhysicsList.hh>
#include <globals.hh>

class TG4OpticalPhysics;

/// \ingroup physics_list
/// \brief The hadron physics list helper class 
///
/// The hadron physics list instatiates the Geant4 hadron physics list 
/// selected by its name in the constructor
///
/// \author I. Hrivnacova; IPN Orsay

class TG4HadronPhysicsList: public G4VUserPhysicsList,
                            public TG4Verbose
{
  public:
    TG4HadronPhysicsList(const G4String& selection);
    virtual ~TG4HadronPhysicsList();
  
    // static methods
    static G4String AvailableHadronSelections();
    static G4String AvailableEMSelections();
    static G4bool   IsAvailableSelection(const G4String& selection);
 
    // methods
    virtual void ConstructParticle();
    virtual void ConstructProcess();

                  /// No cuts are set here
    virtual void SetCuts() {}

    virtual G4int VerboseLevel() const;
    virtual void  VerboseLevel(G4int level);

    // set methods
    void SetRangeCut(G4double value);
    
  protected:
    // static data members
    static const G4double  fgkDefaultCutValue; ///< default cut value

  private:
    /// Not implemented
    TG4HadronPhysicsList(const TG4HadronPhysicsList& right);
    /// Not implemented
    TG4HadronPhysicsList& operator=(const TG4HadronPhysicsList& right);

    // methods
    void Configure(const G4String& selection);
    
    // data members

    /// the physics list according to user selection
    G4VUserPhysicsList*  fPhysicsList;
};

#endif //TG4_HADRON_PHYSICS_LIST_H

