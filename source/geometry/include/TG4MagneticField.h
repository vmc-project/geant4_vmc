//  $Id: TG4MagneticField.h,v 1.1 2002/06/20 11:55:24 hristov Exp $
// Category: geometry
//
// Author: I. Hrivnacova
//
// Class TG4MagneticField
// ----------------------
// Magnetic field - uses the TVirtualMCApplication field map. 

#ifndef TG4_MAGNETIC_FIELD_H
#define TG4_MAGNETIC_FIELD_H

#include <G4MagneticField.hh>
#include <globals.hh>

class TG4MagneticField : public G4MagneticField
{
  public:
    TG4MagneticField();
    virtual ~TG4MagneticField();

    void GetFieldValue(const G4double point[3], G4double* bfield) const;
};

#endif //TG4_MAGNETIC_FIELD_H

