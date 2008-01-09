#ifndef TG4_G3_CUT_H
#define TG4_G3_CUT_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4G3Cut.h
/// \brief Definition of the enumeration TG4G3Cut
///
/// \author I. Hrivnacova; IPN, Orsay

/// \ingroup global
/// \brief Enumeration for G3 types of kinetic energy cuts.

enum TG4G3Cut
{

  /// Gammas 
  /// - G4 particles: "gamma"         
  /// - G3 default value: 0.001 GeV
  kCUTGAM,
   
  /// Electrons        
  /// - G4 particles: "e-", "e+"
  /// - G3 default value: 0.001 GeV
  kCUTELE, 
  
  /// Neutral hadrons  
  /// - G4 particles: of type "baryon", "meson", "nucleus" with zero charge     
  /// - G3 default value: 0.01 GeV
  kCUTNEU, 
  
  /// Charged hadrons  
  /// - G4 particles: of type "baryon", "meson", "nucleus" with non-zero charge     
  /// - G3 default value: 0.01 GeV
  kCUTHAD,
  
  /// Muons            
  /// - G4 particles: "mu+", "mu-"         
  /// - G3 default value: 0.01 GeV
  kCUTMUO,
  
  /// Electron bremsstrahlung         
  /// - G4 particles: "gamma"         
  /// - G3 default value: CUTGAM
  kBCUTE, 

  /// Muon and hadron bremsstrahlung  
  /// - G4 particles: "gamma"         
  /// - G3 default value: CUTGAM
  kBCUTM,

  /// Delta rays by electrons 
  /// - G4 particles: "e-"         
  /// - G3 default value: 10**4 GeV
  kDCUTE,
  
  /// Delta rays by muons        
  /// - G4 particles: "e-"         
  /// - G3 default value: 10**4
  kDCUTM,

  /// Direct pair production by muons 
  /// - G4 particles: "e-", "e+"         
  /// - G3 default value: 0.01 GeV
  kPPCUTM,
  
  /// Time of flight cut in seconds
  /// - G4 particles: all
  /// - G3 default value: ??
  kTOFMAX, 
  
   /// Invalid value 
  kNoG3Cuts 
};

#endif //TG4_G3_CUT_H
