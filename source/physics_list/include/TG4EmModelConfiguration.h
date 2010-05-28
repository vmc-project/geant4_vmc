#ifndef TG4_EM_MODEL_CONFIGURATION_H
#define TG4_EM_MODEL_CONFIGURATION_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
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
    TG4EmModelConfiguration(const G4String& elossModelName,
                            const G4String& fluctModelName,
                            const G4String& particles = "all");
    TG4EmModelConfiguration(const TG4EmModelConfiguration& right);
    TG4EmModelConfiguration& operator=(const TG4EmModelConfiguration& right);
    virtual ~TG4EmModelConfiguration();
    
    // set methods
    void SetElossModelName(const G4String& elossModelName);
    void SetFluctModelName(const G4String& elossModelName);
    void SetParticles(const G4String& particles);

    // get methods
    const G4String& GetElossModelName() const;
    const G4String& GetFluctModelName() const;
    const G4String& GetParticles() const;

  private:

    // data members
    G4String  fElossModelName; ///< the energy loss model name
    G4String  fFluctModelName; ///< the energy fluctuation model name
    G4String  fParticles;      ///< the list of particles
};

// inline functions

inline void TG4EmModelConfiguration::SetElossModelName(const G4String& elossModelName) {
  /// Set the energy loss model name
  fElossModelName = elossModelName;
}

inline void TG4EmModelConfiguration::SetFluctModelName(const G4String& fluctModelName) {
  /// Set the energy fluctuation model name
  fFluctModelName = fluctModelName;
}

inline void TG4EmModelConfiguration::SetParticles(const G4String& particles) {
  /// Set the list of particles
  fParticles = particles;
}


inline const G4String& TG4EmModelConfiguration::GetElossModelName() const {
  /// Return the energy loss model name
  return fElossModelName;
}  

inline const G4String& TG4EmModelConfiguration::GetFluctModelName() const {
  /// Return the energy fluctuation model name
  return fFluctModelName;
}  

inline const G4String& TG4EmModelConfiguration::GetParticles() const {
  /// Return the list of particles
  return fParticles; 
}  

#endif //TG4_EM_MODEL_CONFIGURATION_H

