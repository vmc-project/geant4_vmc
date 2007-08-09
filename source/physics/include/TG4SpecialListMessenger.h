// $Id: TG4SpecialListMessenger.h,v 1.9 2007/05/31 10:24:32 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4SpecialListMessenger
/// \brief Messenger class that defines commands for the Geant4 VMC special 
/// physics list.
///
/// Implements commands:
/// - /mcPhysics/setStackPopperSelection [particleName1 particleName2 ...]
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_SPECIAL_LIST_MESSENGER_H
#define TG4_SPECIAL_LIST_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4SpecialPhysicsList;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class TG4SpecialListMessenger: public G4UImessenger
{
  public:
    TG4SpecialListMessenger(TG4SpecialPhysicsList* physicsList); 
    virtual ~TG4SpecialListMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4SpecialListMessenger();  
    TG4SpecialListMessenger(const TG4SpecialListMessenger& right);
    TG4SpecialListMessenger& operator=(const TG4SpecialListMessenger& right);

    // data members
    TG4SpecialPhysicsList* fPhysicsList;     //associated class

    // commands    
    G4UIcmdWithAString*    fSetStackPopperSelectionCmd;                                                   
};                                           //setStackPopperSelection command

#endif //TG4_PHYSICS_LIST_MESSENGER_H
