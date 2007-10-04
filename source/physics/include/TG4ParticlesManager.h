// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4ParticlesManager
/// \brief Provides mapping between TDatabasePDG and Geant4 particles. 
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PARTICLES_MANAGER_H
#define TG4_PARTICLES_MANAGER_H

#include "TG4Verbose.h"
#include "TG4Globals.h"
#include "TG4NameMap.h"
#include "TG4IntMap.h"

#include <globals.hh>
#include <G4ThreeVector.hh>

#include <Rtypes.h>

#include <map>

class G4DynamicParticle;
class G4ParticleDefinition;

class TParticle;
class TClonesArray;

class TG4UserIon
{
  public:
    TG4UserIon(const G4String& name, G4int pdgEncoding, G4int Q)
      : fName(name), fPdgEncoding(pdgEncoding), fQ(Q) {}
    ~TG4UserIon();
    
    // methods
    G4String GetName() const        { return fName;        }
    G4int    GetPdgEncoding() const { return fPdgEncoding; }
    G4int    GetQ() const           { return fQ;           }
    
    // data members
    G4String fName;        // ion name defined by user
    G4int    fPdgEncoding; // PDG encoding
    G4int    fQ;           // charge
};      


class TG4ParticlesManager : public TG4Verbose
{
  public:
    typedef std::map<G4String, TG4UserIon*>  UserIonMap;

    TG4ParticlesManager();
    virtual ~TG4ParticlesManager();

    // static access method
    static TG4ParticlesManager* Instance();
        
    // methods
    void DefineParticles();
    void AddIon(const G4String& name, G4int Z, G4int A, G4int Q, 
                G4double excEnergy);

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
    TG4UserIon*   GetUserIon(const G4String& ionName, G4bool warn = true) const;
    
  private:
    TG4ParticlesManager(const TG4ParticlesManager& right);
    TG4ParticlesManager& operator=(const TG4ParticlesManager& right);

    // methods
    // G4int GetPDGIonEncoding(G4int Z, G4int A, G4int iso) const;
    void  AddIonToPdgDatabase(const G4String& name,
                              G4ParticleDefinition* particleDefinition);    

    // static data members
    static TG4ParticlesManager*  fgInstance; //this instance
    
    // data members
    TG4NameMap  fParticleNameMap; // the mapping between G4 particle names
                                  // and TDatabasePDG names for special particles
    UserIonMap  fUserIonMap;      // user defined ions mappped by their names
};

// inline methods

inline TG4ParticlesManager* TG4ParticlesManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}

#endif //TG4_PARTICLES_MANAGER_H

