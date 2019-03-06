#ifndef TG4_G3_DEFAULTS_H
#define TG4_G3_DEFAULTS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4G3Defaults.h
/// \brief Definition of the TG4G3Defaults class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4G3CutVector.h"
#include "TG4G3ControlVector.h"
#include "TG4G3Control.h"
#include "TG4G3Cut.h"

#include <globals.hh>

/// \ingroup global
/// \brief Default G3 kinetic energy cuts and control process values
///
/// Class provides the default G3 values of the kinetic energy cuts
/// for particles and the control process parameters.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4G3Defaults
{
  public:
    TG4G3Defaults();
    virtual ~TG4G3Defaults();

    // static access methods
    static TG4G3Defaults* Instance();

    // methods
    G4bool IsDefaultCut(TG4G3Cut cut, G4double value) const;
    G4bool IsDefaultControl(TG4G3Control control, TG4G3ControlValue value) const;

    // get methods
    G4double CutValue(G4int cut) const;
    TG4G3ControlValue ControlValue(G4int control) const;

  private:
    /// Not implemented
    TG4G3Defaults(const TG4G3Defaults& right);
    /// Not implemented
    TG4G3Defaults& operator=(const TG4G3Defaults& right);

    // static data members
    static G4ThreadLocal TG4G3Defaults*  fgInstance; ///< this instance

    // data members
    TG4G3CutVector     fCutVector;     ///< vector of default cut values
    TG4G3ControlVector fControlVector; ///< vector of default control values
};

// inline methods

inline TG4G3Defaults* TG4G3Defaults::Instance() {
  /// Return this instance
  return fgInstance;
}

#endif //ALI_G3_DEFAULTS_H
