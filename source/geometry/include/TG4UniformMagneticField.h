// $Id: TG4UniformMagneticField.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup geometry
//
/// \class TG4UniformMagneticField
/// 
/// Uniform magnetic field.                                                  \n
/// According to:                                                            \n
/// Id: ExN02MagneticField.hh,v 1.1 1999/01/07 16:05:47 gunter Exp           \n
/// GEANT4 tag Name: geant4-00-01 
///
/// Author: I. Hrivnacova

#ifndef TG4_UNIFORM_MAGNETIC_FIELD_H
#define TG4_UNIFORM_MAGNETIC_FIELD_H

#include <G4UniformMagField.hh>

class G4FieldManager;

class TG4UniformMagneticField: public G4UniformMagField
{
  public:
    TG4UniformMagneticField();                   //  A zero field
    TG4UniformMagneticField(G4ThreeVector fied); //  The value of the field
    TG4UniformMagneticField(const TG4UniformMagneticField& right);
    virtual ~TG4UniformMagneticField();  
      
    // operators
    TG4UniformMagneticField& operator=(const TG4UniformMagneticField& right);

    // set methods
    void SetFieldValue(G4ThreeVector fieldVector);
    void SetFieldValue(G4double      fieldValue);
              // Set the field to (0, 0, fieldValue)

  protected:
    // Find the global Field Manager
    G4FieldManager* GetGlobalFieldManager() const; 
};

#endif //TG4_UNIFORM_MAGNETIC_FIELD_H
