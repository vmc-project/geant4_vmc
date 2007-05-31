// $Id: TG4RunMessenger.h,v 1.4 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup run
/// \class TG4RunMessenger
/// \brief Messenger class that defines commands for TG4RunManager
///
/// Implements commands:
/// - /mcControl/root
/// - /mcControl/rootMacro [macroName]
/// - /mcControl/rootCmd [cmdString]
/// - /mcControl/g3Defaults
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_RUN_MESSENGER_H
#define TG4_RUN_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4RunManager;
class TG4UICmdWithAComplexString;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;

class TG4RunMessenger: public G4UImessenger
{
  public:
    TG4RunMessenger(TG4RunManager* runManager);
    virtual ~TG4RunMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4RunMessenger();  
    TG4RunMessenger(const TG4RunMessenger& right);
    TG4RunMessenger& operator=(const TG4RunMessenger& right);

    // data members
    TG4RunManager*  fRunManager; //associated class   
    G4UIdirectory*  fDirectory;  //command directory

    G4UIcmdWithoutParameter*    fRootCmd;        //command: root
    G4UIcmdWithAString*         fRootMacroCmd;   //command: rootMacro 
    TG4UICmdWithAComplexString* fRootCommandCmd; //command: rootCmd 
    G4UIcmdWithoutParameter*    fG3DefaultsCmd;  //command: g3Defaults   
};

#endif //TG4_RUN_MESSENGER_H
