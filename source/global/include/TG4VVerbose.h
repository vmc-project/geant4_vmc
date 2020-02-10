#ifndef TG4_V_VERBOSE_H
#define TG4_V_VERBOSE_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VVerbose.h
/// \brief Definition of the TG4VVerbose class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <globals.hh>

class G4UImessenger;

/// \ingroup global
/// \brief Abstract base class for defining the verbose level
/// in a common way
///
/// Class defines the verbose level:
/// - 0 - no output
/// - 1 - minimal output (default)
/// - 2 and more - more detailed output
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4VVerbose
{
 public:
  TG4VVerbose();
  TG4VVerbose(G4int verboseLevel);
  virtual ~TG4VVerbose();

  // set methods
  virtual void VerboseLevel(G4int level);

  // get methods
  virtual G4int VerboseLevel() const;

 private:
  // methods
  /// Create messenger
  virtual G4UImessenger* CreateMessenger() = 0;

  // static data members
  static const G4int fgkDefaultVerboseLevel; ///< default verbose level

  // data members
  G4int fVerboseLevel; ///< verbose level
};

// inline methods

inline void TG4VVerbose::VerboseLevel(G4int level)
{
  /// Set level of verbosity
  fVerboseLevel = level;
}

inline G4int TG4VVerbose::VerboseLevel() const
{
  /// Return level of verbosity
  return fVerboseLevel;
}

#endif // TG4_V_VERBOSE_H
