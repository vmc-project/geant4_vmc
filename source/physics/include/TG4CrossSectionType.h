#ifndef TG4_OPTICAL_PROCESS_H
#define TG4_OPTICAL_PROCESS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4CrossSectionType.h
/// \brief Definition of the enumeration TG4CrossSectionType
///
/// \author I. Hrivnacova; IPN, Orsay

/// \ingroup physics
/// \brief Enumeration for cross section types

#include <globals.hh>

enum TG4CrossSectionType {
  kElastic,        ///< elastic cross section
  kInelastic,      ///< inelastic cross section
  kCapture,        ///< capture cross section
  kFission,        ///< fission cross section
  kChargeExchange, ///< charge exchane cross  section
  kNoCrossSectionType  ///< no cross section type
};

/// Return name for given cross section type
G4String TG4CrossSectionTypeName(G4int type);

/// Convert G4int to TG4CrossSectionType (for loops)
TG4CrossSectionType GetCrossSectionType(G4int type);

/// Return the cross section type by name
TG4CrossSectionType GetCrossSectionType(G4String name);

#endif //TG4_OPTICAL_PROCESS_H
