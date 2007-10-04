// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup geometry
/// \typedef TG4OpSurfaceMap
/// \brief The map between optical surfaces names and their objects.
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_OP_SURFACE_MAP_H
#define TG4_OP_SURFACE_MAP_H

#include <G4OpticalSurface.hh>

typedef std::map<G4String, G4OpticalSurface*>  TG4OpSurfaceMap;

#endif
