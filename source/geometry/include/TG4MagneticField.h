//  $Id: TG4MagneticField.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup geometry
//
/// \class TG4MagneticField
/// 
/// Class that defines the magnetic field using the TVirtualMCApplication 
/// field map. 
///
/// Author: I. Hrivnacova

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

