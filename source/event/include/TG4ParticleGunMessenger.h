// $Id: TG4ParticleGunMessenger.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup event
//
/// \class TG4ParticleGunMessenger
/// 
/// Messenger class that defines commands for TG4ParticleGun.
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
    // --> protected
    // TG4ParticleGunMessenger();
    // TG4ParticleGunMessenger(const TG4ParticleGunMessenger& right);
    virtual ~TG4ParticleGunMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    virtual G4String GetCurrentValue(G4UIcommand* command);

  protected:
    TG4ParticleGunMessenger();
    TG4ParticleGunMessenger(const TG4ParticleGunMessenger& right);

    // operators
    TG4ParticleGunMessenger& operator=(
                            const TG4ParticleGunMessenger& right);

  private:
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


