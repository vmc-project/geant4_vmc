// $Id: TG4VerboseMessenger.h,v 1.6 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup global
//
/// \class TG4VerboseMessenger
/// \brief Messenger class that defines commands for the verbose classes.
///
/// For all objects derived from TG4Verbose class implements the command 
/// - /mcVerbose/objectName [level]
///
/// \author I. Hrivnacova; IPN, Orsay


#ifndef TG4_VERBOSE_MESSENGER_H
#define TG4_VERBOSE_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>
#include <vector>

class TG4VVerbose;

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAnInteger;

class TG4VerboseMessenger: public G4UImessenger
{
  typedef std::vector<TG4VVerbose*>           VerboseVector;
  typedef std::vector<G4UIcmdWithAnInteger*>  CommandVector;

  public:
    TG4VerboseMessenger(const G4String& directoryName);
    virtual ~TG4VerboseMessenger();
   
    // methods 
    G4UIcommand* AddCommand(TG4VVerbose* verbose, const G4String& cmdName);
            void RemoveCommand(TG4VVerbose* verbose, G4UIcommand* command);
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4VerboseMessenger();  
    TG4VerboseMessenger(const TG4VerboseMessenger& right);
    TG4VerboseMessenger& operator=(const TG4VerboseMessenger& right);

    // methods
    void SetNewValueToAll(const G4String value) const;
  
    // data members
    const G4String        fkDirectoryName;  //command directory name
    G4UIdirectory*        fDirectory;       //command directory
    G4UIcmdWithAnInteger* fGlobalVerboseCmd;//global verbose command 
    VerboseVector         fVerboseVector;   //associated verbose instances
    CommandVector         fCommandVector;   //verbose commands
};

#endif //TG4_VERBOSE_MESSENGER_H
