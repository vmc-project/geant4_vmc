#ifndef TG4_UI_CMD_WITH_A_COMPLEX_STRING_H
#define TG4_UI_CMD_WITH_A_COMPLEX_STRING_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4UICmdWithAComplexString.h
/// \brief Definition of the TG4UICmdWithAComplexString class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UIcommand.hh>

/// \ingroup global
/// \brief G4 command that takes up to three string values.
///
/// Concrete class of G4UIcommand.
/// The command defined by this class takes up to three string values.
/// General information of G4UIcommand is given in G4UIcommand.hh.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4UICmdWithAComplexString : public G4UIcommand
{
  public:
    TG4UICmdWithAComplexString(const G4String& theCommandPath,
                               G4UImessenger* theMessenger);
    virtual ~TG4UICmdWithAComplexString();

    // set methods
    void SetParameterName(const G4String& name, G4bool omittable);
    void SetDefaultValue(const G4String& defVal);

    // get methods
    G4String GetNewStringValue(const G4String& paramString);
};

#endif //TG4_UI_CMD_WITH_A_COMPLEX_STRING_H
