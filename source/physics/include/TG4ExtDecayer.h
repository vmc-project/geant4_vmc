// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4ExtDecayer
/// \brief Implements the G4VExtDecayer abstract class
/// with the TVirtualMCDecayer.
///
/// In case a particle has not defined any decay channel
/// and has not pre-assigned decay products,
/// the external decayer is called.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_EXT_DECAYER_H
#define TG4_EXT_DECAYER_H

#include "TG4Verbose.h"

#include "G4VExtDecayer.hh"
#include "globals.hh"

class TVirtualMCDecayer;
class TG4ParticlesManager;

class G4Track;
class G4DecayProducts;

class TClonesArray;

class TG4ExtDecayer : public G4VExtDecayer, 
                      public TG4Verbose
{
  public:
    TG4ExtDecayer(TVirtualMCDecayer* externalDecayer);
    virtual ~TG4ExtDecayer();

    virtual G4DecayProducts* ImportDecayProducts(const G4Track& track);
    
  private:
    TG4ExtDecayer(const TG4ExtDecayer& right);
    TG4ExtDecayer& operator=(const TG4ExtDecayer& right);

    TG4ParticlesManager* fParticlesManager;  //particles manager 
    TVirtualMCDecayer*   fExternalDecayer;   //the external decayer
    TClonesArray*        fDecayProductsArray;//array of decay products
};

#endif //TG4_EXT_DECAYER_H
