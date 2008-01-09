#ifndef TG4_LV_TREE_MESSENGER_H
#define TG4_LV_TREE_MESSENGER_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4LVTreeMessenger.h
/// \brief Definition of the TG4LVTreeMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4LVTree;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4LogicalVolume;

/// \ingroup geometry
/// \brief The messenger class that defines commands for TG4LVTree.
///
/// Implements commands:
/// - /mcTree/setVolume [volName]
/// - /mcTree/list
/// - /mcTree/listLong
/// - /mcTree/setLVTreeVisibility [true|false]
/// - /mcTree/setVolVisibility [true|false]
/// - /mcTree/setLVTreeColour [colourName]
/// - /mcTree/setVolColour [colourName]
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4LVTreeMessenger: public G4UImessenger
{
  public:
    TG4LVTreeMessenger(TG4LVTree* lvTree);
    virtual ~TG4LVTreeMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);

  private:
    /// Not implemented
    TG4LVTreeMessenger();
    /// Not implemented
    TG4LVTreeMessenger(const TG4LVTreeMessenger& right);
    /// Not implemented
    TG4LVTreeMessenger& operator=(const TG4LVTreeMessenger &right);

    //
    // data members
    
    /// associated class
    TG4LVTree*                fLVTree;
    
    /// current logical volume
    G4LogicalVolume*          fCurrentVolume;
    
    /// command directory
    G4UIdirectory*            fDirectory;
    
    /// command directory
    G4UIcmdWithAString*       fSetCurrentLVCmd;

    /// command: list
    G4UIcmdWithoutParameter*  fListCmd;
    
    /// command: listLong
    G4UIcmdWithoutParameter*  fListLongCmd;
    
    /// command: listDaughters
    G4UIcmdWithAString*       fListDaughtersCmd;
    
    /// command: listLongDaughters  
    G4UIcmdWithAString*       fListLongDaughtersCmd;

#ifdef G4VIS_USE
    /// command: setLVTreeVisibility   
    G4UIcmdWithABool*         fSetLVTreeVisibilityCmd;
    
    /// command: setVolVisibility
    G4UIcmdWithABool*         fSetVolVisibilityCmd;
    
    /// command: setLVTreeColour      
    G4UIcmdWithAString*       fSetLVTreeColourCmd;
    
    /// command: setVolColour
    G4UIcmdWithAString*       fSetVolColourCmd;
#endif //G4VIS_USE
};

#endif //TG4_LV_TREE_MESSENGER_H

