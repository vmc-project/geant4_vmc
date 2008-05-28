#ifndef TG4_SPECIAL_CUTS_PHYSICS_H
#define TG4_SPECIAL_CUTS_PHYSICS_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SpecialCutsPhysics.h
/// \brief Definition of the TG4SpecialCutsPhysics class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class TG4SpecialCutsForGamma;
class TG4SpecialCutsForElectron;
class TG4SpecialCutsForEplus;
class TG4SpecialCutsForChargedHadron;
class TG4SpecialCutsForNeutralHadron;
class TG4SpecialCutsForMuon;
class TG4SpecialCutsForOther;

/// \ingroup physics_list
/// \brief The builder for special cuts process.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4SpecialCutsPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4SpecialCutsPhysics(const G4String& name = "SpecialCuts");
    TG4SpecialCutsPhysics(G4int verboseLevel,
                          const G4String& name = "SpecialCuts");
    virtual ~TG4SpecialCutsPhysics();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    
  private:
    /// Not implemented
    TG4SpecialCutsPhysics(const TG4SpecialCutsPhysics& right);
    /// Not implemented
    TG4SpecialCutsPhysics& operator=(const TG4SpecialCutsPhysics& right);

    //
    // data members
    
    /// the special cuts process for gamma
    TG4SpecialCutsForGamma*          fSpecialCutsForGamma;
    
    /// the special cuts process for e-
    TG4SpecialCutsForElectron*       fSpecialCutsForElectron;
    
    /// the special cuts process for e+ 
    TG4SpecialCutsForEplus*          fSpecialCutsForEplus;
    
    /// the special cuts process for charged hadron
    TG4SpecialCutsForChargedHadron*  fSpecialCutsForChargedHadron;
    
    /// the special cuts process for neutral hadron
    TG4SpecialCutsForNeutralHadron*  fSpecialCutsForNeutralHadron;
    
    /// the special cuts process for muon
    TG4SpecialCutsForMuon*           fSpecialCutsForMuon; 
    
    /// the special cuts process for other particles
    TG4SpecialCutsForOther*          fSpecialCutsForOther;
};

#endif //TG4_SPECIAL_CUTS_PHYSICS_H

