// $Id: TG4MagneticFieldType.h,v 1.2 2004/11/10 11:39:27 brun Exp $
/// \ingroup geometry
//
/// \enum TG4MagneticFieldType
/// \brief Enumeration type for magnetic field types.
///
/// Author: I. Hrivnacova

#ifndef TG4_MAGNETIC_FIELD_TYPE_H
#define TG4_MAGNETIC_FIELD_TYPE_H

enum TG4MagneticFieldType { 
  kMCApplicationField, //< Field defined via MC Application 
  kUniformField,       //< Uniform magnetic field
  kNoField             //< No magnetic field
};

#endif //TG4_MAGNETIC_FIELD_TYPE_H
