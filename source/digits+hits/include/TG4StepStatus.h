// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

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
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_STEP_STATUS_H
#define TG4_STEP_STATUS_H

enum TG4StepStatus { 
  kVertex,     //< in track vertex
  kBoundary,   //< when crossing geometrical boundary
  kNormalStep  //< in post step point
};

#endif //TG4_STEP_STATUS_H

