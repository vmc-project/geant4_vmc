// $Id: TG4G3ParticleWSP.h 295 2007-10-04 23:17:33Z rdm $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \enum TG4UserIon
/// \brief The class for user ion definition.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_USER_ION_H
#define TG4_USER_ION_H

#include "TG4Globals.h"

#include <G4ParticleDefinition.hh>
#include <globals.hh>

#include <TMCParticleType.h>

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

#endif //TG4_USER_ION_H

