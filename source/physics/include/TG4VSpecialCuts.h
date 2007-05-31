// $Id: TG4VSpecialCuts.h,v 1.5 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4VSpecialCuts
/// \brief Abstract base class for a special process that activates 
/// kinetic energy cuts.                                                     
///
/// The pure virtual functions GetMinEkine have to be implemented
/// by derived classes specific for each particle type
/// (see TG4G3ParticleWSP.h).
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_V_SPECIAL_CUTS_H
#define TG4_V_SPECIAL_CUTS_H

#include <G4VProcess.hh>

class TG4G3CutVector;
class TG4Limits;

class G4Track;
class G4LossTableManager;

class TG4VSpecialCuts: public G4VProcess
{
  public:
    TG4VSpecialCuts(const G4String& processName);
    virtual ~TG4VSpecialCuts();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const = 0;
    
    virtual G4double PostStepGetPhysicalInteractionLength(
                         const G4Track& track, G4double previousStepSize,
                         G4ForceCondition* condition);

    virtual G4VParticleChange* PostStepDoIt(const G4Track& track, 
                         const G4Step& step);
                            
    virtual G4double AlongStepGetPhysicalInteractionLength(
                         const G4Track&, G4double, G4double, G4double&,
                         G4GPILSelection*)
                         { return -1.0; }

    virtual G4VParticleChange* AlongStepDoIt(const G4Track&, const G4Step&)
                         { return 0; }

    virtual G4double AtRestGetPhysicalInteractionLength(const G4Track&,
                         G4ForceCondition* )
                         { return -1.0; }
                            
    virtual G4VParticleChange* AtRestDoIt(
                         const G4Track&, const G4Step&)
                         { return 0; }

  private:
    TG4VSpecialCuts();                   
    TG4VSpecialCuts(const TG4VSpecialCuts& right);
    TG4VSpecialCuts& operator = (const TG4VSpecialCuts& right);

    G4LossTableManager*  fLossTableManager;

};

#endif //TG4_SPECIAL_CUTS_H



