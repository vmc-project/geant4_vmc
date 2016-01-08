#ifndef TG4_EM_MODEL_PHYSICS_H
#define TG4_EM_MODEL_PHYSICS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4EmModelPhysics.h
/// \brief Definition of the TG4EmModelPhysics class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

#include <vector>
#include <utility>

/// \ingroup physics_list
/// \brief Enumeration for EM physics models supported in this class.
///
/// \author I. Hrivnacova; IPN Orsay

enum TG4EmModel {
  kPAIModel,             ///< PAI model
  kPAIPhotonModel,       ///< PAIPhot model
  kSpecialUrbanMscModel, ///< Special UrbanMsc model adapted for ALICE EMCAL
  kNoEmModel             ///< No extra EM model
};  

class TG4ModelConfiguration;

class G4Region;

/// \ingroup physics_list
/// \brief Physics builder which activates a selected EM energy loss
/// and/or fluctuations model.
///
/// In this implementation, G4PAIModel, G4PAIPhotModel and a special
/// UrbanMsc model tuned for ALICE EMCAL are supported.
/// Other models available in Geant4 can be added on user
/// requests.
/// 
/// \author I. Hrivnacova; IPN Orsay

class TG4EmModelPhysics: public TG4VPhysicsConstructor
{
  // public:
  //   typedef std::vector<TG4EmModelConfiguration*> EmModelConfigurationVector;
    
  public:
    TG4EmModelPhysics(const G4String& name = "EmModel");
    TG4EmModelPhysics(G4int theVerboseLevel,
                      const G4String& name = "EmModel");
    virtual ~TG4EmModelPhysics();
    
    // static methods
    static TG4EmModel GetEmModel(const G4String& modelName);
    static G4String   GetEmModelName(G4int modelType);
    
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
    
    void AddModel(TG4EmModel model,
                  const G4ParticleDefinition* particle, 
                  const G4String& regionName);
    void AddModels(const std::vector<TG4ModelConfiguration*>& models);
};

#endif //TG4_PROCESS_MAP_PHYSICS_H

