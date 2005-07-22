// $Id: TG4ModularPhysicsList.cxx,v 1.6 2005/05/17 13:43:57 brun Exp $
// Category: physics
//
// Class TG4ModularPhysicsList
// ---------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4ModularPhysicsList.h"
#include "TG4PhysicsConstructorGeneral.h"
#include "TG4PhysicsConstructorEM.h"
#include "TG4PhysicsConstructorMuon.h"
#include "TG4PhysicsConstructorHadron.h"
#include "TG4PhysicsConstructorIon.h"
#include "TG4PhysicsConstructorOptical.h"
#include "TG4PhysicsConstructorSpecialCuts.h"
#include "TG4PhysicsConstructorSpecialControls.h"
#include "TG4PhysicsConstructorStepLimiter.h"
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
    fPhysicsConstructorOptical(0),
    fSetEMPhysics(true),
    fSetMuonPhysics(true),
    fSetHadronPhysics(false),
    fSetOpticalPhysics(false),
    fSetSpecialCutsPhysics(false),
    fSetSpecialControlsPhysics(false),
    fSetStepLimiterPhysics(true)
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

//
// operators
//

//_____________________________________________________________________________
TG4ModularPhysicsList& 
TG4ModularPhysicsList::operator=(const TG4ModularPhysicsList &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception("TG4ModularPhysicsList is protected from assigning.");

  return *this;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetProcessActivation(G4ProcessManager* processManager,
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
void TG4ModularPhysicsList::SetSpecialControlsActivation()
{
/// (In)Activate built processes according
/// to the setup in TG4G3PhysicsManager::fControlVector.

  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  
  TG4G3ControlVector* controlVector = g3PhysicsManager->GetControlVector();
  TG4boolVector*    isControlVector = g3PhysicsManager->GetIsControlVector(); 

  if (!controlVector || !isControlVector) {
    G4String text = "TG4ModularPhysicsList::SetSpecialControlsActivation: \n";
    text = text + "    Vectors of processes controls is not set.";
    TG4Globals::Exception(text);
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
        G4String text = "TG4ModularPhysicsList::SetSpecialCutsActivation: \n";
        text = text + "    The special control process for is not defined";
        TG4Globals::Exception(text);
      }
       
      G4int index = processManager->GetProcessIndex(process);
      SetProcessActivation(processManager, index, (*isControlVector)[particleWSP]);         
    }
  }  
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetSpecialCutsActivation()
{
/// (In)Activate built special cut processes according
/// to the setup in TG4G3PhysicsManager

  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  TG4boolVector* isCutVector = g3PhysicsManager->GetIsCutVector(); 

  if (!isCutVector) {
    G4String text = "TG4ModularPhysicsList::SetSpecialCutsActivation: \n";
    text = text + "    Vector of isCut booleans is not set.";
    TG4Globals::Exception(text);
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
        G4String text = "TG4ModularPhysicsList::SetSpecialCutsActivation: \n";
        text = text + "    The special cut process for ";
	text = text + name;
	text = text + " is not defined";
        TG4Globals::Exception(text);
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
void TG4ModularPhysicsList::Configure()
{
/// Create the selected physics constructors
/// and registeres them in the modular physics list.

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
  if (fSetOpticalPhysics) {
    fPhysicsConstructorOptical 
      = new TG4PhysicsConstructorOptical(verboseLevel);
    RegisterPhysics(fPhysicsConstructorOptical);
  }  

  // special processes
  if (fSetSpecialCutsPhysics) 
    RegisterPhysics(new TG4PhysicsConstructorSpecialCuts(verboseLevel));

  if (fSetSpecialControlsPhysics) 
    RegisterPhysics(new TG4PhysicsConstructorSpecialControls(verboseLevel));

  if (fSetStepLimiterPhysics) 
    RegisterPhysics(new TG4PhysicsConstructorStepLimiter(verboseLevel));

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
void TG4ModularPhysicsList::ConstructProcess()
{
/// Construct all processes.

  // lock physics manager
  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  g3PhysicsManager->Lock();  

  // create processes for registered physics
  G4VModularPhysicsList::ConstructProcess();

  // verbose
  if (verboseLevel>1) DumpAllProcesses();
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetCuts()
{
/// Set the default cut value for all particle types.

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
/// Set the specified level to both TG4Verbose and 
/// G4VModularPhysicsList.

  TG4VVerbose::VerboseLevel(level);
  SetVerboseLevel(level);
}


//_____________________________________________________________________________
void TG4ModularPhysicsList::PrintAllProcesses() const
{
/// Print all processes.

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
/// Dump all particles and their processes.

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
//_____________________________________________________________________________
void TG4ModularPhysicsList::SetRangeCut(G4double value)
{
/// Reset the default cut to a given value.                                 \n
/// !!! Should be used only in PreInit phase,
/// use SetDefaultCutValue() method of base class to reset
/// the cut value in later phases.

  defaultCutValue = value;
}  

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetProcessActivation()
{
/// (In)Activate built processes according
/// to the setup in TG4G3PhysicsManager

  if ( fSetSpecialControlsPhysics &&
      (TG4G3PhysicsManager::Instance()->IsGlobalSpecialControls() ||
       TG4G3PhysicsManager::Instance()->IsSpecialControls()) ) {
       
    SetSpecialControlsActivation();
  }  
       
  if ( fSetSpecialCutsPhysics &&
       TG4G3PhysicsManager::Instance()->IsSpecialCuts() ) {
  
    SetSpecialCutsActivation();
  }  
}

//_____________________________________________________________________________
void TG4ModularPhysicsList::SetMaxNumPhotonsPerStep(G4int maxNumPhotons)
{
/// Limit step to the specified maximum number of Cherenkov photons

  if ( !fPhysicsConstructorOptical ) {
    G4String text = "TG4ModularPhysicsList::SetMaxNumPhotonsPerStep: \n";
    text = text + "    Optical physics is not activated.";
    TG4Globals::Exception(text);
  }  
  
  fPhysicsConstructorOptical->SetMaxNumPhotonsPerStep(maxNumPhotons); 
}    

