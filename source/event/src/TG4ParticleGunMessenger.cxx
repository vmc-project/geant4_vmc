// $Id: TG4ParticleGunMessenger.cxx,v 1.3 2004/11/10 11:39:27 brun Exp $
// Category: event
//
// Class TG4ParticleGunMessenger
// -----------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4ParticleGunMessenger.h"
#include "TG4ParticleGun.h"
#include "TG4GunParticle.h"
#include "TG4Globals.h"

#include <G4Geantino.hh>
#include <G4ThreeVector.hh>
#include <G4ParticleTable.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>

//_____________________________________________________________________________
TG4ParticleGunMessenger::TG4ParticleGunMessenger(TG4ParticleGun* gun)
  : G4UImessenger(),
    fGun(gun),
    fParticle(0),
    fGunDirectory(0),
    fListAvailableCmd(0),
    fListCurrentCmd(0),
    fParticleCmd(0),
    fMomentumCmd(0),
    fPositionCmd(0),
    fTimeCmd(0),
    fPolarizationCmd(0),
    fDirectionCmd(0),
    fKinEnergyCmd(0),
    fListCmd(0),
    fAddParticleCmd(0),
    fRemoveParticleCmd(0),
    fResetCmd(0)
    
{
//
  fGunDirectory = new G4UIdirectory("/mcGun/");
  fGunDirectory->SetGuidance("TG4ParticleGun control commands.");

  fListAvailableCmd 
    = new G4UIcmdWithoutParameter("/mcGun/listAvailable", this);
  fListAvailableCmd->SetGuidance("List available particles.");
  fListAvailableCmd->SetGuidance(" Invoke G4ParticleTable.");
  fListAvailableCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fListCurrentCmd 
    = new G4UIcmdWithoutParameter("/mcGun/listCurrent", this);
  fListCurrentCmd->SetGuidance("List current particle properties.");
  fListCurrentCmd
    ->SetGuidance("(Use addParticle to add this particle to the gun.");
  fListCurrentCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fParticleCmd 
    = new G4UIcmdWithAString("/mcGun/particle", this);
  fParticleCmd->SetGuidance("Set particle to be generated.");
  fParticleCmd->SetGuidance(" (geantino is default)");
  fParticleCmd->SetParameterName("particleName", true);
  fParticleCmd->SetDefaultValue("geantino");
  G4String candidateList; 
  G4int nofPTEntries = G4ParticleTable::GetParticleTable()->entries();
  for (G4int i=0; i<nofPTEntries; i++)
  {
    candidateList += G4ParticleTable::GetParticleTable()->GetParticleName(i);
    candidateList += " ";
  }
  fParticleCmd->SetCandidates(candidateList);
  fParticleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fMomentumCmd 
    = new G4UIcmdWith3VectorAndUnit("/mcGun/momentum", this);
  fMomentumCmd->SetGuidance("Set momentum.");
  fMomentumCmd->SetParameterName("Px","Py","Pz", true, true);
  fMomentumCmd->SetDefaultUnit("MeV");
  fMomentumCmd->SetUnitCategory("Energy"); 
  fMomentumCmd->SetRange("Px != 0 || Py != 0 || Pz != 0");
  fMomentumCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fPositionCmd 
    = new G4UIcmdWith3VectorAndUnit("/mcGun/position", this);
  fPositionCmd->SetGuidance("Set starting position of the particle.");
  fPositionCmd->SetParameterName("X","Y","Z", true, true);
  fPositionCmd->SetDefaultUnit("cm");
  fPositionCmd->SetUnitCategory("Length");
  fPositionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fTimeCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcGun/time", this);
  fTimeCmd->SetGuidance("Set initial time of the particle.");
  fTimeCmd->SetParameterName("t0", true, true);
  fTimeCmd->SetDefaultUnit("ns");
  fTimeCmd->SetUnitCategory("Time");
  fTimeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  fPolarizationCmd 
    = new G4UIcmdWith3Vector("/mcGun/polarization", this);
  fPolarizationCmd->SetGuidance("Set polarization.");
  fPolarizationCmd->SetParameterName("Px","Py","Pz", true, true); 
  fPolarizationCmd
    ->SetRange("Px>=-1. && Px<=1. && Py>=-1. && Py<=1. && Pz>=-1. && Pz<=1.");
  fPolarizationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fDirectionCmd 
    = new G4UIcmdWith3Vector("/mcGun/direction", this);
  fDirectionCmd->SetGuidance("Set momentum direction.");
  fDirectionCmd->SetGuidance("Direction needs not to be a unit vector.");
  fDirectionCmd->SetParameterName("Dx","Dy","Dz", true, true); 
  fDirectionCmd->SetRange("Dx != 0 || Dy != 0 || Dz != 0");
  fDirectionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  fKinEnergyCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcGun/kinEnergy", this);
  fKinEnergyCmd->SetGuidance("Set kinetic energy.");
  fKinEnergyCmd->SetParameterName("KineticEnergy", true, true);
  fKinEnergyCmd->SetDefaultUnit("GeV");
  fKinEnergyCmd->SetUnitCategory("Energy");
  fKinEnergyCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fListCmd 
    = new G4UIcmdWithoutParameter("/mcGun/list",this);
  fListCmd->SetGuidance("List the Alice gun particles.");
  fListCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fAddParticleCmd 
    = new G4UIcmdWithoutParameter("/mcGun/addParticle", this);
  fAddParticleCmd->SetGuidance("Add the particle to the Alice particle gun.");
  fAddParticleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fRemoveParticleCmd 
    = new G4UIcmdWithAnInteger("/mcGun/removeParticle", this);
  fRemoveParticleCmd
    ->SetGuidance("Remove the i-th particle friom the Alice particle gun.");
  fRemoveParticleCmd->SetParameterName("iParticle", false);
  fRemoveParticleCmd->SetRange("iParticle>=0");
  fRemoveParticleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fResetCmd 
    = new G4UIcmdWithoutParameter("/mcGun/reset", this);
  fResetCmd->SetGuidance("ReSet the Alice particle gun.");
  fResetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set initial value to TG4GunParticle
  fParticle = new TG4GunParticle();

  fParticle->SetParticleDefinition(G4Geantino::Geantino());
  fParticle->SetMomentumDirection(G4ThreeVector(1.0,0.0,0.0));
  fParticle->SetKineticEnergy(1.0*GeV);
  fParticle->SetPosition(G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm));
  fParticle->SetTime(0.0*ns);
  fParticle->SetPolarization(G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm));
}

//_____________________________________________________________________________
TG4ParticleGunMessenger::~TG4ParticleGunMessenger() {
//
  delete fListAvailableCmd;
  delete fParticleCmd;
  delete fMomentumCmd;
  delete fPositionCmd;
  delete fTimeCmd;
  delete fPolarizationCmd;
  delete fDirectionCmd;
  delete fKinEnergyCmd;
  delete fListCmd;
  delete fAddParticleCmd;
  delete fRemoveParticleCmd;
  delete fResetCmd;
  delete fGunDirectory;
  delete fParticle;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4ParticleGunMessenger::SetNewValue(G4UIcommand * command, 
       G4String newValues)
{
/// Apply command to the associated object.

  // Alice gun particle set commands
  if (command==fListAvailableCmd)
  { 
    G4ParticleTable::GetParticleTable()->DumpTable(); 
  } 
  else if (command == fParticleCmd)
  {
    G4ParticleDefinition* particleDef 
      = G4ParticleTable::GetParticleTable()->FindParticle(newValues);
    if (particleDef != 0)
    { fParticle->SetParticleDefinition(particleDef); }
  } 
  else if (command == fListCurrentCmd)
  { 
    fParticle->Print(); 
  } 
  else if (command == fMomentumCmd)
  { 
    fParticle->SetMomentum(fMomentumCmd->GetNew3VectorValue(newValues)); 
  } 
  else if (command == fPositionCmd)
  { 
    fParticle->SetPosition(fDirectionCmd->GetNew3VectorValue(newValues)); 
  }
  else if (command == fTimeCmd)
  { 
    fParticle->SetTime(fTimeCmd->GetNewDoubleValue(newValues)); 
  }
  else if (command == fPolarizationCmd)
  { 
    fParticle
      ->SetPolarization(fPolarizationCmd->GetNew3VectorValue(newValues)); 
  } 
  else if (command == fDirectionCmd)
  { 
    fParticle
      ->SetMomentumDirection(fDirectionCmd->GetNew3VectorValue(newValues)); 
  } 
  else if (command == fKinEnergyCmd)
  { 
    fParticle->SetKineticEnergy(fKinEnergyCmd->GetNewDoubleValue(newValues)); 
  }

  // Alice particle gun commands 
  else if (command == fListCmd)
  { 
    fGun->List(); 
  }
  else if (command == fAddParticleCmd)
  { 
    fGun->AddParticle(fParticle);
    fParticle = new TG4GunParticle(*fParticle); 
  } 
  else if (command == fRemoveParticleCmd)
  { 
    fGun->RemoveParticle(fRemoveParticleCmd->GetNewIntValue(newValues)); 
  }
  else if (command == fResetCmd)
  { 
    fGun->Reset(); 
  }
}

//_____________________________________________________________________________
G4String TG4ParticleGunMessenger::GetCurrentValue(G4UIcommand * command)
{
/// Return current command parameters as string.

  G4String curValue;

  if( command==fDirectionCmd )
  { 
    curValue 
      = fDirectionCmd->ConvertToString(fParticle->GetMomentumDirection()); 
  } 
  else if( command==fKinEnergyCmd )
  { 
    curValue 
      = fKinEnergyCmd->ConvertToString(fParticle->GetKineticEnergy(),"GeV"); 
  } 
  else if( command==fPositionCmd )
  { 
    curValue = fPositionCmd->ConvertToString(fParticle->GetPosition(),"cm"); 
  } 
  else if( command==fTimeCmd )
  { 
    curValue = fTimeCmd->ConvertToString(fParticle->GetTime(),"ns"); 
  } 
  else if( command==fPolarizationCmd )
  { 
    curValue = fPolarizationCmd->ConvertToString(fParticle->GetPolarization()); 
  }
  
 return curValue;
}

