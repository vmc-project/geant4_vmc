// $Id: TG4ModularPhysicsList.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4ModularPhysicsList
// ---------------------------
// See the class description in the header file.

#include "TG4ModularPhysicsList.h"
#include "TG4PhysicsConstructorGeneral.h"
#include "TG4PhysicsConstructorEM.h"
#include "TG4PhysicsConstructorMuon.h"
#include "TG4PhysicsConstructorHadron.h"
#include "TG4PhysicsConstructorIon.h"
#include "TG4PhysicsConstructorOptical.h"
#include "TG4PhysicsConstructorSpecialCuts.h"
#include "TG4PhysicsConstructorSpecialControls.h"
#include "TG4GeometryServices.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3ControlVector.h"
#include "TG4ProcessControlMap.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4BosonConstructor.hh>
#include <G4LeptonConstructor.hh>
#include <G4MesonConstructor.hh>
#include <G4BaryonConstructor.hh>
#include <G4IonConstructor.hh>
#include <G4ShortLivedConstructor.hh>
#include <G4ProcessTable.hh>

const G4double TG4ModularPhysicsList::fgkDefaultCutValue = 1.0 * mm;

//_____________________________________________________________________________
TG4ModularPhysicsList::TG4ModularPhysicsList()
  : G4VModularPhysicsList(),
    TG4Verbose("physicsList"),
    fMessenger(this),
    fSetEMPhysics(true),
    fSetMuonPhysics(true),
    fSetHadronPhysics(false),
    fSetOpticalPhysics(false),
    fSetSpecialCutsPhysics(false),
    fSetSpecialControlsPhysics(false)
{
//
  defaultCutValue = fgkDefaultCutValue;

  SetVerboseLevel(TG4VVerbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4ModularPhysicsList::TG4ModularPhysicsList(const TG4ModularPhysicsList& right)
  : TG4Verbose("physicsList"),
    fMessenger(this) {
//
  TG4Globals::Exception("TG4ModularPhysicsList is protected from copying.");
}

//_____________________________________________________________________________
TG4ModularPhysicsList::~TG4ModularPhysicsList() {
//
  //delete fExtDecayer;
       // fExtDecayer is deleted in G4Decay destructor
}

// operators

//_____________________________________________________________________________
TG4ModularPhysicsList& 
TG4ModularPhysicsList::operator=(const TG4ModularPhysicsList &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception("TG4ModularPhysicsList is protected from assigning.");

  return *this;
}

// private methods

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetProcessActivation(G4ProcessManager* processManager,
                                          G4int  processId, G4bool activation)
{				      
// Sets process activation for the given process.
// ---

  G4String strActivation = "activation";
  if (!activation) strActivation = "inactivation";

  if (TG4VVerbose::VerboseLevel() > 1) {
    G4cout << "Set process " << strActivation << " for " 
           << (*processManager->GetProcessList())[processId]->GetProcessName() 
	   << G4endl;
  }
  
  processManager->SetProcessActivation(processId, activation);	
}  

// protected methods

//_____________________________________________________________________________
void TG4ModularPhysicsList::ConstructParticle()
{
// In this method, static member functions should be called
// for all particles which you want to use.
// This ensures that objects of these particle types will be
// created in the program. 
// ---

  // lock physics manager
  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  g3PhysicsManager->Lock();  
  
  // create particles for registered physics
  G4VModularPhysicsList::ConstructParticle();
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::ConstructProcess()
{
// Constructs all processes.
// ---

  // create processes for registered physics
  G4VModularPhysicsList::ConstructProcess();

  // verbose
  if (verboseLevel>1) DumpAllProcesses();
}


// public methods

//_____________________________________________________________________________
void TG4ModularPhysicsList::Configure()
{
// Creates the selected physics constructors
// and registeres them in the modular physics list.
// ---

  Int_t verboseLevel = TG4VVerbose::VerboseLevel();

  // general physics
  RegisterPhysics(new TG4PhysicsConstructorGeneral(verboseLevel));

  // electromagnetic physics
  if (fSetEMPhysics)  
    RegisterPhysics(new TG4PhysicsConstructorEM(verboseLevel));

  // muon physics
  if (fSetMuonPhysics && fSetEMPhysics)      
    RegisterPhysics(new TG4PhysicsConstructorMuon(verboseLevel));

  // hadron physics
  if (fSetEMPhysics || fSetHadronPhysics) { 
    RegisterPhysics(new TG4PhysicsConstructorIon(
    		           verboseLevel, fSetEMPhysics, fSetHadronPhysics));
    RegisterPhysics(new TG4PhysicsConstructorHadron(
		           verboseLevel, fSetEMPhysics, fSetHadronPhysics));
  }  

  // optical physics
  if (fSetOpticalPhysics) 
    RegisterPhysics(new TG4PhysicsConstructorOptical(verboseLevel));

  // special processes
  if (fSetSpecialCutsPhysics) 
    RegisterPhysics(new TG4PhysicsConstructorSpecialCuts(verboseLevel));

  if (fSetSpecialControlsPhysics) 
    RegisterPhysics(new TG4PhysicsConstructorSpecialControls(verboseLevel));

  // warn about not allowed combinations
  if (fSetMuonPhysics && !fSetEMPhysics) {
    G4String text = "TG4PhysicsManager::CreatePhysicsConstructors:\n";
    text = text + "    Muon physics cannot be constructed without EM physics.\n";
    text = text + "    SetMuon control was ignored.";
    TG4Globals::Warning(text);     
  }

         // all created physics constructors are deleted
	 // in the base class destructor
}    

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetCuts()
{
// Sets the default cut value for all particle types
// other then e-/e+. 
// The cut value for e-/e+ is high in oredr to supress
// tracking of delta electrons.
// ---

  // SetCutsWithDefault();   
         // "G4VUserPhysicsList::SetCutsWithDefault" method sets 
         // the default cut value for all particle types.

  // default cut value
  G4double cut  = defaultCutValue;
  //G4double ecut = 10.*m; 
  G4double ecut = cut; 

  if (TG4VVerbose::VerboseLevel() > 1) {
    G4cout << "TG4ModularPhysicsList::SetCutsWithDefault:";
    G4cout << "CutLength : " << cut/mm << " (mm)" << G4endl;
  }  

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma 
  SetCutValue(cut, "gamma");
  SetCutValue(ecut, "e-");
  SetCutValue(ecut, "e+");
 
  // set cut values for proton and anti_proton before all other hadrons
  // because some processes for hadrons need cut values for proton/anti_proton 
  SetCutValue(cut, "proton");
  SetCutValue(cut, "anti_proton");
  
  if (verboseLevel>0) {
    DumpCutValuesTable();
  }
}


//_____________________________________________________________________________
void TG4ModularPhysicsList::VerboseLevel(G4int level) 
{
// Sets the specified level to both TG4Verbose and 
// G4VModularPhysicsList.
// ---

  TG4VVerbose::VerboseLevel(level);
  SetVerboseLevel(level);
}


//_____________________________________________________________________________
void TG4ModularPhysicsList::SetRangeCut(G4double value)
{
// Resets the default cut to a given value.
// !!! Should be used only in PreInit phase,
// use SetDefaultCutValue() method of base class to reset
// the cut value in later phases.

  defaultCutValue = value;
}  

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetProcessActivation()
{
// (In)Activates built processes according
// to the setup in TG4G3PhysicsManager::fControlVector.
// ---

  TG4G3ControlVector* controlVector 
    = TG4G3PhysicsManager::Instance()->GetControlVector();

  G4bool specialControls 
    = TG4GeometryServices::Instance()->IsSpecialControls();  

  if (!specialControls) 
    if (TG4VVerbose::VerboseLevel() > 0) {
      G4cout << G4endl
             << "### No special controls in user limits are set." << G4endl;
    }	     

  if (!controlVector) {
    G4String text = "TG4ModularPhysicsList::SetProcessActivation: \n";
    text = text + "    Vector of processes controls is not set.";
    TG4Globals::Warning(text);
    return;
  }    
  
  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ProcessManager* processManager 
      = theParticleIterator->value()->GetProcessManager(); 
      
    G4ProcessVector* processVector = processManager->GetProcessList();
  
    // set processes controls
    for (G4int i=0; i<processVector->length(); i++) {

      TG4G3ControlValue control
	 = controlVector->GetControlValue((*processVector)[i]);
      G4bool activation = processManager->GetProcessActivation(i);

      if (control != kUnset) {
        if (!TG4Globals::Compare(activation, control)) {

          // set new process activation
          G4bool activate;
          if (control == kInActivate) activate = false; 
	  else                        activate = true;
	  
	  SetProcessActivation(processManager, i, activate);         
        }
      }	
      else  {
        // control == kUnset
	if ((*processVector)[i]->GetProcessName().find("specialControl")
	    != G4String::npos) { 

	  SetProcessActivation(processManager, i, specialControls);         
        }  
      }	
    }
  }  
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::PrintAllProcesses() const
{
// Prints all processes.
// ---

  G4cout << "TG4ModularPhysicsList processes: " << G4endl;
  G4cout << "================================ " << G4endl;
 
  G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
  G4ProcessTable::G4ProcNameVector* processNameList 
    = processTable->GetNameList();

  for (G4int i=0; i<G4int(processNameList->size()); i++){
    G4cout << "   " << (*processNameList)[i] << G4endl;
  }  
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::DumpAllProcesses() const
{
// Dumps all particles and their processes.
// ---

  G4cout << "TG4ModularPhysicsList particles and processes: " << G4endl;
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
