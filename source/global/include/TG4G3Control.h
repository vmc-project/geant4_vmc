#ifndef TG4_G3_CONTROL_H
#define TG4_G3_CONTROL_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4G3Control.h
/// \brief Definition of the enumerations TG4G3Control, TG4G3ControlValue
///
/// \author I. Hrivnacova; IPN, Orsay

/// \ingroup global
/// \brief Enumeration for G3 types of physics processes controls.
///
/// The G4 physics processes and particles, which the process
/// control is applied to, are indicated in the comments.
/// The process control is mapped to the physics processes
/// (using TG4ProcessControlMap) at the moment of process creating
/// by the physics constructor; the physics contructor type
/// is indicated by the "Physics:" comment.

enum TG4G3Control
{
  /// Pair production       
  /// - G3 default value: 1
  /// - G4 processes: G4GammaConversion, 
  ///                 G4MuPairProduction/G4IMuPairProduction
  ///                 G4LowEnergyGammaConversion
  /// - Particles: gamma, mu
  /// - Physics:   EM  
  kPAIR,  

  /// Compton scattering    
  /// - G3 default value: 1
  /// - G4 processes: G4ComptonScattering, 
  ///                 G4LowEnergyCompton,
  ///                 G4PolarizedComptonScattering
  /// - Particles: gamma
  /// - Physics:   EM  
  kCOMP, 

  /// Photo electric effect 
  /// - G3 default value: 1
  /// - G4 processes: G4PhotoElectricEffect
  ///                 G4LowEnergyPhotoElectric
  /// - Particles: gamma
  /// - Physics:   EM  
  kPHOT, 

  /// Photofission          
  /// - G3 default value: 0
  /// - G4 process: ??
  /// - Particles: gamma
  /// - Physics:   ??  
  kPFIS, 

  /// Delta rays              
  /// - G3 default value: 2
  /// - G4 treats delta rays in different way
  /// - G4 processes: G4eIonisation/G4IeIonization,
  ///                 G4MuIonisation/G4IMuIonization, 
  ///                 G4hIonisation/G4IhIonisation
  /// - Particles: charged 
  /// - Physics:   EM  
  kDRAY, 

  /// Annihilation          
  /// - G3 default value: 1
  /// - G4 processes: G4eplusAnnihilation/G4IeplusAnnihilation
  /// - Particles: e+ 
  /// - Physics:   EM  
  kANNI, 
             
  /// Bremsstrahlung        
  /// - G3 default value: 1
  /// - G4 processes: G4eBremsstrahlung/G4IeBremsstrahlung,
  ///                 G4MuBremsstrahlung/G4IMuBremsstrahlung,
  ///                 G4LowEnergyBremstrahlung
  /// - Particles: e-/e+; mu+/mu- 
  /// - Physics:   EM  
  kBREM, 
             
  /// Hadronic process      
  /// - G3 default value: 1
  /// - G4 processes: defined by G4 hadronic physics lists
  /// - Particles: hadrons 
  /// - Physics:   Hadron
  kHADR,
             
  /// Muon nuclear interaction 
  /// - G3 default value: 0
  /// - G4 processes: G4MuNuclearInteraction,
  ///                 G4MuonMinusCaptureAtRest
  /// - Particles: mu
  /// - Physics:   Not set
  kMUNU, 
             
  /// Decay                 
  /// - G3 default value: 1
  /// - G4 process: G4Decay
  /// - Particles: all which decay is applicable for
  /// - Physics:   General
  kDCAY, 
             
  /// Energy loss           
  /// - G3 default value: 2
  /// - G4 processes: G4eIonisation/G4IeIonization,
  ///                 G4MuIonisation/G4IMuIonization, 
  ///                 G4hIonisation/G4IhIonisation
  /// - Particles: charged 
  /// - Physics:   EM  
  kLOSS,             

  /// Multiple scattering   
  /// - G3 default value: 1
  /// - G4 process: G4MultipleScattering/G4IMultipleScattering
  /// - Particles: charged 
  /// - Physics:   EM  
  kMULS, 

  /// Cerenkov photon generation
  /// - G3 default value: 0
  /// - G4 process: G4Cerenkov
  /// - Particles: charged  
  /// - Physics:   Optical  
  kCKOV, 
           
  /// Rayleigh scattering
  /// - G3 default value: 0             
  /// - G4 process: G4OpRayleigh
  /// - Particles: optical photon  
  /// - Physics:   Optical  
  kRAYL,
             
  /// Light photon absorption
  /// - it is turned on when Cerenkov process is turned on
  /// - G3 default value: 0             
  /// - G4 process: G4OpAbsorption, G4OpBoundaryProcess
  /// - Particles: optical photon  
  /// - Physics:   Optical  
  kLABS, 

  /// Synchrotron radiation in magnetic field           
  /// - G3 default value: 0             
  /// - G4 process: G4SynchrotronRadiation
  /// - Particles: ??
  /// - Physics:   Not set  
  kSYNC, 

  /// No process control
  kNoG3Controls
};

/// \ingroup global
/// \brief Enumeration for G3 processes control values.
///
/// In G3 the process control values meaning can be different for
/// different processes, but for most of them is:
/// -  0  process is not activated
/// -  1  process is activated WITH generation of secondaries
/// -  2  process is activated WITHOUT generation of secondaries
/// if process does not generate secondaries => 1 same as 2
///
/// Exceptions:
/// -  MULS:  also 3
/// -  LOSS:  also 3, 4 
/// -  RAYL:  only 0,1
/// -  HADR:  may be > 2
enum TG4G3ControlValue 
{
  /// value not set 
  kUnsetControlValue = -1,
  
  /// process is not activated
  kInActivate = 0,
  
  /// process is activated WITH generation of secondaries
  kActivate   = 1,

  /// process is activated WITHOUT generation of secondaries 
  kActivate2  = 2 
}; 

#endif //TG4_G3_CONTROL_H

