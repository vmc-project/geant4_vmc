// $Id: TG4G3ParticleWSP.h,v 1.3 2005/09/01 10:04:33 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \enum TG4G3ParticleWSP
/// \brief The particles types which a special process (cuts, controls)
/// is applicable for.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_G3_PARTICLE_WSP_H
#define TG4_G3_PARTICLE_WSP_H

enum TG4G3ParticleWSP
// particle with special process
{
  kGamma,           //< kPAIR, kCOMP, kPHOT, kPHIS
  kElectron,        //< kDRAY, kBREM, kMULS, kLOSS
  kEplus,           //< kDRAY, kBREM, kMULS, kLOSS, kANNI
  kNeutralHadron,   //< kHADR 
  kChargedHadron,   //< kDRAY, kMULS, kLOSS, kHADR,
  kMuon,            //< kDRAY, kBREM, kMULS, kLOSS, kMUNU
  kAny,             //< kDCAY
  kNofParticlesWSP  //< not a particle with a special control
};
   
#endif //TG4_G3_PARTICLE_WSP_H

