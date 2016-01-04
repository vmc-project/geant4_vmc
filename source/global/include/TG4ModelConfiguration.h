#ifndef TG4_MODEL_CONFIGURATION_H
#define TG4_MODEL_CONFIGURATION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ModelConfiguration.h
/// \brief Definition of the TG4ModelConfiguration class
///
/// \author I. Hrivnacova; IPN Orsay

#include <globals.hh>

/// \ingroup global
/// \brief A helper class to hold a configuration of a special physics model.
///
/// The class holds the information abou the association of
/// the special physics model (EM physics or fast simulation model)
/// and the applicable regions (G4Region) defined via tracking media
/// and particles.
/// 
/// This class is intended also to substitute TG4EmModelConfiguration.
///
/// \author I. Hrivnacova; IPN Orsay

class G4VFastSimulationModel;

class TG4ModelConfiguration
{
  public:
    TG4ModelConfiguration(const G4String& modelName);
    
    // set methods
    void SetParticles(const G4String& particles);
    void SetRegions(const G4String& regions);
    void SetFastSimulationModel(G4VFastSimulationModel* fastSimulationModel);

    // get methods
    const G4String& GetModelName() const;
    const G4String& GetParticles() const;
    const G4String& GetRegions() const;
    G4VFastSimulationModel* GetFastSimulationModel() const;

    G4bool HasParticle(const G4String& particleName);
    G4bool HasRegion(const G4String& regionName);

  private:
    /// not implemented
    TG4ModelConfiguration(const TG4ModelConfiguration& right);
    /// not implemented
    TG4ModelConfiguration& operator=(const TG4ModelConfiguration& right);

    // data members
    G4String  fModelName; ///< the EM model name
    G4String  fParticles; ///< the list of particle names
    G4String  fRegions;   ///< the list of regions (=media)
    G4VFastSimulationModel*  fFastSimulationModel; ///< fast simulation model
};

// inline functions

inline void TG4ModelConfiguration::SetParticles(const G4String& particles) {
  /// Set the list of particles
  fParticles = particles;
}

inline void TG4ModelConfiguration::SetRegions(const G4String& regions) {
  /// Set the list of regions
  fRegions = regions;
}

inline void TG4ModelConfiguration::SetFastSimulationModel(
                                     G4VFastSimulationModel* fastSimulationModel) {
  /// Set fast simulation model
  fFastSimulationModel = fastSimulationModel;
}

inline const G4String& TG4ModelConfiguration::GetModelName() const {
  /// Return the energy loss model name
  return fModelName;
}

inline const G4String& TG4ModelConfiguration::GetParticles() const {
  /// Return the list of particles
  return fParticles;
}

inline const G4String& TG4ModelConfiguration::GetRegions() const {
  /// Return the list of regions
  return fRegions;
}

inline G4VFastSimulationModel* TG4ModelConfiguration::GetFastSimulationModel() const {
  /// Return fast simulation model
  return fFastSimulationModel;
}

#endif //TG4_MODEL_CONFIGURATION_H
