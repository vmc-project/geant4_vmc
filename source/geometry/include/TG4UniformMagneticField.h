#ifndef TG4_UNIFORM_MAGNETIC_FIELD_H
#define TG4_UNIFORM_MAGNETIC_FIELD_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4UniformMagneticField.h
/// \brief Definition of the TG4UniformMagneticField class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UniformMagField.hh>

class G4FieldManager;

/// \ingroup geometry
/// \brief Uniform magnetic field. 
///
/// According to:                                                            \n
/// Id: ExN02MagneticField.hh,v 1.1 1999/01/07 16:05:47 gunter Exp           \n
/// GEANT4 tag Name: geant4-00-01 
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4UniformMagneticField: public G4UniformMagField
{
  public:
    TG4UniformMagneticField(); 
    TG4UniformMagneticField(G4ThreeVector fied);
    TG4UniformMagneticField(const TG4UniformMagneticField& right);
    virtual ~TG4UniformMagneticField();  
      
    // operators
    TG4UniformMagneticField& operator=(const TG4UniformMagneticField& right);

    // set methods
    void SetFieldValue(G4ThreeVector fieldVector);
    void SetFieldValue(G4double      fieldValue);

  protected:
    G4FieldManager* GetGlobalFieldManager() const; 
};

#endif //TG4_UNIFORM_MAGNETIC_FIELD_H
