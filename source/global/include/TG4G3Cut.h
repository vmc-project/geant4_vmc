// $Id: TG4G3Cut.h,v 1.4 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup global
/// \enum TG4G3Cut
/// Enumeration for G3 types of kinetic energy cuts.
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_G3_CUT_H
#define TG4_G3_CUT_H

enum TG4G3Cut
{
  kCUTGAM, //< gammas 
               //< G4 particles: "gamma"         
               //< G3 default value: 0.001 GeV
  kCUTELE, //< electrons        
               //< G4 particles: "e-"         
               //< ?? positrons
               //< G3 default value: 0.001 GeV
  kCUTNEU, //< neutral hadrons  
               //< G4 particles: of type "baryon", "meson", "nucleus"   
               //<               with zero charge     
               //< G3 default value: 0.01 GeV
  kCUTHAD, //< charged hadrons  
               //< G4 particles: of type "baryon", "meson", "nucleus"        
               //<               with non-zero charge     
               //< G3 default value: 0.01 GeV
  kCUTMUO, //< muons            
               //< G4 particles: "mu+", "mu-"         
               //< G3 default value: 0.01 GeV
  kBCUTE,  //< electron bremsstrahlung         
               //< G4 particles: "gamma"         
               //< G3 default value: CUTGAM
  kBCUTM,  //< muon and hadron bremsstrahlung  
               //< G4 particles: "gamma"         
               //< G3 default value: CUTGAM
  kDCUTE,  //< delta-rays by electrons 
               //< G4 particles: "e-"         
               //< G3 default value: 10**4
  kDCUTM,  //< delta-rays by muons        
               //< G4 particles: "e-"         
               //< G3 default value: 10**4
  kPPCUTM, //< direct pair production by muons 
               //< G4 particles: "e-", "e+"         
               //< G3 default value: 0.01 GeV
  kTOFMAX, //< time of flight cut in seconds
               //< G4 particles: all
               //< G3 default value: 0.01 GeV
  kNoG3Cuts //< not valid value   
};

#endif //TG4_G3_CUT_H
