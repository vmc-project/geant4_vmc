#ifndef TG4_EXT_DECAYER_PHYSICS_H
#define TG4_EXT_DECAYER_PHYSICS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ExtDecayerPhysics.h 
/// \brief Definition of the TG4ExtDecayerPhysics class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"
#include "TG4ExtDecayerMessenger.h"
#include <globals.hh>

class G4Decay;

/// \ingroup physics_list
/// \brief The builder for external decayer.
///
/// The external decayer is added to all instantiated decay
/// processes
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ExtDecayerPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4ExtDecayerPhysics(const G4String& name = "ExtDecayer");
    TG4ExtDecayerPhysics(G4int theVerboseLevel,
                         const G4String& name = "ExtDecayer");
    virtual ~TG4ExtDecayerPhysics();

    // set methods
    void SetSelection(const G4String& selection);
    void SetSkipNeutrino(G4bool skipNeutrino);

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    /// Not implemented
    TG4ExtDecayerPhysics(const TG4ExtDecayerPhysics& right);
    /// Not implemented
    TG4ExtDecayerPhysics& operator=(const TG4ExtDecayerPhysics& right);
    
    // data members
    TG4ExtDecayerMessenger  fMessenger;  ///< messenger
    G4Decay*  fDecayProcess; ///< decay process
    G4String  fSelection;    ///< particles selection
    G4bool    fSkipNeutrino; ///< option to skip importing neutrinos
};

// inline functions

inline void TG4ExtDecayerPhysics::SetSelection(const G4String& selection) {
  /// Select particles for which the external decayer will have priority 
  /// over using Geant4 decay table 
  fSelection = selection;
}  

inline void TG4ExtDecayerPhysics::SetSkipNeutrino(G4bool skipNeutrino) {
  /// Set option to skip importing neutrinos
  fSkipNeutrino = skipNeutrino;
}

#endif //TG4_EXT_DECAYER_PHYSICS_H

