#ifndef TG4_OP_SURFACE_MAP_H
#define TG4_OP_SURFACE_MAP_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4OpSurfaceMap.h
/// \brief Definition of the TG4OpSurfaceMap typedef 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4OpticalSurface.hh>

/// \ingroup geometry
/// \brief The map between optical surfaces names and their objects.

typedef std::map<G4String, G4OpticalSurface*>  TG4OpSurfaceMap;

#endif
