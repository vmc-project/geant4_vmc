// $Id: TG4SpecialPhysicsList.cxx,v 1.12 2007/05/31 10:24:33 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4SpecialPhysicsList
// ---------------------------
// See the class description in the header file.

#include "TG4SpecialPhysicsList.h"
#include "TG4PhysicsConstructorSpecialCuts.h"
#include "TG4PhysicsConstructorSpecialControls.h"
#include "TG4PhysicsConstructorStepLimiter.h"
#include "TG4PhysicsConstructorStackPopper.h"
#include "TG4GeometryServices.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3ControlVector.h"
#include "TG4ProcessControlMap.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessTable.hh>

//_____________________________________________________________________________
TG4SpecialPhysicsList::TG4SpecialPhysicsList(const TG4PhysicsListOptions& options)
  : G4VModularPhysicsList(),
    TG4Verbose("physicsList"),
    //fMessenger(this),
    fPhysicsConstructorStackPopper(0),
    fOptions(options)
 {
//
  Configure();

  SetVerboseLevel(TG4VVerbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4SpecialPhysicsList::TG4SpecialPhysicsList()
  : G4VModularPhysicsList(),
    TG4Verbose("physicsList"),
    //fMessenger(this),
    fPhysicsConstructorStackPopper(0),
    fOptions()
 {
//
  Configure();

  SetVerboseLevel(TG4VVerbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4SpecialPhysicsList::~TG4SpecialPhysicsList() {
//
  //delete fExtDecayer;
       // fExtDecayer is deleted in G4Decay destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4SpecialPhysicsList::Configure()
{
/// Create the selected physics constructors
/// and registeres them in the modular physics list.

  Int_t verboseLevel = TG4VVerbose::VerboseLevel();

  if ( fOptions.GetSpecialCutsPhysics() ) 
    RegisterPhysics(new TG4PhysicsConstructorSpecialCuts(verboseLevel));

  if ( fOptions.GetSpecialControlsPhysics() ) 
    RegisterPhysics(new TG4PhysicsConstructorSpecialControls(verboseLevel));

  if ( fOptions.GetStepLimiterPhysics() ) 
    RegisterPhysics(new TG4PhysicsConstructorStepLimiter(verboseLevel));

  if ( fOptions.GetStackPopperPhysics() ) {
    fPhysicsConstructorStackPopper
      = new TG4PhysicsConstructorStackPopper(verboseLevel); 
    RegisterPhysics(fPhysicsConstructorStackPopper);
  }  
}    

//_____________________________________________________________________________
void TG4SpecialPhysicsList::SetProcessActivation(G4ProcessManager* processManager,
                                          G4int  processId, G4bool activation)
{                                      
/// Set process activation for the given process.

  G4String strActivation = "Activate   ";
  if (!activation) strActivation = "Inactivate ";

  if (TG4VVerbose::VerboseLevel() > 1) {
    G4cout << strActivation << " process " 
           << (*processManager->GetProcessList())[processId]->GetProcessName() 
           << " for " << processManager->GetParticleType()->GetParticleName() 
           << G4endl;
  }
  
  processManager->SetProcessActivation(processId, activation);        
}  

//_____________________________________________________________________________
void TG4SpecialPhysicsList::SetSpecialControlsActivation()
{
/// (In)Activate built processes according
/// to the setup in TG4G3PhysicsManager::fControlVector.

  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  
  TG4G3ControlVector* controlVector = g3PhysicsManager->GetControlVector();
  TG4boolVector*    isControlVector = g3PhysicsManager->GetIsControlVector(); 

  if (!controlVector || !isControlVector) {
    TG4Globals::Exception(
      "TG4SpecialPhysicsList", "SetSpecialControlsActivation",
      "Vectors of processes controls is not set.");
    return;
  }    
  
  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* processManager = particle->GetProcessManager(); 
      
    G4ProcessVector* processVector = processManager->GetProcessList();
  
    // activate or inactivate processes according to 
    // global setting in the control vector in G3 physics manager
    //
    for (G4int i=0; i<processVector->length(); i++) {

      TG4G3ControlValue control
         = controlVector->GetControlValue((*processVector)[i]);
      G4bool activation = processManager->GetProcessActivation(i);
      
      if (control != kUnsetControlValue) {
         if (!TG4Globals::Compare(activation, control)) {

          // set new process activation
          G4bool activate;
          if (control == kInActivate) activate = false; 
          else                        activate = true;
          
          SetProcessActivation(processManager, i, activate);         
        }
      }
    }          
     
    // activate or inactivate the special controls processes according to 
    // setting in the isControl vector in G3 physics manager
    //
    //G4bool specialControls 
    //  = TG4GeometryServices::Instance()->IsSpecialControls();  
    G4bool specialControls 
      = g3PhysicsManager->IsSpecialControls();  
    TG4G3ParticleWSP particleWSP 
      = g3PhysicsManager->GetG3ParticleWSP(particle);

    if ( specialControls && particleWSP != kNofParticlesWSP ) { 
      // special process is activated in case
      // isControlVector in G3 physics manager is set
      // or the special control is set by TG4Limits
  
      // get the special cut process
      G4String processName = "specialControls";
      G4VProcess* process = g3PhysicsManager->FindProcess(processName);
      if (!process) {
        TG4Globals::Exception(
          "TG4SpecialPhysicsList", "SetSpecialCutsActivation",
          "The special control process for is not defined.");
      }
       
      G4int index = processManager->GetProcessIndex(process);
      SetProcessActivation(processManager, index, (*isControlVector)[particleWSP]);         
    }
  }  
}

//_____________________________________________________________________________
void TG4SpecialPhysicsList::SetSpecialCutsActivation()
{
/// (In)Activate built special cut processes according
/// to the setup in TG4G3PhysicsManager

  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  TG4boolVector* isCutVector = g3PhysicsManager->GetIsCutVector(); 

  if (!isCutVector) {
    TG4Globals::Exception(
      "TG4SpecialPhysicsList", "SetSpecialCutsActivation",
      "Vector of isCut booleans is not set.");
    return;
  }    
  
  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    TG4G3ParticleWSP particleWSP 
      = g3PhysicsManager->GetG3ParticleWSP(particle);
    G4String name =
      g3PhysicsManager->GetG3ParticleWSPName(particleWSP);

    if ( (particleWSP !=kNofParticlesWSP) ) { 
      // special process is activated in case
      // cutVector (vector of kinetic energy cuts) is set
      // or the special cut is set by TG4Limits
  
      G4ProcessManager* processManager 
        = theParticleIterator->value()->GetProcessManager(); 
      
      // get the special cut process
      G4String processName = "specialCutFor" + name;
      G4VProcess* process = g3PhysicsManager->FindProcess(processName);
      if (!process) {
        TG4Globals::Exception(
          "TG4SpecialPhysicsList", "SetSpecialCutsActivation",
          "The special cut process for " + TString(name) + " is not defined");
      }
       
      // special process is activated in case
      // cutVector (vector of kinetic energy cuts) is set
      // or the special cut is set by TG4Limits
      G4int index = processManager->GetProcessIndex(process);
      SetProcessActivation(processManager, index, (*isCutVector)[particleWSP]);         
    }
  }    
}

//
// public methods
//

//_____________________________________________________________________________
void TG4SpecialPhysicsList::ConstructProcess()
{
/// Construct all processes.

  // lock physics manager
  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  g3PhysicsManager->Lock();  

  // create processes for registered physics
  // To avoid call AddTransportation twice we do not call directly
  // G4VModularPhysicsList::ConstructProcess();
  // but call registered processes ourselves:
  G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    (*itr)->ConstructProcess();
  }

  // verbose
  if (verboseLevel>1) DumpAllProcesses();
}

//_____________________________________________________________________________
void TG4SpecialPhysicsList::SetCuts()
{
/// Do nothing for special processes
}

//_____________________________________________________________________________
G4int TG4SpecialPhysicsList::VerboseLevel() const 
{
/// Return verbose level (via TG4VVerbose)

  return TG4VVerbose::VerboseLevel();
}

//_____________________________________________________________________________
void TG4SpecialPhysicsList::VerboseLevel(G4int level) 
{
/// Set the specified level to both TG4Verbose and 
/// G4VModularPhysicsList.
/// The verbose level is also propagated to registered physics contructors.

  TG4VVerbose::VerboseLevel(level);
  SetVerboseLevel(level);
  
  G4PhysConstVector::iterator it;
  for ( it = physicsVector->begin(); it != physicsVector->end(); ++it ) {
    TG4Verbose* verbose = dynamic_cast<TG4Verbose*>(*it);
    verbose->VerboseLevel(level);
  }
}


//_____________________________________________________________________________
void TG4SpecialPhysicsList::PrintAllProcesses() const
{
/// Print all processes.

  G4cout << "TG4SpecialPhysicsList processes: " << G4endl;
  G4cout << "================================ " << G4endl;
 
  G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
  G4ProcessTable::G4ProcNameVector* processNameList 
    = processTable->GetNameList();

  for (G4int i=0; i<G4int(processNameList->size()); i++){
    G4cout << "   " << (*processNameList)[i] << G4endl;
  }  
}

//_____________________________________________________________________________
void TG4SpecialPhysicsList::DumpAllProcesses() const
{
/// Dump all particles and their processes.

  G4cout << "TG4SpecialPhysicsList particles and processes: " << G4endl;
  G4cout << "============================================== " << G4endl;
 
  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    // print particle name
    G4cout << "Particle: " 
           << theParticleIterator->value()->GetParticleName()
           << G4endl;

    // dump particle processes
    G4ProcessVector* processVector 
      = theParticleIterator->value()->GetProcessManager()->GetProcessList();
    for (G4int i=0; i<processVector->length(); i++)
      (*processVector)[i]->DumpInfo();
      
    G4cout << G4endl;  
  }  
}

//_____________________________________________________________________________
void TG4SpecialPhysicsList::SetProcessActivation()
{
/// (In)Activate built processes according
/// to the setup in TG4G3PhysicsManager

  if ( fOptions.GetSpecialControlsPhysics() &&
      (TG4G3PhysicsManager::Instance()->IsGlobalSpecialControls() ||
       TG4G3PhysicsManager::Instance()->IsSpecialControls()) ) {
       
    SetSpecialControlsActivation();
  }  
       
  if ( fOptions.GetSpecialCutsPhysics() &&
       TG4G3PhysicsManager::Instance()->IsSpecialCuts() ) {
  
    SetSpecialCutsActivation();
  }  
}

//_____________________________________________________________________________
void TG4SpecialPhysicsList::SetStackPopperSelection(const G4String& selection)
{
/// Select particles with stack popper process

  if ( !fPhysicsConstructorStackPopper ) {
    TG4Globals::Exception(
      "TG4SpecialPhysicsList", "SetStackPopperSelection",
      "SetStackPopper physics is not activated.");
  }  
  
  fPhysicsConstructorStackPopper->SetSelection(selection); 

  G4cout << "TG4SpecialPhysicsList::SetStackPopperSelection: " << selection << G4endl;
}   
 
