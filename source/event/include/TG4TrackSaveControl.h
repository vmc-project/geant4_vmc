#ifndef TG4_TRACK_SAVE_CONTROL_H
#define TG4_TRACK_SAVE_CONTROL_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4TrackSaveControl.h
/// \brief Definition of the enumeration TG4TrackSaveControl
///
/// \author I. Hrivnacova; IPN, Orsay

/// \ingroup event
/// \brief Enumeration for options for storing secondary tracks

#include <globals.hh>

enum TG4TrackSaveControl { 
  kDoNotSave,       ///< do not save
  kSaveInPreTrack,  ///< save in pre-track
  kSaveInStep,      ///< save in step
};

#endif //TRACK_SAVE_CONTROL
