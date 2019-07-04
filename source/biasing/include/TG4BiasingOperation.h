#ifndef TG4_BIASING_OPERATION_HH
#define TG4_BIASING_OPERATION_HH

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2019 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4BiasingOperation.h
/// \brief Definition of the TG4BiasingOperation class
///
/// \author Alberto Ribon, CERN

#include "G4VBiasingOperation.hh"

class G4ProtonInelasticProcess;
class G4NeutronInelasticProcess;
class G4PionPlusInelasticProcess;
class G4PionMinusInelasticProcess;


class TG4BiasingOperation : public G4VBiasingOperation {
  // The biasing operation implemented in this class is indeed a "trick" to 
  // use FTFP+INCLXX instead of FTFP+BERT for determining the final-state of
  // proton, neutron, pion+, pion- inelastic interactions happening in one
  // particular logical volume, Tracking_region, where the biasing is applied.
  public:
    TG4BiasingOperation( G4String name );
    virtual ~TG4BiasingOperation();
    virtual G4VParticleChange* ApplyFinalStateBiasing( const G4BiasingProcessInterface*, const G4Track*,
                                                       const G4Step*, G4bool& );
    // Unused :
    virtual const G4VBiasingInteractionLaw* 
      ProvideOccurenceBiasingInteractionLaw( const G4BiasingProcessInterface*, G4ForceCondition& ) { return 0; }
    virtual G4double DistanceToApplyOperation( const G4Track*, G4double, G4ForceCondition* ) { return DBL_MAX; }
    virtual G4VParticleChange* GenerateBiasingFinalState( const G4Track*, const G4Step* )  { return 0; }

  private:
    G4ProtonInelasticProcess*    fProtonInelasticProcess;
    G4NeutronInelasticProcess*   fNeutronInelasticProcess;
    G4PionPlusInelasticProcess*  fPionPlusInelasticProcess;
    G4PionMinusInelasticProcess* fPionMinusInelasticProcess;
};

#endif

