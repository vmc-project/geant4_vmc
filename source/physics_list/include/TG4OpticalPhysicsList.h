#ifndef TG4_OPTICAL_PHYSICS_LIST_H
#define TG4_OPTICAL_PHYSICS_LIST_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4OpticalPhysicsList.h
/// \brief Definition of the TG4OpticalPhysicsList class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

class TG4OpticalPhysics;

/// \ingroup physics_list
/// \brief The optical physics list.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4OpticalPhysicsList: public G4VModularPhysicsList,
                             public TG4Verbose
{
  public:
    TG4OpticalPhysicsList();
    virtual ~TG4OpticalPhysicsList();
  
    // methods
    virtual void ConstructProcess();
                  /// No cuts are set here
    virtual void SetCuts() {}

    virtual G4int VerboseLevel() const;
    virtual void  VerboseLevel(G4int level);

  private:
    /// Not implemented
    TG4OpticalPhysicsList(const TG4OpticalPhysicsList& right);
    /// Not implemented
    TG4OpticalPhysicsList& operator=(const TG4OpticalPhysicsList& right);

    // methods
    void Configure();
};

#endif //TG4_OPTICAL_PHYSICS_LIST_H

