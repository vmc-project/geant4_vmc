//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4CrossSectionType.cxx
/// \brief Definition of the enumeration TG4CrossSectionType
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4CrossSectionType.h"


//_____________________________________________________________________________
G4String TG4CrossSectionTypeName(G4int type)
{
  switch ( type ) {
    case kElastic:        return "Elastic";
    case kInelastic:      return "Inelastic";
    case kCapture:        return "Capture";
    case kFission:        return "Fission";
    case kChargeExchange: return "ChargeExchange";
    default:              return "Undefined";
  }
}

//_____________________________________________________________________________
TG4CrossSectionType GetCrossSectionType(G4int type)
{
  switch ( type ) {
    case kElastic:        return kElastic;
    case kInelastic:      return kInelastic;
    case kCapture:        return kCapture;
    case kFission:        return kFission;
    case kChargeExchange: return kChargeExchange;
    default:              return kNoCrossSectionType;
  }
}

//_____________________________________________________________________________
TG4CrossSectionType GetCrossSectionType(G4String name)
{
  if ( name == TG4CrossSectionTypeName(kElastic) )
    return kElastic;
  else if ( name == TG4CrossSectionTypeName(kInelastic) )
    return kInelastic;
  else if ( name == TG4CrossSectionTypeName(kCapture) )
    return kCapture;
  else if ( name == TG4CrossSectionTypeName(kFission) )
    return kFission;
  else if ( name == TG4CrossSectionTypeName(kChargeExchange) )
    return kChargeExchange;
  else
    return kNoCrossSectionType;
}

