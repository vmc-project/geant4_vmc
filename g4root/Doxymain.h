
// $Id$

/// \file Doxymain.h
/// \brief The main page for G4Root code documenation


/*! \mainpage G4Root interface

This package is an interface allowing running native GEANT4 with a ROOT
geometry. In fact it is just the G4 navigation that uses directly the
TGeo geometry. The package contains:

  -# A manager class: TG4RootNavMgr. This is a TObject-based class allowing
the connection of a TGeo geometry to a G4RunManager object.
  -# A special G4 user detector construction: TG4RootDetectorConstruction
The class takes a TGeo geometry and builds a GEANT4 logical hierarchy based
 on it. The class provides methods to acess GEANT4 created objects corresponding
 to TGeo ones.
  -# A user class TVirtualUserPostDetConstruction. Users should derive from this
 and implement the method Initialize() to connect G4 objects to the geometry
 (such as: sensitive detectors, user limits, magnetic field, ...)
  -# The navigation interface: TG4RootNavigator. This class derives from the base
 class G4Navigator and implements the navigation based on TGeo.
  -# TG4RootSolid - an interface class for using TGeo shapes as ROOT solids.

To use the interface, one has to use the following calling sequence:
 -# Get the geometry in memory:
\verbatim
  TGeoManager  *geom = TGeoManager::Import("mygeom.root");
  TG4RootNavMgr *mgr = TG4RootNavMgr::GetInstance(geom);
\endverbatim
 -# Create G4 run manager
\verbatim
  G4RunManager* runManager = new G4RunManager;
\endverbatim
 -# Instantiate user class
\verbatim
  MyUserPostDetConstruction *pdc = new MyUserPostDetConstruction();
\endverbatim
 -# Connect interface to run manager
\verbatim
  mgr->Initialize(pdc);
  mgr->ConnectToG4();
\endverbatim

*/
