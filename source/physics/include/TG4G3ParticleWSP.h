// $Id: TG4G3ParticleWSP.h,v 1.2 2004/11/10 11:39:28 brun Exp $
/// \ingroup physics
//
/// \enum TG4G3ParticleWSP
/// \brief The particles types which a special process (cuts, controls)
/// is applicable for.
///
/// Author: I. Hrivnacova

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

