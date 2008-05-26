#ifndef TG4_APPLICATION_STATE_H
#define TG4_APPLICATION_STATE_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ApplicationState.h 
/// \brief Definition of the enumeration TG4ApplicationState
///
/// \author I. Hrivnacova; IPN, Orsay

/// \ingroup global
/// \brief Enumeration for application states

enum TG4ApplicationState
{
  kPreInit,             ///< in PreInit
  kConstructGeometry,   ///< in ConstructGeometry
  kConstructOpGeometry, ///< in ConstructOpGeometry
  kMisalignGeometry,    ///< in MisalignGeometry
  kInitGeometry,        ///< in InitGeometry
  kAddParticles,        ///< in AddParticles
  kAddIons,             ///< in AddIons
  kInEvent,             ///< in event processing
  kNotInApplication     ///< not in VMC application
};

#endif //TG4_APPLICATION_STATE_H
