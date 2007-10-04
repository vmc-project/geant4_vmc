// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4SpecialCutsPhysics
/// \brief The builder for special cuts process.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_SPECIAL_CUTS_PHYSICS_H
#define TG4_SPECIAL_CUTS_PHYSICS_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class TG4SpecialCutsForGamma;
class TG4SpecialCutsForElectron;
class TG4SpecialCutsForEplus;
class TG4SpecialCutsForChargedHadron;
class TG4SpecialCutsForNeutralHadron;
class TG4SpecialCutsForMuon;
class TG4SpecialCutsForOther;


class TG4SpecialCutsPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4SpecialCutsPhysics(const G4String& name = "Special cuts");
    TG4SpecialCutsPhysics(G4int verboseLevel,
                          const G4String& name = "Special cuts");
    virtual ~TG4SpecialCutsPhysics();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    
  private:
    TG4SpecialCutsPhysics(const TG4SpecialCutsPhysics& right);
    TG4SpecialCutsPhysics& operator=(const TG4SpecialCutsPhysics& right);

    // data members
    TG4SpecialCutsForGamma*          fSpecialCutsForGamma;
    TG4SpecialCutsForElectron*       fSpecialCutsForElectron;
    TG4SpecialCutsForEplus*          fSpecialCutsForEplus;
    TG4SpecialCutsForChargedHadron*  fSpecialCutsForChargedHadron;
    TG4SpecialCutsForNeutralHadron*  fSpecialCutsForNeutralHadron;
    TG4SpecialCutsForMuon*           fSpecialCutsForMuon;
    TG4SpecialCutsForOther*          fSpecialCutsForOther;
};

#endif //TG4_SPECIAL_CUTS_PHYSICS_H

