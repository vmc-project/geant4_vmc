//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ExtDecayer.cxx
/// \brief Implementation of the TG4ExtDecayer class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ExtDecayer.h"
#include "TG4ParticlesManager.h"
#include "TG4G3Units.h"

#include <G4DynamicParticle.hh>
#include <G4DecayProducts.hh>
#include <G4DecayTable.hh>
#include <G4ParticleTable.hh>
#include <G4DynamicParticle.hh>
#include <G4Track.hh>

#include <TParticle.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TVirtualMCDecayer.h>

#include <math.h>

//_____________________________________________________________________________
TG4ExtDecayer::TG4ExtDecayer(TVirtualMCDecayer* externalDecayer)
  : G4VExtDecayer("TG4ExtDecayer"),
    TG4Verbose("extDecayer"),
    fParticlesManager(TG4ParticlesManager::Instance()),
    fExternalDecayer(externalDecayer),
    fDecayProductsArray(0),
    fSkipNeutrino(false)
{
/// Standard constructor

  fDecayProductsArray = new  TClonesArray("TParticle", 1000);
}

//_____________________________________________________________________________
TG4ExtDecayer::~TG4ExtDecayer()
{
/// Destructor

  delete fDecayProductsArray;
}

//
// public methods
//

//_____________________________________________________________________________
G4DecayProducts* TG4ExtDecayer::ImportDecayProducts(const G4Track& track)
{
/// Import decay products

  // check if external decayer is defined
  if (!fExternalDecayer) {
     G4cerr << "TG4ExtDecayer::ImportDecayProducts: " << G4endl
            << " No fExternalDecayer is defined." << G4endl;
    return 0;
  }

  // get particle momentum
  G4ThreeVector momentum = track.GetMomentum();
  G4double etot = track.GetDynamicParticle()->GetTotalEnergy();;
  TLorentzVector p;
  p[0] = momentum.x() / TG4G3Units::Energy();
  p[1] = momentum.y() / TG4G3Units::Energy();
  p[2] = momentum.z() / TG4G3Units::Energy();
  p[3] = etot         / TG4G3Units::Energy();

  // get particle PDG
  // ask TG4ParticlesManager to get PDG encoding
  // (in order to get PDG from extended TDatabasePDG
  // in case the standard PDG code is not defined)
  G4ParticleDefinition* particleDef = track.GetDefinition();
  G4int pdgEncoding = fParticlesManager->GetPDGEncoding(particleDef);

  // let TVirtualMCDecayer decay the particle
  // and import the decay products
  fExternalDecayer->Decay(pdgEncoding, &p);
  G4int nofParticles
    = fExternalDecayer->ImportParticles(fDecayProductsArray);

  if (VerboseLevel()>1) {
    G4cout << "nofParticles: " <<  nofParticles << G4endl;
  }

  // convert decay products TParticle type
  // to G4DecayProducts
  G4DecayProducts* decayProducts
    = new G4DecayProducts(*(track.GetDynamicParticle()));

  G4int counter = 0;
  for (G4int i=0; i<nofParticles; i++) {

    // get particle from TClonesArray
    TParticle* particle
      = fParticlesManager->GetParticle(fDecayProductsArray, i);

    G4int status = particle->GetStatusCode();
    G4int pdg = particle->GetPdgCode();
    if ( ( status>0 && status<11 ) &&
         ( ( ! fSkipNeutrino ) || ( abs(pdg)!=12 && abs(pdg)!=14 && abs(pdg)!=16 ) ) ) {
      // pass to tracking final particles only;
      // skip neutrinos if skipping option is active

      if (VerboseLevel()>1) {
        G4cout << "  " << i << "th particle PDG: " << pdg << "   ";
      }

      // create G4DynamicParticle
      G4DynamicParticle* dynamicParticle
        = fParticlesManager->CreateDynamicParticle(particle);

      if (dynamicParticle) {

        if (VerboseLevel()>1) {
          G4cout << "  G4 particle name: "
                 << dynamicParticle->GetDefinition()->GetParticleName()
                 << G4endl;
        }

        // add dynamicParticle to decayProducts
        decayProducts->PushProducts(dynamicParticle);

        counter++;
      }
    }
  }
  if (VerboseLevel()>1) {
    G4cout << "nofParticles for tracking: " <<  counter << G4endl;
  }

  return decayProducts;
}

