#ifndef A01_RUN_CONFIGURATION1_H
#define A01_RUN_CONFIGURATION1_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01RunConfiguration.h
/// \brief Definition of the A01RunConfiguration class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunConfiguration.h"

/// \ingroup A01
/// \brief User Geant4 VMC run configuration
///
/// This class overrides setting of the default Geant4 VMC geometry
/// construction with Geant4 detector construction
///
/// \author I. Hrivnacova; IPN, Orsay

class A01RunConfiguration : public TG4RunConfiguration
{
  public:
    A01RunConfiguration(const TString& physicsList = "emStandard",
                        const TString& specialProcess = "stepLimiter");
    virtual ~A01RunConfiguration();

    // methods
    virtual G4VUserDetectorConstruction*  CreateDetectorConstruction();

    // set methods
    void  SetUseLocalMagField(Bool_t localMagField);

  private:
    /// Option to use local magnetic field
    Bool_t  fUseLocalMagField;
};

// inline functions

/// Set the option to use local magnetic field (working only with Geant4 !)
/// \param localMagField  The new value of the option
inline void A01RunConfiguration::SetUseLocalMagField(Bool_t localMagField)
{ fUseLocalMagField = localMagField; }

#endif //A01_RUN_CONFIGURATION1_H
