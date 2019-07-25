//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2019 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4BiasingOperation.cxx
/// \brief Definition of the TG4BiasingOperation class
///
/// \author Alberto Ribon, CERN

#include "TG4BiasingOperation.h"
#include "G4BiasingProcessInterface.hh"
#include "G4VParticleChange.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
// #include "G4HadronicParameters.hh"
#include "G4FTFModel.hh"
#include "G4LundStringFragmentation.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4TheoFSGenerator.hh"
#include "G4HadronicInteractionRegistry.hh"
#include "G4VPreCompoundModel.hh"
#include "G4INCLXXInterface.hh"


TG4BiasingOperation::TG4BiasingOperation( G4String name ) : G4VBiasingOperation( name ) {

  // Create the inelastic processes for  p , n , pi+ , pi- 
  fProtonInelasticProcess    = new G4ProtonInelasticProcess;
  fNeutronInelasticProcess   = new G4NeutronInelasticProcess;
  fPionPlusInelasticProcess  = new G4PionPlusInelasticProcess;
  fPionMinusInelasticProcess = new G4PionMinusInelasticProcess;

  // Set the energy ranges
  const G4double minPreco = 0.0;
  const G4double maxPreco = 2.0 * CLHEP::MeV;
  const G4double maxBERT = 12.0 * CLHEP::GeV;
  const G4double minINCLXX = 1.0 * CLHEP::MeV;
  const G4double minFTFP = 3.0 * CLHEP::GeV;
  // const G4double maxFTFP = G4HadronicParameters::Instance()->GetMaxEnergy();
  const G4double maxFTFP = 100.0 * CLHEP::TeV;

  // Create the hadronic models (to replace FTFP_BERT with "FTFP_INCLXX", 
  // keeping the same energy ranges for the transition between models).
  // Notice that it is better to create the models here from scratch,
  // instead of reusing the existing ones, because we might pick up the
  // existing ones associated to the wrong particles...
  // --- FTFP model ---
  G4FTFModel* theStringModel = new G4FTFModel;
  G4LundStringFragmentation* theLund = new G4LundStringFragmentation;
  G4ExcitedStringDecay* theStringDecay = new G4ExcitedStringDecay( theLund );
  theStringModel->SetFragmentationModel( theStringDecay );
  G4GeneratorPrecompoundInterface* thePrecoInterface = new G4GeneratorPrecompoundInterface;
  G4TheoFSGenerator* theHighEnergyModel = new G4TheoFSGenerator( "FTFP" );
  theHighEnergyModel->SetHighEnergyGenerator( theStringModel );
  theHighEnergyModel->SetTransport( thePrecoInterface );
  theHighEnergyModel->SetMinEnergy( minFTFP );
  theHighEnergyModel->SetMaxEnergy( maxFTFP );
  // Preco : create a new model to be used only for INCLXX for nucleons
  G4VPreCompoundModel* thePreCompoundModel = new G4PreCompoundModel;
  thePreCompoundModel->SetMinEnergy( minPreco );
  thePreCompoundModel->SetMaxEnergy( maxPreco );  
  // --- Preco ---
  // --- INCLXX model ---
  // The instance for nucleons:
  G4INCLXXInterface* theInclxxModel = new G4INCLXXInterface( thePreCompoundModel );
  theInclxxModel->SetMinEnergy( minINCLXX );
  theInclxxModel->SetMaxEnergy( maxBERT );  // Use the same as for Bertini
  // The instance for pions:
  G4INCLXXInterface* theInclxxModel_forPions = new G4INCLXXInterface;
  theInclxxModel_forPions->SetMinEnergy( minPreco );
  theInclxxModel_forPions->SetMaxEnergy( maxBERT );  // Use the same as for Bertini

  // Register the models
  fProtonInelasticProcess->RegisterMe( theHighEnergyModel );
  fProtonInelasticProcess->RegisterMe( theInclxxModel );
  fProtonInelasticProcess->RegisterMe( thePreCompoundModel );
  fNeutronInelasticProcess->RegisterMe( theHighEnergyModel );
  fNeutronInelasticProcess->RegisterMe( theInclxxModel );
  fNeutronInelasticProcess->RegisterMe( thePreCompoundModel );
  fPionPlusInelasticProcess->RegisterMe( theHighEnergyModel );
  fPionPlusInelasticProcess->RegisterMe( theInclxxModel_forPions );
  fPionMinusInelasticProcess->RegisterMe( theHighEnergyModel );
  fPionMinusInelasticProcess->RegisterMe( theInclxxModel_forPions );

}


TG4BiasingOperation::~TG4BiasingOperation() {}


G4VParticleChange* TG4BiasingOperation::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* , const G4Track* track, const G4Step* step, G4bool& ) {
  if ( track->GetParticleDefinition() == G4Proton::Definition() ) {
    return fProtonInelasticProcess->PostStepDoIt( *track, *step );
  } else if ( track->GetParticleDefinition() == G4Neutron::Definition() ) {
    return fNeutronInelasticProcess->PostStepDoIt( *track, *step );
  } else if ( track->GetParticleDefinition() == G4PionPlus::Definition() ) {
    return fPionPlusInelasticProcess->PostStepDoIt( *track, *step );
  } else if ( track->GetParticleDefinition() == G4PionMinus::Definition() ) {
    return fPionMinusInelasticProcess->PostStepDoIt( *track, *step );
  } else {
    G4cerr << "ERROR in TG4BiasingOperation::ApplyFinalStateBiasing : unexpected particle = " 
          << track->GetParticleDefinition()->GetParticleName() << G4endl;
    return 0;
  }
}

