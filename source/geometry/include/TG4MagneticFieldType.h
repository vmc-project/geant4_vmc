#ifndef TG4_MAGNETIC_FIELD_TYPE_H
#define TG4_MAGNETIC_FIELD_TYPE_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4MagneticFieldType.h
/// \brief Definition of the enumeration TG4MagneticFieldType
///
/// \author I. Hrivnacova; IPN, Orsay

/// \ingroup geometry
/// \brief Enumeration type for magnetic field types.

enum TG4MagneticFieldType { 
  kMCApplicationField, ///<  Field defined via MC Application 
  kUniformField,       ///<  Uniform magnetic field
  kNoField             ///<  No magnetic field
};

#endif //TG4_MAGNETIC_FIELD_TYPE_H
