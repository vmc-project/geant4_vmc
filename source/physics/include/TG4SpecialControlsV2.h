#ifndef TG4_SPECIAL_CONTROLS_V2_H
#define TG4_SPECIAL_CONTROLS_V2_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SpecialControlsV2.h
/// \brief Definition of the TG4SpecialControlsV2 class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"
#include "TG4Globals.h"
#include "TG4G3Control.h"

#include <G4VProcess.hh>
#include <G4ProcessVector.hh>
#include <globals.hh>
#include <vector>

/// \ingroup physics
/// \brief The manager class for G3 process controls
///
/// This class takes care of activation/inactivation
/// of physics processes in current tracking medium according
/// to user setting via TVirtualMC::Gstpar() method.
///
/// Note that the global activation/inactivation of
/// physics processes via  TVirtualMC::SetProcess() method
/// is not managed by this class.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4SpecialControlsV2 : public TG4Verbose
{
  /// The action to be performed in the current step
  enum Switch {
    kSwitch,  ///< switch the process activation
    kReswitch,///< switch the process activation back
    kUnswitch ///< do not switch the process activation
  };

  public:
    TG4SpecialControlsV2();
    virtual ~TG4SpecialControlsV2();

    // methods
    void StartTrack(const G4Track* track);
    void ApplyControls();
    void RestoreProcessActivations();

    // get methods
    Bool_t IsApplicable() const;

  private:
    /// Not implemented
    TG4SpecialControlsV2(const TG4SpecialControlsV2& right);
    /// Not implemented
    TG4SpecialControlsV2& operator = (const TG4SpecialControlsV2& right);

    // methods
    void  SetSwitch();
    void  Reset();

    // data members

    /// Applicability for the current track
    G4bool  fIsApplicable;

    /// The current track
    const G4Track*  fkTrack;

    /// The action to be performed in the current step
    Switch fSwitch;

    /// Vector of the processes the activation of which is changed by this process
    G4ProcessVector  fSwitchedProcesses;

    /// Vector for storing the current values of the processes activation
    TG4boolVector  fSwitchedControls;

    /// vector of the origin process activations
    TG4boolVector  fProcessActivations;
};

inline Bool_t TG4SpecialControlsV2::IsApplicable() const {
  /// Return true if special controls are applicable for the current track
  return fIsApplicable;
}

#endif //TG4_SPECIAL_CONTROLS_V2_H

