#ifndef TG4_EM_MODEL_PHYSICS_H
#define TG4_EM_MODEL_PHYSICS_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4EmModelPhysics.h
/// \brief Definition of the TG4EmModelPhysics class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"
#include "TG4EmModelPhysicsMessenger.h"

#include <G4EmConfigurator.hh>
#include <globals.hh>

#include <vector>
#include <utility>

/// \ingroup physics_list
/// \brief Enumeration for EM physics models supported in this class.
///
/// \author I. Hrivnacova; IPN Orsay

enum TG4EmModel {
  kPAIModel,             ///< PAI model
  kPAIPhotonModel,       ///< PAIPhoton model
  kNoEmModel             ///< No extra EM model
};  

class TG4EmModelConfiguration;

class G4Region;

/// \ingroup physics_list
/// \brief Physics builder which activates a selected EM energy loss
/// and/or fluctuations model.
///
/// In this first implementation, only G4PAIModel and G4PAIPhotonModel
/// are supported. Other models available in Geant4 can be added on user
/// requests.
/// 
/// \author I. Hrivnacova; IPN Orsay

class TG4EmModelPhysics: public TG4VPhysicsConstructor
{
  public:
    typedef std::map<G4int,TG4EmModelConfiguration*>  ModelToMediumIdMap;
    
  public:
    TG4EmModelPhysics(const G4String& name = "EmModelPhysics");
    TG4EmModelPhysics(G4int verboseLevel,
                      const G4String& name = "mModelPhysics");
    virtual ~TG4EmModelPhysics();
    
    // static methods
    static TG4EmModel GetEmModel(const G4String& modelName);
    static G4String   GetEmModelName(G4int modelType);
    
    // set methods
    void SetEmModel(G4int mediumId, 
                    const G4String& elossModelName,
                    const G4String& fluctModelName,
                    const G4String& particles = "all");
    void SetEmElossModel(G4int mediumId, 
                    const G4String& elossModelName);
    void SetEmFluctModel(G4int mediumId, 
                    const G4String& elossModelName);
    void SetEmModelParticles(G4int mediumId, 
                    const G4String& particles);

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    /// Not implemented
    TG4EmModelPhysics(const TG4EmModelPhysics& right);
    /// Not implemented
    TG4EmModelPhysics& operator=(const TG4EmModelPhysics& right);
    
    // methods
    void CreateRegions();
    void FillModelVectorFromMedia();
    void AddModel(TG4EmModel elossModel,
                  TG4EmModel fluctModel,
                  const G4ParticleDefinition* particle, 
                  const G4String& processName,
                  const G4String& regionName);
    void AddModels();

    // data members
    TG4EmModelPhysicsMessenger  fMessenger;///< messenger 
    G4EmConfigurator      fEmConfigurator; ///< G4 EM configurator
    ModelToMediumIdMap    fModelMap;       ///< the map of models per tracking media
};

#endif //TG4_PROCESS_MAP_PHYSICS_H

