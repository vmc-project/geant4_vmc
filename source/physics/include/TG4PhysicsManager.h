#ifndef TG4_PHYSICS_MANAGER_H
#define TG4_PHYSICS_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4PhysicsManager.h
/// \brief Definition of the TG4PhysicsManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"
#include "TG4NameMap.h"
#include "TG4G3Cut.h"
#include "TG4G3Control.h"
#include "TG4Globals.h"

#include <Rtypes.h>
#include <TMCProcess.h>
#include <TMCParticleType.h>
#include <TString.h>

#include <globals.hh>

#include <set>

class TG4ParticlesManager;
class TG4G3PhysicsManager;
class TG4G3ProcessMap;

class G4ParticleDefinition;
class G4ProcessManager;
class G4VProcess;
class G4VUserPhysicsList;
class G4OpBoundaryProcess;

/// \ingroup physics
/// \brief Geant4 implementation of the TVirtualMC interface methods
/// for building Geant4 physics and access to it.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4PhysicsManager : public TG4Verbose
{
  public:
    TG4PhysicsManager();
    virtual ~TG4PhysicsManager();

    // static access method
    static TG4PhysicsManager* Instance();

    // methods
    void Gstpar(Int_t itmed, const char *param, Float_t parval);

    // set methods
    Bool_t SetCut(const char* cutName, Float_t cutValue);
    Bool_t SetProcess(const char* controlName, Int_t controlValue);
    Bool_t DefineParticle(Int_t pdg, const char* name,
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
    Bool_t DefineIon(const char* name, Int_t Z, Int_t A,
                 Int_t Q, Double_t excEnergy, Double_t mass);
    void   SetUserDecay(Int_t pdg);
    Bool_t SetDecayMode(Int_t pdg, Float_t bratio[6], Int_t mode[6][3]);

    Float_t Xsec(char* reac, Float_t energy, Int_t part, Int_t mate);

        // particle table usage
    Int_t IdFromPDG(Int_t pdgID) const;
    Int_t PDGFromId(Int_t mcID) const;

        // get methods
    TString   ParticleName(Int_t pdg) const;
    Double_t  ParticleMass(Int_t pdg) const;
    Double_t  ParticleCharge(Int_t pdg) const;
    Double_t  ParticleLifeTime(Int_t pdg) const;
    TMCParticleType ParticleMCType(Int_t pdg) const;

    //
    // methods for Geant4 only
    //

    void  DefineParticles();
    void  SetProcessActivation();
    void  RetrieveOpBoundaryProcess();
    TMCProcess GetMCProcess(const G4VProcess* process);
    TMCProcess GetOpBoundaryStatus();

    void SetCutForGamma(G4double cut);
    void SetCutForElectron(G4double cut);
    void SetCutForPositron(G4double cut);
    void SetCutForProton(G4double cut);

    G4double GetCutForGamma() const;
    G4double GetCutForElectron() const;
    G4double GetCutForPositron() const;
    G4double GetCutForProton() const;
    G4bool   IsOpBoundaryProcess() const;

  private:
    /// Not implemented
    TG4PhysicsManager(const TG4PhysicsManager& right);
    /// Not implemented
    TG4PhysicsManager& operator=(const TG4PhysicsManager& right);

    // methods
    void GstparCut(G4int itmed, TG4G3Cut par, G4double parval);
    void GstparControl(G4int itmed, TG4G3Control control,
                       TG4G3ControlValue parval);
    G4ParticleDefinition* GetParticleDefinition(G4int pdgEncoding) const;

    G4VProcess*  FindProcess(G4String processName) const;

    void SetProcessActivation(G4ProcessManager* processManager,
                              G4int processId, G4bool activation);
    void SetSpecialControlsActivation();
    void SetSpecialCutsActivation();

    // static data members
    static  TG4PhysicsManager*  fgInstance; ///< this instance

    static const G4double    fgkDefautCut; ///< the default range cut value

    /// the mapping between G4 process names and TMCProcess codes
    static TG4ProcessMCMap*  fgProcessMCMap;

    /// the mapping between G4 processes and G3 process controls
    static TG4ProcessControlMap*  fgProcessControlMap;

    //
    // data members

    /// particles manager
    TG4ParticlesManager*   fParticlesManager;

    /// G3 physics manager
    TG4G3PhysicsManager*   fG3PhysicsManager;

    /// set of not implemented Gstpar parameters
    std::set<TString>      fNotImplParNames;

    /// range cut for gamma
    G4double               fCutForGamma;

    /// range cut for electron
    G4double               fCutForElectron;

    /// range cut for positron
    G4double               fCutForPositron;

    /// range cut for proton
    G4double               fCutForProton;

    /// optical boundary process
    G4OpBoundaryProcess*   fOpBoundaryProcess;

};

// inline methods

inline TG4PhysicsManager* TG4PhysicsManager::Instance() {
  /// Return this instance
  return fgInstance;
}

inline void TG4PhysicsManager::SetCutForGamma(G4double cut) {
  /// Set range cut value for gamma
  fCutForGamma = cut;
}

inline void TG4PhysicsManager::SetCutForElectron(G4double cut) {
  /// Set range cut value for electron
  fCutForElectron = cut;
}

inline void TG4PhysicsManager::SetCutForPositron(G4double cut) {
  /// Set range cut value for positron
  fCutForPositron = cut;
}

inline void TG4PhysicsManager::SetCutForProton(G4double cut) {
  /// Set range cut value for proton
  fCutForProton = cut;
}

inline G4double TG4PhysicsManager::GetCutForGamma() const {
  /// Return range cut for gamma
  return fCutForGamma;
}

inline G4double TG4PhysicsManager::GetCutForElectron() const {
  /// Return range cut for electron
  return fCutForElectron;
}

inline G4double TG4PhysicsManager::GetCutForPositron() const {
  /// Return range cut for positron
  return fCutForPositron;
}

inline G4double TG4PhysicsManager::GetCutForProton() const {
  /// Return range cut for positron
  return fCutForProton;
}

inline G4bool TG4PhysicsManager::IsOpBoundaryProcess() const {
  /// Return true if optical boundary process is defined
  return ( fOpBoundaryProcess != 0 );
}

#endif //TG4_PHYSICS_MANAGER_H

