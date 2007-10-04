// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup global
/// \enum TG4ApplicationState
/// Enumeration for application states
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_APPLICATION_STATE_H
#define TG4_APPLICATION_STATE_H

enum TG4ApplicationState
{
  kPreInit,
  kConstructGeometry, 
  kConstructOpGeometry,
  kMisalignGeometry,
  kInitGeometry,
  kAddParticles,
  kInEvent,
  kNotInApplication
};

#endif //TG4_APPLICATION_STATE_H
