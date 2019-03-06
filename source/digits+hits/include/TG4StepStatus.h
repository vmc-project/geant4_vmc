#ifndef TG4_STEP_STATUS_H
#define TG4_STEP_STATUS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4StepStatus.h
/// \brief Definition of the enumeration TG4StepStatus
///
/// \author I. Hrivnacova; IPN, Orsay

/// \ingroup digits_hits
/// \enum TG4StepStatus
/// \brief Step status enumeration
///
/// In orded to take into account different stepping
/// mechanism in G3 and G4 three states of TG4StepManager
/// are defined:
///  - kVertex     - returns track properties in the vertex
///                  position before particle started stepping
///  - kBoundary   - returns track properties when particle
///                  is crossing geometrical boundary
///                  (the volume that preceeds the boundary is returned
///                   as current volume)
///  - kNormalStep - returns track properties in a post step
///                  point
///  - kGflashStep - returns track properties in a Gflash spot
///                  point
enum TG4StepStatus {
  kVertex,     ///<  in track vertex
  kBoundary,   ///<  when crossing geometrical boundary
  kNormalStep, ///<  in post step point
  kGflashSpot  ///<  in post step point with Gflash
};

#endif //TG4_STEP_STATUS_H

