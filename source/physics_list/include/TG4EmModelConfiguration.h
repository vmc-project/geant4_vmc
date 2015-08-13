#ifndef TG4_EM_MODEL_CONFIGURATION_H
#define TG4_EM_MODEL_CONFIGURATION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4EmModelConfiguration.h
/// \brief Definition of the TG4EmModelConfiguration class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <globals.hh>

/// \ingroup physics_list
/// \brief A helper class to hold a configuration of an extra EM model 
///  defined per tracking medium (via G4Region).
///
/// \author I. Hrivnacova; IPN Orsay

class TG4EmModelConfiguration
{
  public:
    TG4EmModelConfiguration(const G4String& modelName);
    TG4EmModelConfiguration(const TG4EmModelConfiguration& right);
    TG4EmModelConfiguration& operator=(const TG4EmModelConfiguration& right);
    virtual ~TG4EmModelConfiguration();
    
    // set methods
    void SetParticles(const G4String& particles);
    void SetRegions(const G4String& regions);

    // get methods
    const G4String& GetModelName() const;
    const G4String& GetParticles() const;
    const G4String& GetRegions() const;

  private:

    // data members
    G4String  fModelName; ///< the EM model name
    G4String  fParticles; ///< the list of particles
    G4String  fRegions;   ///< the list of regions (media)
};

// inline functions

inline void TG4EmModelConfiguration::SetRegions(const G4String& regions) {
  /// Set the list of particles
  fRegions = regions;
}

inline void TG4EmModelConfiguration::SetParticles(const G4String& particles) {
  /// Set the list of particles
  fParticles = particles;
}

inline const G4String& TG4EmModelConfiguration::GetModelName() const {
  /// Return the energy loss model name
  return fModelName;
}  

inline const G4String& TG4EmModelConfiguration::GetRegions() const {
  /// Return the list of particles
  return fRegions;
}

inline const G4String& TG4EmModelConfiguration::GetParticles() const {
  /// Return the list of particles
  return fParticles; 
}  

#endif //TG4_EM_MODEL_CONFIGURATION_H

