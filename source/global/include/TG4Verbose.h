#ifndef TG4_VERBOSE_H
#define TG4_VERBOSE_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4Verbose.h
/// \brief Definition of the TG4Verbose class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VVerbose.h"
#include "TG4VerboseMessenger.h"

#include <globals.hh>

class G4UIcommand;

/// \ingroup global
/// \brief Base class for defining the verbose level and
/// a common messenger
///
/// Class defines the verbose level and the static messenger
/// (common for all instances).
/// Used as a base class for all Geant4 VMC verbose classes;
/// enables to handle the standard output in a common way.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4Verbose : public TG4VVerbose
{
  public:
    TG4Verbose(const G4String& cmdName);
    TG4Verbose(const G4String& cmdName, G4int verboseLevel);
    virtual ~TG4Verbose();

  private:
    /// Not implemented
    TG4Verbose();
    /// Not implemented
    TG4Verbose(const TG4Verbose& right);
    /// Not implemented
    TG4Verbose& operator=(const TG4Verbose& right);

    // methods
    virtual TG4VerboseMessenger* CreateMessenger();

    // static data members
    static const G4String        fgkDirectoryName;///< directory name
    static G4ThreadLocal G4int   fgCounter;       ///< object counter
    static G4ThreadLocal TG4VerboseMessenger*  fgMessenger;///< messenger

    // data members
    G4UIcommand*                 fCommand;        ///< verbose command
};

#endif //TG4_VERBOSE_H
