// $Id: TG4MagneticFieldType.h,v 1.1 2002/06/20 11:55:24 hristov Exp $
// Category: geometry
//
// Author: I. Hrivnacova
//
// Enum TG4MagneticFieldType
// -------------------------
// Enumeration type for distinguishing between detector
// and structure modules.

#ifndef TG4_MAGNETIC_FIELD_TYPE_H
#define TG4_MAGNETIC_FIELD_TYPE_H

enum TG4MagneticFieldType { 
  kMCApplicationField,
  kUniformField, 
  kNoField
};

#endif //TG4_MAGNETIC_FIELD_TYPE_H
