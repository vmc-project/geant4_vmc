// $Id: TG4PrimaryGeneratorAction.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: run
//
// Author: I. Hrivnacova
//
// Class TG4PrimaryGeneratorAction
// -------------------------------
// See the class description in the header file.

#include "TG4PrimaryGeneratorAction.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"
//#include "AliTrackingAction.h"

#include <G4Event.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>

#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMCStack.h>
#include <TParticle.h>

//_____________________________________________________________________________
TG4PrimaryGeneratorAction::TG4PrimaryGeneratorAction()
  : TG4Verbose("primaryGeneratorAction") {
//
}

//_____________________________________________________________________________
TG4PrimaryGeneratorAction::~TG4PrimaryGeneratorAction() {
//
}

// private methods

//_____________________________________________________________________________
void TG4PrimaryGeneratorAction::TransformPrimaries(G4Event* event)
{
// Creates a new G4PrimaryVertex objects for each TParticle
// in in the MC stack.
// ---

  TVirtualMCStack* stack = gMC->GetStack();
  
  if (!stack) {
    G4String text = "TG4PrimaryGeneratorAction::TransformPrimaries:\n";
    text = text + "   No MC stack is defined.";
    TG4Globals::Exception(text);
  }  
    

  G4int nofParticles = stack->GetNtrack();
  // G4cout << "TG4PrimaryGeneratorAction::TransformPrimaries " << nofParticles << G4endl;
  if (nofParticles <= 0) {
    G4String text = "TG4PrimaryGeneratorAction::TransformPrimaries:\n";
    text = text + "   No primary particles found on the stack.";
    TG4Globals::Exception(text);
  }  

  G4PrimaryVertex* previousVertex = 0;
  G4ThreeVector previousPosition = G4ThreeVector(); 
  G4double previousTime = 0.; 

  // add verbose
  //G4cout << " nofParticles: " <<  nofParticles << G4endl;
  for (G4int i=0; i<nofParticles; i++) {    
  
    // get the particle from the stack
    TParticle* particle = stack->PopPrimaryForTracking(i);
    
    if (particle) {
      // only particles that didn't die (decay) in primary generator
      // will be transformed to G4 objects   

      // get particle definition from G4ParticleTable
      G4int pdgEncoding = particle->GetPdgCode();
      G4ParticleTable* particleTable 
        = G4ParticleTable::GetParticleTable();                        
      G4ParticleDefinition* particleDefinition = 0;      
      if (pdgEncoding != 0) 
        particleDefinition = particleTable->FindParticle(pdgEncoding);
      else {
        G4String name = particle->GetName();
        if (name == "Rootino")	
            particleDefinition = particleTable->FindParticle("geantino");
      }	
  
      if (particleDefinition==0) {
        G4cout << "pdgEncoding: " << pdgEncoding << G4endl;
        G4String text = 
            "TG4PrimaryGeneratorAction::GenerateAliGeneratorPrimaries:\n";
        text = text + "   G4ParticleTable::FindParticle() failed.";
        TG4Globals::Exception(text);
      }	

      // get/create vertex
      G4ThreeVector position 
        = G4ThreeVector(particle->Vx()*TG4G3Units::Length(),
                        particle->Vy()*TG4G3Units::Length(),
		        particle->Vz()*TG4G3Units::Length());
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
      G4double px = particle->Px()*TG4G3Units::Energy();
      G4double py = particle->Py()*TG4G3Units::Energy();
      G4double pz = particle->Pz()*TG4G3Units::Energy();
      G4PrimaryParticle* primaryParticle 
        = new G4PrimaryParticle(particleDefinition, px, py, pz);
        
      // Set polarization
      TVector3 polarization;
      particle->GetPolarisation(polarization);
      primaryParticle
        ->SetPolarization(polarization.X(), polarization.Y(), polarization.Z());    
    
      // Add primary particle to the vertex
      vertex->SetPrimary(primaryParticle);
    }   
  }
}

// public methods

//_____________________________________________________________________________
void TG4PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
// Generates primary particles by the selected generator.
// ---

  // Begin of event
  TVirtualMCApplication::Instance()->BeginEvent();

  // Generate primaries and fill the MC stack
  TVirtualMCApplication::Instance()->GeneratePrimaries();
  
  // Transform Root particle objects to G4 objects
  TransformPrimaries(event);
}
