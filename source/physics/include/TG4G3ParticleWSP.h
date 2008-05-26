#ifndef TG4_G3_PARTICLE_WSP_H
#define TG4_G3_PARTICLE_WSP_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4G3ParticleWSP.h
/// \brief Definition of the enumeration TG4G3ParticleWSP
///
/// \author I. Hrivnacova; IPN Orsay

/// \ingroup physics
/// \brief The particles types which a special process (cuts, controls)
/// is applicable for.

enum TG4G3ParticleWSP
{
  kGamma,           ///< kPAIR, kCOMP, kPHOT, kPHIS
  kElectron,        ///< kDRAY, kBREM, kMULS, kLOSS
  kEplus,           ///< kDRAY, kBREM, kMULS, kLOSS, kANNI
  kNeutralHadron,   ///< kHADR 
  kChargedHadron,   ///< kDRAY, kMULS, kLOSS, kHADR,
  kMuon,            ///< kDRAY, kBREM, kMULS, kLOSS, kMUNU
  kAny,             ///< kDCAY
  kNofParticlesWSP  ///< not a particle with a special control
};
   
#endif //TG4_G3_PARTICLE_WSP_H

