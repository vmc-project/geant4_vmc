// $Id: $
/// \ingroup geometry
//
/// \class TG4OpSurfaceMap
/// 
/// The typedef for the map between optical surfaces names and their objects.
///
/// Author: I. Hrivnacova

#ifndef TG4_OP_SURFACE_MAP_H
#define TG4_OP_SURFACE_MAP_H

#include <G4OpticalSurface.hh>

typedef std::map<G4String, G4OpticalSurface*>  TG4OpSurfaceMap;

#endif
