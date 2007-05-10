// $Id: TG4PhysicsListMessenger.h,v 1.7 2006/12/12 16:21:16 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsListMessenger
/// \brief Messenger class that defines commands for the Geant4 VMC default 
/// physics list.
///
/// Author: I. Hrivnacova

#ifndef TG4_PHYSICS_LIST_MESSENGER_H
#define TG4_PHYSICS_LIST_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4ModularPhysicsList;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class TG4PhysicsListMessenger: public G4UImessenger
{
  public:
    TG4PhysicsListMessenger(TG4ModularPhysicsList* physicsList); 
    virtual ~TG4PhysicsListMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4PhysicsListMessenger();  
    TG4PhysicsListMessenger(const TG4PhysicsListMessenger& right);
    TG4PhysicsListMessenger& operator=(const TG4PhysicsListMessenger& right);

    // data members
    TG4ModularPhysicsList*  fPhysicsList;     //associated class

    // commands    
    G4UIcmdWithADouble*       fRangeCutCmd;   //rangeCut command  
    G4UIcmdWithoutParameter*  fProcessActivationCmd; //.
                                              //setProcessActivation command
    G4UIcmdWithAnInteger*     fSetCerenkovMaxPhotonsCmd; 
                                              //setCerenkovMaxPhotons command                                                  
    G4UIcmdWithAString*       fSetStackPopperSelectionCmd;                                                   
};                                            //setStackPopperSelection command

#endif //TG4_PHYSICS_LIST_MESSENGER_H
