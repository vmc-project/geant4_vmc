#ifndef TG4_G3_CONTROL_VECTOR_H
#define TG4_G3_CONTROL_VECTOR_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4G3ControlVector.h
/// \brief Definition of the TG4G3ControlVector class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4G3Control.h"
#include "TG4Globals.h"

#include <vector>

class TG4G3CutVector;

class G4VProcess;

/// \ingroup global
/// \brief Vector of control process values with convenient set/get methods.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4G3ControlVector
{
 public:
  /// The vector of TG4G3ControlValue
  typedef std::vector<TG4G3ControlValue> TG4ControlValueVector;

 public:
  TG4G3ControlVector();
  TG4G3ControlVector(const TG4G3ControlVector& right);
  ~TG4G3ControlVector();

  // operators
  TG4G3ControlVector& operator=(const TG4G3ControlVector& right);
  TG4G3ControlValue operator[](G4int index) const;

  // static methods
  static TG4G3Control GetControl(const G4String& controlName);
  static const G4String& GetControlName(TG4G3Control control);
  static TG4G3ControlValue GetControlValue(G4int value, TG4G3Control control);
  static TG4G3ControlValue GetControlValue(
    G4double value, TG4G3Control control);

  // set methods
  G4bool SetControl(
    TG4G3Control control, TG4G3ControlValue controlValue, TG4G3CutVector& cuts);
  void SetG3Defaults();
  G4bool Update(const TG4G3ControlVector& vector);

  // methods
  G4String Format() const;
  void Print() const;

  // get methods
  TG4G3ControlValue GetControlValue(G4VProcess* process) const;
  TG4G3ControlValue GetControlValue(TG4G3Control control) const;
  G4bool IsControl() const;

 private:
  // static methods
  static void FillControlNameVector();

  //
  // static data members

  /// vector of control parameters names
  static TG4StringVector fgControlNameVector;

  //
  // data members

  /// vector of control process values
  TG4ControlValueVector fControlVector;
};

#endif // TG4_G3_CONTROL_VECTOR_H
