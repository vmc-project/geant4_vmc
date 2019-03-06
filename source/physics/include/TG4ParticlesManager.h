#ifndef TG4_PARTICLES_MANAGER_H
#define TG4_PARTICLES_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ParticlesManager.h
/// \brief Definition of the TG4ParticlesManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"
#include "TG4Globals.h"
#include "TG4NameMap.h"
#include "TG4IntMap.h"
#include "TG4ParticlesChecker.h"

#include <globals.hh>
#include <G4ThreeVector.hh>

#include <Rtypes.h>
#include <TMCParticleType.h>

#include <map>
#include <vector>

class G4DynamicParticle;
class G4ParticleDefinition;

class TParticle;
class TClonesArray;

class TG4UserIon;
class TG4UserParticle;

/// \ingroup physics
/// \brief Provides mapping between TDatabasePDG and Geant4 particles.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ParticlesManager : public TG4Verbose
{
  public:
    /// The map of user ions by their names
    typedef std::map<G4String, TG4UserIon*>  UserIonMap;

    /// The vector of user particles
    typedef std::vector<TG4UserParticle*>    UserParticleVector;

    TG4ParticlesManager();
    virtual ~TG4ParticlesManager();

    // static access method
    static TG4ParticlesManager* Instance();

    // methods
    void DefineParticles();
    void AddParticle(Int_t pdg, const TString& name,
                 TMCParticleType mcType,
                 Double_t mass, Double_t charge, Double_t lifetime,
                 const TString& pType, Double_t width,
                 Int_t iSpin, Int_t iParity, Int_t iConjugation,
                 Int_t iIsospin, Int_t iIsospinZ, Int_t gParity,
                 Int_t lepton, Int_t baryon,
                 Bool_t stable, Bool_t shortlived = kFALSE,
                 const TString& subType = "",
                 Int_t antiEncoding = 0, Double_t magMoment = 0.0,
                 Double_t excitation = 0.0);
    void AddIon(const G4String& name, G4int Z, G4int A, G4int Q,
                G4double excEnergy);
    void   SetUserDecay(Int_t pdg);
    Bool_t SetDecayMode(Int_t pdg, Float_t bratio[6], Int_t mode[6][3]);
    // get methods
         // for G4 particle types
    G4int GetPDGEncoding(G4ParticleDefinition* particle);

         // for Root particle types;
    TParticle* GetParticle(const TClonesArray* particles, G4int index) const;
    G4ParticleDefinition* GetParticleDefinition(
                           const TParticle* particle, G4bool warn = true) const;

    G4DynamicParticle* CreateDynamicParticle(
                           const TParticle* particle) const;
    G4ThreeVector GetParticlePosition(
                           const TParticle* particle) const;
    G4ThreeVector GetParticleMomentum(
                           const TParticle* particle) const;
    G4ThreeVector GetParticlePolarization(
                           const TParticle* particle) const;

    TG4UserIon*   GetUserIon(const G4String& ionName, G4bool warn = true) const;

    G4int             GetNofUserParticles() const;
    TG4UserParticle*  GetUserParticle(G4int index) const;

  private:
    /// Not implemented
    TG4ParticlesManager(const TG4ParticlesManager& right);
    /// Not implemented
    TG4ParticlesManager& operator=(const TG4ParticlesManager& right);

    // methods
    // G4int GetPDGIonEncoding(G4int Z, G4int A, G4int iso) const;
    void  AddParticleToPdgDatabase(const G4String& name,
                              G4ParticleDefinition* particleDefinition);

    // static data members
    static TG4ParticlesManager*  fgInstance; ///< this instance

    //
    // data members

    /// \brief the mapping between G4 particle names and TDatabasePDG names
    /// for special particles
    TG4NameMap         fParticleNameMap;

    /// user defined ions mappped by their names
    UserIonMap         fUserIonMap;

    /// vector of user defined particles
    UserParticleVector fUserParticles;

    /// particles checker
    TG4ParticlesChecker  fParticlesChecker;
};

// inline methods

inline TG4ParticlesManager* TG4ParticlesManager::Instance() {
  /// Return this instance
  return fgInstance;
}

#endif //TG4_PARTICLES_MANAGER_H

