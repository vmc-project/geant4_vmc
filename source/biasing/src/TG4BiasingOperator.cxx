//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2019 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4BiasingOperator.cxx
/// \brief Definition of the TG4BiasingOperator class
///
/// \author Alberto Ribon, CERN

#include "TG4BiasingOperator.h"
#include "TG4BiasingOperation.h"

#include "G4BiasingProcessInterface.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

TG4BiasingOperator::TG4BiasingOperator() : G4VBiasingOperator( "BiasingOperator" ) {
  fBiasingOperation = new TG4BiasingOperation( "BiasingOperation" );
}


void TG4BiasingOperator::AddParticle( G4String particleName ) {
  const G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle( particleName );
  if ( particle == 0 ) {
    G4ExceptionDescription ed;
    ed << "Particle `" << particleName << "' not found !" << G4endl;
    G4Exception( "TG4BiasingOperator::AddParticle(...)", "BiasError", JustWarning, ed );
    return;
  }
  fParticlesToBias.push_back( particle );
}


G4VBiasingOperation* TG4BiasingOperator::
ProposeFinalStateBiasingOperation( const G4Track* , const G4BiasingProcessInterface* callingProcess ) {

    // G4cout << "In TG4BiasingOperator::ProposeFinalStateTG4BiasingOperation " 
    //        << " calling process: " << callingProcess 
    //        << " wrapper process: " << callingProcess->GetWrappedProcess();
    // if ( callingProcess->GetWrappedProcess() ) {
    //   G4cout << " wrapper process name: " << callingProcess->GetWrappedProcess()->GetProcessName();
    // }      
    // G4cout << G4endl;

  // Apply the biasing operation only for inelastic processes of:
  // proton, neutron, pion+ and pion-
  if ( callingProcess  &&  callingProcess->GetWrappedProcess()  &&  
       ( callingProcess->GetWrappedProcess()->GetProcessName() == "protonInelastic"  ||
         callingProcess->GetWrappedProcess()->GetProcessName() == "neutronInelastic" || 
         callingProcess->GetWrappedProcess()->GetProcessName() == "pi+Inelastic"     || 
         callingProcess->GetWrappedProcess()->GetProcessName() == "pi-Inelastic" ) ) {
    G4cout << "In TG4BiasingOperator: Returning " << fBiasingOperation << G4endl;
    return fBiasingOperation;
  } else {  
    // G4cout << "In TG4BiasingOperator: Returning 0 " << G4endl;
    return 0; 
  }
}

