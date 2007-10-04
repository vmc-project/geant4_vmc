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
/// \enum TG4MagneticFieldType
/// \brief Enumeration type for magnetic field types.
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_MAGNETIC_FIELD_TYPE_H
#define TG4_MAGNETIC_FIELD_TYPE_H

enum TG4MagneticFieldType { 
  kMCApplicationField, //< Field defined via MC Application 
  kUniformField,       //< Uniform magnetic field
  kNoField             //< No magnetic field
};

#endif //TG4_MAGNETIC_FIELD_TYPE_H
