// $Id: TG4PhysicsManager.h,v 1.10 2007/05/31 10:24:32 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsManager
/// \brief Geant4 implementation of the TVirtualMC interface methods                    
/// for building Geant4 physics and access to it.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PHYSICS_MANAGER_H
#define TG4_PHYSICS_MANAGER_H

#include "TG4Verbose.h"
#include "TG4PhysicsMessenger.h"
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

class TG4ParticlesManager;
class TG4G3PhysicsManager;
class TG4G3ProcessMap;
class TG4SpecialPhysicsList;

class G4ParticleDefinition;
class G4VProcess;
class G4VUserPhysicsList;

class TG4PhysicsManager : public TG4Verbose
{
  public:
    TG4PhysicsManager(TG4SpecialPhysicsList* physicsList);
    virtual ~TG4PhysicsManager();

    // static access method
    static TG4PhysicsManager* Instance();
        
    // methods
    void Gstpar(Int_t itmed, const char *param, Float_t parval); 

    // set methods
    Bool_t SetCut(const char* cutName, Float_t cutValue);
    Bool_t SetProcess(const char* controlName, Int_t controlValue);
    Bool_t DefineParticle(Int_t pdg, const char* name, TMCParticleType type, 
                 Double_t mass, Double_t charge, Double_t lifetime);
    Bool_t DefineIon(const char* name, Int_t Z, Int_t A,  
                 Int_t Q, Double_t excEnergy, Double_t mass);
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
    TMCProcess GetMCProcess(const G4VProcess* process);
    TMCProcess GetOpBoundaryStatus(const G4VProcess* process);

    // set methods
    void SetSpecialPhysicsList(TG4SpecialPhysicsList* physicsList);
    
    // get methods
    TG4SpecialPhysicsList* GetSpecialPhysicsList() const; 
   
  private:
    TG4PhysicsManager();
    TG4PhysicsManager(const TG4PhysicsManager& right);
    TG4PhysicsManager& operator=(const TG4PhysicsManager& right);

    // methods
    void FillProcessMap();
    void GstparCut(G4int itmed, TG4G3Cut par, G4double parval);
    void GstparControl(G4int itmed, TG4G3Control control, 
                       TG4G3ControlValue parval);
    G4ParticleDefinition* GetParticleDefinition(G4int pdgEncoding) const;

    // static data members
    static TG4PhysicsManager*  fgInstance; //this instance
    
    // data members
    TG4PhysicsMessenger    fMessenger;         //messenger
    TG4ParticlesManager*   fParticlesManager;  //particles manager
    TG4G3PhysicsManager*   fG3PhysicsManager;  //G3 physics manager
    TG4SpecialPhysicsList* fSpecialPhysicsList;//special physics list
    TG4ProcessMCMap        fProcessMCMap;//the mapping between G4 process names
                                         //and TMCProcess codes
    TG4ProcessControlMap   fProcessControlMap; //the mapping between G4 processes
                                         //and G3 process controls
};

// inline methods

inline TG4PhysicsManager* TG4PhysicsManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}

inline void TG4PhysicsManager::SetSpecialPhysicsList(
                                  TG4SpecialPhysicsList* physicsList) { 
  /// Set the physics list
  fSpecialPhysicsList = physicsList; 
}

inline TG4SpecialPhysicsList* TG4PhysicsManager::GetSpecialPhysicsList() const { 
  /// Return the physics list
  return fSpecialPhysicsList; 
}

#endif //TG4_PHYSICS_MANAGER_H

