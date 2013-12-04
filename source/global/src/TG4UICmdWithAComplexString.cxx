// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4UICmdWithAComplexString.cxx
/// \brief Implementation of the TG4UICmdWithAComplexString class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4UICmdWithAComplexString.h"

#include <sstream>

//_____________________________________________________________________________
TG4UICmdWithAComplexString::TG4UICmdWithAComplexString(
                               const G4String& theCommandPath, 
                               G4UImessenger* theMessenger)
  : G4UIcommand(theCommandPath, theMessenger)
{
/// Standard constructor.
/// The command string with full path directory
/// and the pointer to the messenger must be given.

  G4UIparameter* first  = new G4UIparameter('s');
  SetParameter(first);
  G4UIparameter* second = new G4UIparameter('s');
  SetParameter(second);
  G4UIparameter* third  = new G4UIparameter('s');
  SetParameter(third);
}

//_____________________________________________________________________________
TG4UICmdWithAComplexString::~TG4UICmdWithAComplexString() 
{
/// Destructor
}
                               
//
// public methods
//

//_____________________________________________________________________________
void TG4UICmdWithAComplexString::SetParameterName(const G4String& name, 
                                                  G4bool omittable)
{
/// Set the parameter names for the parameters. 
/// The "omittable" is set only for the first parameter, 
/// for the second and third it is always true.
/// The "currentAsDefault" flag is valid only if "omittable" is true.
/// If this flag is true, the current values are used as the default values
/// when user ommits the parameters. If this flag is false, the values
/// given by the next SetDefaultValue() method are used. 

  G4UIparameter* first = GetParameter(0);
  first->SetParameterName(name);
  first->SetOmittable(omittable);
  first->SetCurrentAsDefault(false);
  
  G4UIparameter* second = GetParameter(1);
  G4String secondName = name + "_cont1";
  second->SetParameterName(secondName);
  second->SetOmittable(true);
  second->SetCurrentAsDefault(false);

  G4UIparameter* third = GetParameter(2);
  G4String thirdName = name + "_cont2";
  third->SetParameterName(thirdName);
  third->SetOmittable(true);
  third->SetCurrentAsDefault(false);
}

//_____________________________________________________________________________
void TG4UICmdWithAComplexString::SetDefaultValue(const G4String& defaultValue)
{
/// Set the default values of the parameters.
/// These default values are used when user of this command ommits 
/// some of the parameter values, and "ommitable" is true and 
/// "currentAsDefault" is false.

  G4UIparameter* first = GetParameter(0);
  first->SetDefaultValue(defaultValue);

  G4UIparameter* second = GetParameter(1);
  second->SetDefaultValue(" ");

  G4UIparameter* third = GetParameter(2);
  third->SetDefaultValue(" ");
}

//_____________________________________________________________________________
G4String TG4UICmdWithAComplexString::GetNewStringValue(const G4String& paramString)
{
/// Return the parameter string 

  return paramString;
}

