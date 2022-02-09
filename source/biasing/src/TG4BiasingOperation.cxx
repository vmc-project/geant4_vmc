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

#include "G4BGGNucleonInelasticXS.hh"
#include "G4BGGPionInelasticXS.hh"
#include "G4BiasingProcessInterface.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4FTFModel.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4HadronInelasticProcess.hh"
#include "G4HadronicInteractionRegistry.hh"
#include "G4HadronicParameters.hh"
#include "G4INCLXXInterface.hh"
#include "G4LundStringFragmentation.hh"
#include "G4NeutronInelasticXS.hh"
#include "G4TheoFSGenerator.hh"
#include "G4VParticleChange.hh"
#include "G4VPreCompoundModel.hh"

TG4BiasingOperation::TG4BiasingOperation(G4String name)
  : G4VBiasingOperation(name)
{

  // Create the inelastic processes for  p , n , pi+ , pi-
  fProtonInelasticProcess =
    new G4HadronInelasticProcess("protonInelastic", G4Proton::Definition());
  fNeutronInelasticProcess =
    new G4HadronInelasticProcess("neutronInelastic", G4Neutron::Definition());
  fPionPlusInelasticProcess =
    new G4HadronInelasticProcess("pi+Inelastic", G4PionPlus::Definition());
  fPionMinusInelasticProcess =
    new G4HadronInelasticProcess("pi-Inelastic", G4PionMinus::Definition());

  // Set the energy ranges
  const G4double minPreco = 0.0;
  const G4double maxPreco = 2.0 * CLHEP::MeV;
  const G4double maxBERT = 12.0 * CLHEP::GeV;
  const G4double minINCLXX = 1.0 * CLHEP::MeV;
  const G4double minFTFP = 3.0 * CLHEP::GeV;
  const G4double maxFTFP = G4HadronicParameters::Instance()->GetMaxEnergy();

  // Create the hadronic models (to replace FTFP_BERT with "FTFP_INCLXX",
  // keeping the same energy ranges for the transition between models).
  // Notice that it is better to create the models here from scratch,
  // instead of reusing the existing ones, because we might pick up the
  // existing ones associated to the wrong particles...
  // --- FTFP model ---
  G4FTFModel* theStringModel = new G4FTFModel;
  G4LundStringFragmentation* theLund = new G4LundStringFragmentation;
  G4ExcitedStringDecay* theStringDecay = new G4ExcitedStringDecay(theLund);
  theStringModel->SetFragmentationModel(theStringDecay);
  G4GeneratorPrecompoundInterface* thePrecoInterface =
    new G4GeneratorPrecompoundInterface;
  G4TheoFSGenerator* theHighEnergyModel = new G4TheoFSGenerator("FTFP");
  theHighEnergyModel->SetHighEnergyGenerator(theStringModel);
  theHighEnergyModel->SetTransport(thePrecoInterface);
  theHighEnergyModel->SetMinEnergy(minFTFP);
  theHighEnergyModel->SetMaxEnergy(maxFTFP);
  // Preco : create a new model to be used only for INCLXX for nucleons
  G4VPreCompoundModel* thePreCompoundModel = new G4PreCompoundModel;
  thePreCompoundModel->SetMinEnergy(minPreco);
  thePreCompoundModel->SetMaxEnergy(maxPreco);
  // --- Preco ---
  // --- INCLXX model ---
  // The instance for nucleons:
  G4INCLXXInterface* theInclxxModel =
    new G4INCLXXInterface(thePreCompoundModel);
  theInclxxModel->SetMinEnergy(minINCLXX);
  theInclxxModel->SetMaxEnergy(maxBERT); // Use the same as for Bertini
  // The instance for pions:
  G4INCLXXInterface* theInclxxModel_forPions = new G4INCLXXInterface;
  theInclxxModel_forPions->SetMinEnergy(minPreco);
  theInclxxModel_forPions->SetMaxEnergy(maxBERT); // Use the same as for Bertini

  // Register the models
  fProtonInelasticProcess->RegisterMe(theHighEnergyModel);
  fProtonInelasticProcess->RegisterMe(theInclxxModel);
  fProtonInelasticProcess->RegisterMe(thePreCompoundModel);
  fNeutronInelasticProcess->RegisterMe(theHighEnergyModel);
  fNeutronInelasticProcess->RegisterMe(theInclxxModel);
  fNeutronInelasticProcess->RegisterMe(thePreCompoundModel);
  fPionPlusInelasticProcess->RegisterMe(theHighEnergyModel);
  fPionPlusInelasticProcess->RegisterMe(theInclxxModel_forPions);
  fPionMinusInelasticProcess->RegisterMe(theHighEnergyModel);
  fPionMinusInelasticProcess->RegisterMe(theInclxxModel_forPions);

  // Register the cross sections: this is mandatory starting from G4 10.6
  // because the default Gheisha inelastic cross sections have been removed.
  // It is convenient to use the Gheisha inelastic cross sections here
  // because they do not require any special initialization.
  G4VCrossSectionDataSet* theProtonXSdata =
    new G4BGGNucleonInelasticXS(G4Proton::Definition());
  theProtonXSdata->BuildPhysicsTable(*(G4Proton::Definition()));
  fProtonInelasticProcess->AddDataSet(theProtonXSdata);
  G4VCrossSectionDataSet* theNeutronXSdata = new G4NeutronInelasticXS;
  theNeutronXSdata->BuildPhysicsTable(*(G4Neutron::Definition()));
  fNeutronInelasticProcess->AddDataSet(theNeutronXSdata);
  G4VCrossSectionDataSet* thePionPlusXSdata =
    new G4BGGPionInelasticXS(G4PionPlus::Definition());
  thePionPlusXSdata->BuildPhysicsTable(*(G4PionPlus::Definition()));
  fPionPlusInelasticProcess->AddDataSet(thePionPlusXSdata);
  G4VCrossSectionDataSet* thePionMinusXSdata =
    new G4BGGPionInelasticXS(G4PionMinus::Definition());
  thePionMinusXSdata->BuildPhysicsTable(*(G4PionMinus::Definition()));
  fPionMinusInelasticProcess->AddDataSet(thePionMinusXSdata);
}

TG4BiasingOperation::~TG4BiasingOperation() {}

G4VParticleChange* TG4BiasingOperation::ApplyFinalStateBiasing(
  const G4BiasingProcessInterface*, const G4Track* track, const G4Step* step,
  G4bool&)
{
  if (track->GetParticleDefinition() == G4Proton::Definition()) {
    return fProtonInelasticProcess->PostStepDoIt(*track, *step);
  }
  else if (track->GetParticleDefinition() == G4Neutron::Definition()) {
    return fNeutronInelasticProcess->PostStepDoIt(*track, *step);
  }
  else if (track->GetParticleDefinition() == G4PionPlus::Definition()) {
    return fPionPlusInelasticProcess->PostStepDoIt(*track, *step);
  }
  else if (track->GetParticleDefinition() == G4PionMinus::Definition()) {
    return fPionMinusInelasticProcess->PostStepDoIt(*track, *step);
  }
  else {
    G4cerr << "ERROR in TG4BiasingOperation::ApplyFinalStateBiasing : "
              "unexpected particle = "
           << track->GetParticleDefinition()->GetParticleName() << G4endl;
    return 0;
  }
}
