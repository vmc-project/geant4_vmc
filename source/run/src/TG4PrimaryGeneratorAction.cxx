// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4PrimaryGeneratorAction.cxx
/// \brief Implementation of the TG4PrimaryGeneratorAction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4PrimaryGeneratorAction.h"
#include "TG4ParticlesManager.h"
#include "TG4TrackManager.h"
#include "TG4StateManager.h"
#include "TG4UserIon.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"

#include <G4Event.hh>
#include <G4ParticleTable.hh>
#include <G4IonTable.hh>
#include <G4ParticleDefinition.hh>

#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMCStack.h>
#include <TParticle.h>

// Moved after Root includes to avoid shadowed variables 
// generated from short units names
#include <G4SystemOfUnits.hh>

//_____________________________________________________________________________
TG4PrimaryGeneratorAction::TG4PrimaryGeneratorAction()
  : TG4Verbose("primaryGeneratorAction") 
{
/// Default constructor
}

//_____________________________________________________________________________
TG4PrimaryGeneratorAction::~TG4PrimaryGeneratorAction() 
{
/// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4PrimaryGeneratorAction::TransformPrimaries(G4Event* event)
{
/// Create a new G4PrimaryVertex objects for each TParticle
/// in the VMC stack.
  
  TVirtualMCStack* stack = gMC->GetStack();  
  if (!stack) {
    TG4Globals::Exception(
      "TG4PrimaryGeneratorAction", "TransformPrimaries",
      "No VMC stack is defined.");
  }  
    
  G4int nofParticles = stack->GetNtrack();
  if (nofParticles <= 0) {
    TG4Globals::Exception(
      "TG4PrimaryGeneratorAction", "TransformPrimaries",
      "No primary particles found on the stack.");
  }  

  if (VerboseLevel() > 1)
    G4cout << "TG4PrimaryGeneratorAction::TransformPrimaries: " 
           << nofParticles << " particles" << G4endl; 
     

  TG4ParticlesManager* particlesManager = TG4ParticlesManager::Instance();

  G4PrimaryVertex* previousVertex = 0;
  G4ThreeVector previousPosition = G4ThreeVector(); 
  G4double previousTime = 0.; 
  
  for (G4int i=0; i<nofParticles; i++) {    
  
    // get the particle from the stack
    TParticle* particle = stack->PopPrimaryForTracking(i);
    
    if (particle) {
      // only particles that didn't die (decay) in primary generator
      // will be transformed to G4 objects   
      
      // Pass this particle Id (in the VMC stack) to Track manager
      TG4TrackManager::Instance()->AddPrimaryParticleId(i);

      // Get particle definition from TG4ParticlesManager
      //
      G4ParticleDefinition* particleDefinition
        = particlesManager->GetParticleDefinition(particle, false);

      if (!particleDefinition) { 
        TString text = "pdgEncoding=";
        text += particle->GetPdgCode();
        TG4Globals::Exception(
         "TG4PrimaryGeneratorAction", "TransformPrimaries",
         "G4ParticleTable::FindParticle() failed for " +
         TString(particle->GetName()) + "  "  + text + "."); 
      }        

      // Get/Create vertex
      G4ThreeVector position 
        = particlesManager->GetParticlePosition(particle);
      G4double time = particle->T()*TG4G3Units::Time(); 
      G4PrimaryVertex* vertex;
      if ( i==0 || previousVertex ==0 || 
           position != previousPosition || time != previousTime ) {
        // Create a new vertex 
        // in case position and time of gun particle are different from 
        // previous values
        // (vertex objects are destroyed in G4EventManager::ProcessOneEvent()
        // when event is deleted)  
        vertex = new G4PrimaryVertex(position, time);
        event->AddPrimaryVertex(vertex);

        previousVertex = vertex;
        previousPosition = position;
        previousTime = time;
      }
      else 
        vertex = previousVertex;

      // Create a primary particle and add it to the vertex
      // (primaryParticle objects are destroyed in G4EventManager::ProcessOneEvent()
      // when event and then vertex is deleted)
      G4ThreeVector momentum 
        = particlesManager->GetParticleMomentum(particle);
      G4PrimaryParticle* primaryParticle 
        = new G4PrimaryParticle(particleDefinition, 
                                momentum.x(), momentum.y(), momentum.z());
 
      // Set mass
      primaryParticle->SetMass(particleDefinition->GetPDGMass());

      // Set charge
      G4double charge = particleDefinition->GetPDGCharge();
      if ( G4IonTable::IsIon(particleDefinition) &&
           particleDefinition->GetParticleName() != "proton" ) {
        // Get dynamic charge defined by user
        TG4UserIon* userIon = particlesManager->GetUserIon(particle->GetName(), false);
        if ( userIon ) charge = userIon->GetQ() * eplus;
      }       
      primaryParticle->SetCharge(charge);
      
      // Set polarization
      TVector3 polarization;
      particle->GetPolarisation(polarization);
      primaryParticle
        ->SetPolarization(polarization.X(), polarization.Y(), polarization.Z()); 
        
      // Set weight
      G4double weight =  particle->GetWeight();
      primaryParticle->SetWeight(weight); 
        
      // Add primary particle to the vertex
      vertex->SetPrimary(primaryParticle);

      // Verbose
      if (VerboseLevel() > 1) {
        G4cout << i << "th primary particle: " << G4endl;
        primaryParticle->Print();
      } 
    }   
  }
}

//
// public methods
//

//_____________________________________________________________________________
void TG4PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
/// Generate primary particles by the selected generator.

  // Begin of event
  TG4StateManager::Instance()->SetNewState(kInEvent);
  TVirtualMCApplication::Instance()->BeginEvent();

  // Generate primaries and fill the VMC stack
  TVirtualMCApplication::Instance()->GeneratePrimaries();
  
  // Transform Root particle objects to G4 objects
  TransformPrimaries(event);
}
