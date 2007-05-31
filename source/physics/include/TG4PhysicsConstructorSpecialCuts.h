// $Id: TG4PhysicsConstructorSpecialCuts.h,v 1.6 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsConstructorSpecialCuts
/// \brief Constructor of special cuts.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CUTS_H
#define TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CUTS_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class TG4SpecialCutsForGamma;
class TG4SpecialCutsForElectron;
class TG4SpecialCutsForEplus;
class TG4SpecialCutsForChargedHadron;
class TG4SpecialCutsForNeutralHadron;
class TG4SpecialCutsForMuon;
class TG4SpecialCutsForOther;


class TG4PhysicsConstructorSpecialCuts: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorSpecialCuts(const G4String& name = "Special cuts");
    TG4PhysicsConstructorSpecialCuts(G4int verboseLevel,
                                     const G4String& name = "Special cuts");
    virtual ~TG4PhysicsConstructorSpecialCuts();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    
  private:
    TG4PhysicsConstructorSpecialCuts(const TG4PhysicsConstructorSpecialCuts& right);
    TG4PhysicsConstructorSpecialCuts& operator=(const TG4PhysicsConstructorSpecialCuts& right);

    // data members
    TG4SpecialCutsForGamma*          fSpecialCutsForGamma;
    TG4SpecialCutsForElectron*       fSpecialCutsForElectron;
    TG4SpecialCutsForEplus*          fSpecialCutsForEplus;
    TG4SpecialCutsForChargedHadron*  fSpecialCutsForChargedHadron;
    TG4SpecialCutsForNeutralHadron*  fSpecialCutsForNeutralHadron;
    TG4SpecialCutsForMuon*           fSpecialCutsForMuon;
    TG4SpecialCutsForOther*          fSpecialCutsForOther;
};

#endif //TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CUTS_H

