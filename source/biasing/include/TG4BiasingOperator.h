#ifndef TG4_BIASING_OPERATOR_HH
#define TG4_BIASING_OPERATOR_HH

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2019 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4BiasingOperator.h
/// \brief Definition of the TG4BiasingOperator class
///
/// \author Alberto Ribon, CERN

#include "G4VBiasingOperator.hh"
#include <vector>

class G4ParticleDefinition;
class TG4BiasingOperation;


class TG4BiasingOperator : public G4VBiasingOperator {
  // When a proton, or neutron, or pion+ or pion- inelastic process occurs
  // (naturally, without any biasing) in the logical volume(s) where this
  // biasing operator has been attached to, this class uses the biasing "trick"
  // of calling FTFP+INCLXX instead of FTFP+BERT for determining the final-state.
  // Note that the weights of the produced secondaries are left to their default values, 1.0.
  public:
    TG4BiasingOperator();
    virtual ~TG4BiasingOperator() {}
    void AddParticle( G4String particleName );
    virtual G4VBiasingOperation* ProposeFinalStateBiasingOperation( const G4Track* track,
                                        const G4BiasingProcessInterface* callingProcess ) final;
    // Not used:  
    virtual G4VBiasingOperation* ProposeNonPhysicsBiasingOperation( const G4Track*,
                                    const G4BiasingProcessInterface* ) { return 0; }
    virtual G4VBiasingOperation* ProposeOccurenceBiasingOperation ( const G4Track*,
                                    const G4BiasingProcessInterface* ) { return 0; }
  private:
    std::vector< const G4ParticleDefinition* > fParticlesToBias;
    TG4BiasingOperation* fBiasingOperation;
};

#endif //TG4_BIASING_OPERATOR_HH

