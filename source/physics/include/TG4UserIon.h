#ifndef TG4_USER_ION_H
#define TG4_USER_ION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4UserIon.h
/// \brief Definition of the TG4UserIon class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Globals.h"

#include <G4ParticleDefinition.hh>
#include <globals.hh>

#include <TMCParticleType.h>

/// \ingroup physics
/// \brief The class for user ion definition.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4UserIon
{
  public:
             /// Standard constructor
    TG4UserIon(const G4String& name, G4int pdgEncoding, G4int Q)
      : fName(name), fPdgEncoding(pdgEncoding), fQ(Q) {}

             /// Destructor
    ~TG4UserIon() {}

    // methods

             /// Return the ion name defined by user
    G4String GetName() const { return fName; }

             /// Return the ion PDG encoding
    G4int    GetPdgEncoding() const { return fPdgEncoding; }

             /// Return the ion charge
    G4int    GetQ() const { return fQ; }

  private:
    // data members
    G4String fName;        ///< ion name defined by user
    G4int    fPdgEncoding; ///< PDG encoding
    G4int    fQ;           ///< charge
};

#endif //TG4_USER_ION_H

