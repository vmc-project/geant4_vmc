// $Id: TG4LVTreeMessenger.h,v 1.4 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup geometry
/// \class TG4LVTreeMessenger
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

#ifndef TG4_LV_TREE_MESSENGER_H
#define TG4_LV_TREE_MESSENGER_H

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4LVTree;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4LogicalVolume;

class TG4LVTreeMessenger: public G4UImessenger
{
  public:
    TG4LVTreeMessenger(TG4LVTree* lvTree);
    virtual ~TG4LVTreeMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);

  protected:
    TG4LVTreeMessenger();
    TG4LVTreeMessenger(const TG4LVTreeMessenger& right);
    TG4LVTreeMessenger& operator=(const TG4LVTreeMessenger &right);
             
  private:
    // data members
    TG4LVTree*                fLVTree;               //associated class
    G4LogicalVolume*          fCurrentVolume;        //current logical volume
    G4UIdirectory*            fDirectory;            //command directory
    G4UIcmdWithAString*       fSetCurrentLVCmd;      //command: setVolume
    G4UIcmdWithoutParameter*  fListCmd;              //command: list
    G4UIcmdWithoutParameter*  fListLongCmd;          //command: listLong
    G4UIcmdWithAString*       fListDaughtersCmd;     //command: listDaughters
    G4UIcmdWithAString*       fListLongDaughtersCmd; //command: listLongDaughters  

#ifdef G4VIS_USE
    G4UIcmdWithABool*         fSetLVTreeVisibilityCmd; //command: setLVTreeVisibility   
    G4UIcmdWithABool*         fSetVolVisibilityCmd;    //command: setVolVisibility
    G4UIcmdWithAString*       fSetLVTreeColourCmd;     //command: setLVTreeColour  
    G4UIcmdWithAString*       fSetVolColourCmd;        //command: setVolColour
#endif //G4VIS_USE
};

#endif //TG4_LV_TREE_MESSENGER_H

