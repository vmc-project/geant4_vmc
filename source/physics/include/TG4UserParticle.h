// $Id: TG4G3ParticleWSP.h 295 2007-10-04 23:17:33Z rdm $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \enum TG4UserParticle
/// \brief The class for user particle definition.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_USER_PARTICLE_H
#define TG4_USER_PARTICLE_H

#include "TG4Globals.h"

#include <G4ParticleDefinition.hh>
#include <globals.hh>

#include <TMCParticleType.h>

class TG4UserParticle : public G4ParticleDefinition
{
  public:
    TG4UserParticle(
       const G4String& name,
       TMCParticleType mcType,        
       G4double        mass,
       G4double        width,        
       G4double        charge,   
       G4int           iSpin,        
       G4int           iParity,    
       G4int           iConjugation, 
       G4int           iIsospin,   
       G4int           iIsospin3,    
       G4int           gParity,
       const G4String& pType,        
       G4int           lepton,      
       G4int           baryon,       
       G4int           encoding,
       G4bool          stable,       
       G4double        lifetime,
       G4DecayTable    *decaytable,  
       G4bool          shortlived,
       const G4String& subType ="",
       G4int           anti_encoding =0,
       G4double        magneticMoment = 0.0,
       G4double        excitation = 0.0
    );
    virtual ~TG4UserParticle();
  
    TMCParticleType  GetMCType() const;
    G4double         GetExcitationEnergy() const ; 

  private:
    TG4UserParticle();
    
    // data members
    TMCParticleType  fMCType;
    G4double         fExcitationEnergy;
};

// inline functions

inline TMCParticleType TG4UserParticle::GetMCType() const
{ return fMCType; }

inline G4double TG4UserParticle::GetExcitationEnergy() const
{ return fExcitationEnergy; }


#endif //TG4_USER_PARTICLE_H








