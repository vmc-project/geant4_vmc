// $Id: TG4LVTreeMessenger.h,v 1.2 2004/11/10 11:39:27 brun Exp $
/// \ingroup geometry
//
/// \class TG4LVTreeMessenger
/// \brief The messenger class that defines commands for TG4LVTree.
///
/// Author: I. Hrivnacova

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
    // --> protected   
    // TG4LVTreeMessenger();
    // TG4LVTreeMessenger(const TG4LVTreeMessenger& right);
    virtual ~TG4LVTreeMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);

  protected:
    TG4LVTreeMessenger();
    TG4LVTreeMessenger(const TG4LVTreeMessenger& right);

    // operators
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

