// $Id: TG4OpticalPhysicsList.h,v 1.13 2007/08/09 13:43:34 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4OpticalPhysicsList
/// \brief The optical physics list.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_OPTICAL_PHYSICS_LIST_H
#define TG4_OPTICAL_PHYSICS_LIST_H

#include "TG4Verbose.h"

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

class TG4OpticalPhysics;

class TG4OpticalPhysicsList: public G4VModularPhysicsList,
                             public TG4Verbose
{
  public:
    TG4OpticalPhysicsList();
    virtual ~TG4OpticalPhysicsList();
  
    // methods
    virtual void ConstructProcess();
    virtual void SetCuts() {}
    virtual G4int VerboseLevel() const;
    virtual void  VerboseLevel(G4int level);

  private:
    TG4OpticalPhysicsList(const TG4OpticalPhysicsList& right);
    TG4OpticalPhysicsList& operator=(const TG4OpticalPhysicsList& right);

    // methods
    void Configure();
};

#endif //TG4_OPTICAL_PHYSICS_LIST_H

