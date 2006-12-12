// $Id: TG4ParticleGunMessenger.h,v 1.3 2005/09/01 10:04:32 brun Exp $
/// \ingroup event
//
/// \class TG4ParticleGunMessenger
/// \brief Messenger class that defines commands for TG4ParticleGun.
///
/// Author: I. Hrivnacova

#ifndef TG4_PARTICLE_GUN_MESSENGER_H
#define TG4_PARTICLE_GUN_MESSENGER_H

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4ParticleGun;
class TG4GunParticle;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;

class TG4ParticleGunMessenger: public G4UImessenger
{
  public:
    TG4ParticleGunMessenger(TG4ParticleGun* gun);
    virtual ~TG4ParticleGunMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    virtual G4String GetCurrentValue(G4UIcommand* command);

  private:
    TG4ParticleGunMessenger();
    TG4ParticleGunMessenger(const TG4ParticleGunMessenger& right);
    TG4ParticleGunMessenger& operator=(
                            const TG4ParticleGunMessenger& right);

    // data members
    TG4ParticleGun*   fGun;           //associated class
    TG4GunParticle*   fParticle;      //current TG4GunParticle
 
    // commands data members
    G4UIdirectory*              fGunDirectory;      //command directory
    G4UIcmdWithoutParameter*    fListAvailableCmd;  //command: listAvailable
    G4UIcmdWithoutParameter*    fListCurrentCmd;    //command: listCurrent
    G4UIcmdWithAString*         fParticleCmd;       //command: particle
    G4UIcmdWith3VectorAndUnit*  fMomentumCmd;       //command: momentum
    G4UIcmdWith3VectorAndUnit*  fPositionCmd;       //command: position
    G4UIcmdWithADoubleAndUnit*  fTimeCmd;           //command: time
    G4UIcmdWith3Vector*         fPolarizationCmd;   //command: polarization
    G4UIcmdWith3Vector*         fDirectionCmd;      //command: direction 
    G4UIcmdWithADoubleAndUnit*  fKinEnergyCmd;      //command: kinEnergy
    G4UIcmdWithoutParameter*    fListCmd;           //command: list
    G4UIcmdWithoutParameter*    fAddParticleCmd;    //command: addParticle
    G4UIcmdWithAnInteger*       fRemoveParticleCmd; //command: removeParticle
    G4UIcmdWithoutParameter*    fResetCmd;          //command: reset
};

#endif //TG4_PARTICLE_GUN_MESSENGER_H


