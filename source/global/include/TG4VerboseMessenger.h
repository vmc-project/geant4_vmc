#ifndef TG4_VERBOSE_MESSENGER_H
#define TG4_VERBOSE_MESSENGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VerboseMessenger.h
/// \brief Definition of the TG4VerboseMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UImessenger.hh>
#include <globals.hh>
#include <vector>

class TG4VVerbose;

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAnInteger;

/// \ingroup global
/// \brief Messenger class that defines commands for the verbose classes.
///
/// For all objects derived from TG4Verbose class implements the command
/// - /mcVerbose/objectName [level]
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4VerboseMessenger: public G4UImessenger
{
  public:
    /// The vector of TG4 verbose objects
    typedef std::vector<TG4VVerbose*>           VerboseVector;

    /// The vector of G4 commands with an integer
    typedef std::vector<G4UIcmdWithAnInteger*>  CommandVector;

  public:
    TG4VerboseMessenger(const G4String& directoryName);
    virtual ~TG4VerboseMessenger();

    // methods
    G4UIcommand* AddCommand(TG4VVerbose* verbose, const G4String& cmdName);
            void RemoveCommand(TG4VVerbose* verbose, G4UIcommand* command);
    virtual void SetNewValue(G4UIcommand* command, G4String string);

  private:
    /// Not implemented
    TG4VerboseMessenger();
    /// Not implemented
    TG4VerboseMessenger(const TG4VerboseMessenger& right);
    /// Not implemented
    TG4VerboseMessenger& operator=(const TG4VerboseMessenger& right);

    // methods
    void SetNewValueToAll(const G4String value) const;

    // data members
    const G4String        fkDirectoryName;  ///< command directory name
    G4UIdirectory*        fDirectory;       ///< command directory
    G4UIcmdWithAnInteger* fGlobalVerboseCmd;///< global verbose command
    VerboseVector         fVerboseVector;   ///< associated verbose instances
    CommandVector         fCommandVector;   ///< verbose commands
};

#endif //TG4_VERBOSE_MESSENGER_H
