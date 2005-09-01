// $Id: TG4ParticlesManager.h,v 1.3 2004/11/10 11:39:28 brun Exp $
/// \ingroup physics
//
/// \class TG4ParticlesManager
/// \brief Provides mapping between TDatabasePDG and Geant4 particles. 
///
/// Author: I. Hrivnacova

#ifndef TG4_PARTICLES_MANAGER_H
#define TG4_PARTICLES_MANAGER_H

#include "TG4Verbose.h"
#include "TG4Globals.h"
#include "TG4NameMap.h"
#include "TG4IntMap.h"

#include <globals.hh>
#include <G4ThreeVector.hh>

#include <Rtypes.h>

class G4DynamicParticle;
class G4ParticleDefinition;

class TParticle;
class TClonesArray;

class TG4ParticlesManager : public TG4Verbose
{
  public:
    TG4ParticlesManager();
    // --> protected
    // TG4ParticlesManager(const TG4ParticlesManager& right);
    virtual ~TG4ParticlesManager();

    // static access method
    static TG4ParticlesManager* Instance();
        
    // methods
    void DefineParticles();
    void AddIon(const G4String& name, G4int Z, G4int A, G4int Q, 
                G4double excEnergy);

    // get methods
         // for G4 particle types   
    G4int GetPDGEncodingFast(G4ParticleDefinition* particle, G4int Q);

         // for Root particle types;
    TParticle* GetParticle(const TClonesArray* particles, G4int index) const;
    G4ParticleDefinition* GetParticleDefinition(
                           const TParticle* particle, G4bool warn = true) const;
    G4ParticleDefinition* GetIonParticleDefinition(
                           const TParticle* particle, G4bool warn = true) const;
    G4DynamicParticle* CreateDynamicParticle(
                           const TParticle* particle) const;
    G4ThreeVector GetParticlePosition(
                           const TParticle* particle) const;
    G4ThreeVector GetParticleMomentum(
                           const TParticle* particle) const;        
    
  protected:
    TG4ParticlesManager(const TG4ParticlesManager& right);

    // operators
    TG4ParticlesManager& operator=(const TG4ParticlesManager& right);

  private:
    // methods
    G4int GetPDGEncoding(G4ParticleDefinition* particle) const;
    G4int GetPDGEncoding(G4String particleName) const;
    G4int GetPDGIonEncoding(G4int Z, G4int A, G4int iso) const;
    void  AddParticlesToPdgDatabase() const;
    void  MapParticles();
    G4int AddIonToPdgDatabase(const G4String& name,
              G4ParticleDefinition* particleDefinition, G4int Q);    
    G4String  UniqueIonName(const G4String& g4IonName, G4int Q) const;
    G4String  CutUniqueIonName(const G4String& uniqueIonName) const;

    // static data members
    static TG4ParticlesManager*  fgInstance; //this instance
    
    // data members
    TG4NameMap  fParticleNameMap;  //the mapping between G4 particle names
                                   //and TDatabasePDG names 
    TG4IntMap   fParticlePDGMap;   //the mapping between G4 particle names
                                   //and TDatabasePDG codes
};

// inline methods

inline TG4ParticlesManager* TG4ParticlesManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}

#endif //TG4_PARTICLES_MANAGER_H

