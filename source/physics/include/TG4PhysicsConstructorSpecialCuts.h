// $Id: TG4PhysicsConstructorSpecialCuts.h,v 1.2 2004/11/10 11:39:28 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorSpecialCuts
/// 
/// Constructor of special cuts.
///
/// Author: I. Hrivnacova

#ifndef TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CUTS_H
#define TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CUTS_H

#include "TG4VPhysicsConstructor.h"
#include "TG4SpecialCuts.h"

#include <globals.hh>

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
    // data members
    TG4SpecialCutsForGamma          fSpecialCutsForGamma;
    TG4SpecialCutsForElectron       fSpecialCutsForElectron;
    TG4SpecialCutsForEplus          fSpecialCutsForEplus;
    TG4SpecialCutsForChargedHadron  fSpecialCutsForChargedHadron;
    TG4SpecialCutsForNeutralHadron  fSpecialCutsForNeutralHadron;
    TG4SpecialCutsForMuon           fSpecialCutsForMuon;
    TG4SpecialCutsForOther          fSpecialCutsForOther;
};

#endif //TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CUTS_H

