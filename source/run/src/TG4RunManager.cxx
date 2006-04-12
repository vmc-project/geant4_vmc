// $Id: TG4RunManager.cxx,v 1.10 2006/01/13 16:59:39 brun Exp $
// Category: run
//
// Class TG4RunManager
// -------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4RunManager.h"
#include "TG4RunConfiguration.h"
#include "TG4Globals.h"
#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4SDManager.h"
#include "TG4SDServices.h"
#include "TG4PhysicsManager.h"
#include "TG4G3PhysicsManager.h"
#include "TG4DetConstruction.h"
#include "TG4SDConstruction.h"
#include "TG4ModularPhysicsList.h"
#include "TG4PrimaryGeneratorAction.h"
#include "TG4RunAction.h"
#include "TG4EventAction.h"
#include "TG4TrackingAction.h"
#include "TG4SteppingAction.h"
#include "TG4SpecialStackingAction.h"

#include <G4RunManager.hh>
#include <G4UIsession.hh>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh> 
#include <G4UIXm.hh>
#include <G4UIXaw.hh>
#ifdef G4UI_USE_WO
#include <G4UIWo.hh>
#endif
#ifdef G4UI_USE_GAG
#include <G4UIGAG.hh>
#endif

#include <TROOT.h> 
#include <TRint.h>
#include <TCint.h> 
#include <TVirtualMCApplication.h>

TG4RunManager* TG4RunManager::fgInstance = 0;

//_____________________________________________________________________________
TG4RunManager::TG4RunManager(TG4RunConfiguration* runConfiguration, 
                             int argc, char** argv)		  
  : TG4Verbose("runManager"),
    fRunManager(0),
    fMessenger(this),
    fRunConfiguration(runConfiguration),
    fGeantUISession(0),
    fRootUISession(0),
    fRootUIOwner(false),
    fARGC(argc),
    fARGV(argv)  
{
// 
  if (fgInstance) {
    TG4Globals::Exception(
      "TG4RunManager: attempt to create two instances of singleton.");
  }
      
  if (!fRunConfiguration) {
    TG4Globals::Exception(
      "TG4RunManager: attempt to create instance without runConfiguration.");
  }
      
  fgInstance = this;
  
  if (VerboseLevel() > 1) {
    G4cout << "G4RunManager has been created." << G4endl;
  }  
  
  // filter out "-splash" from argument list
  FilterARGV("-splash");

  // create and configure G4 run manager
  fRunManager =  new G4RunManager();
  ConfigureRunManager();

  // create geant4 UI
  CreateGeantUI();
      // must be created before TG4VisManager::Initialize()
      // (that is invoked in TGeant4 constructor)

  // create root UI
  CreateRootUI();
}

//_____________________________________________________________________________
TG4RunManager::TG4RunManager(TG4RunConfiguration* runConfiguration)
  : TG4Verbose("runManager"),
    fRunManager(0),
    fMessenger(this),
    fRunConfiguration(runConfiguration),
    fGeantUISession(0),
    fRootUISession(0),
    fRootUIOwner(false),
    fARGC(0),
    fARGV(0)
  
{
//
  if (fgInstance) {
    TG4Globals::Exception(
      "TG4RunManager: attempt to create two instances of singleton.");
  }
      
  if (!fRunConfiguration) {
    TG4Globals::Exception(
      "TG4RunManager: attempt to create instance without runConfiguration.");
  }
      
  fgInstance = this;
  
  // set primary UI
  fRootUISession = gROOT->GetApplication();
  if (fRootUISession) {
    fARGC = fRootUISession->Argc();
    fARGV = fRootUISession->Argv();
  }

  // filter out "-splash" from argument list
  FilterARGV("-splash");

  // create and configure G4 run manager
  fRunManager =  new G4RunManager();
  ConfigureRunManager();

  if (VerboseLevel() > 1) {
    G4cout << "G4RunManager has been created." << G4endl;
  }  

  // create geant4 UI
  CreateGeantUI();
      // must be created before TG4VisManager::Initialize()
      // (that is invoked in TGeant4 constructor)

  // create root UI
  CreateRootUI();
}

//_____________________________________________________________________________
TG4RunManager::TG4RunManager()
  : TG4Verbose("runManager"),
    fMessenger(this)
{
//
}

//_____________________________________________________________________________
TG4RunManager::TG4RunManager(const TG4RunManager& right) 
  : TG4Verbose("runManager"),
    fMessenger(this) {
// 
  TG4Globals::Exception(
    "Attempt to copy TG4RunManager singleton.");
}

//_____________________________________________________________________________
TG4RunManager::~TG4RunManager() {
//  
  delete fRunConfiguration;
  delete fGeantUISession;
  delete fRunManager;
  if (fRootUIOwner) delete fRootUISession;
}

//
// operators
//

//_____________________________________________________________________________
TG4RunManager& TG4RunManager::operator=(const TG4RunManager& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception(
    "Attempt to assign TG4RunManager singleton.");
    
  return *this;  
}    

//
// private methods
//

//_____________________________________________________________________________
void TG4RunManager::ConfigureRunManager()
{
/// Set the user action classes defined by TG4RunConfiguration to G4RunManager.

  // set mandatory classes
  //
  fRunManager
    ->SetUserInitialization(fRunConfiguration->CreateDetectorConstruction());
  fRunManager
    ->SetUserInitialization(fRunConfiguration->CreatePhysicsList());
  fRunManager
    ->SetUserAction(fRunConfiguration->CreatePrimaryGenerator());      

  // user other action classes 
  //
  G4UserRunAction* runAction = fRunConfiguration->CreateRunAction();
  if ( runAction ) fRunManager->SetUserAction(runAction);  

  G4UserEventAction* eventAction = fRunConfiguration->CreateEventAction();
  if ( eventAction ) fRunManager->SetUserAction(eventAction); 
    
  TG4TrackingAction* trackingAction = fRunConfiguration->CreateTrackingAction();
  if ( trackingAction) fRunManager->SetUserAction(trackingAction); 
    
  TG4SteppingAction* steppingAction = fRunConfiguration->CreateSteppingAction();
  if ( steppingAction) fRunManager->SetUserAction(steppingAction);

  G4UserStackingAction* stackingAction = fRunConfiguration->CreateStackingAction(); 
  if ( stackingAction) fRunManager->SetUserAction(stackingAction);
}

//_____________________________________________________________________________
void TG4RunManager::CreateGeantUI()
{
/// Create interactive Geant4.

  if (!fGeantUISession)
  {
    // create geant4 UI
    G4UImanager* pUI = G4UImanager::GetUIpointer();  
    if (fARGC == 1) {
#ifdef G4UI_USE_GAG
      fGeantUISession = new G4UIGAG();
#else
      fGeantUISession = new G4UIterminal(new G4UItcsh);      
#endif      
    }  
    else if (strcmp (fARGV[1], "dumb") == 0) {
      fGeantUISession = new G4UIterminal(); 
    }
#ifdef G4UI_USE_WO
    else if (strcmp (fARGV[1], "Wo") == 0) {
      fGeantUISession = new G4UIWo(fARGC, fARGV); 
    }
#endif
#ifdef G4UI_USE_XM
    else if (strcmp (fARGV[1], "Xm") == 0) {
      fGeantUISession = new G4UIXm(fARGC, fARGV); 
    }
#endif
#ifdef G4UI_USE_XAW
    else if (strcmp (fARGV[1], "Xaw") == 0) {
      fGeantUISession = new G4UIXaw(fARGC, fARGV); 
    }
#endif 
#ifdef G4UI_USE_GAG
    else if (strcmp (fARGV[1], "GAG") == 0) {
      fGeantUISession = new G4UIGAG (); 
    }
#endif 
    if (fGeantUISession) {   
      pUI->SetSession(fGeantUISession); 
    }
  }
}

//_____________________________________________________________________________
void TG4RunManager::CreateRootUI()
{
/// Create interactive Root.

  if (!fRootUISession) 
  {
    // create session if it does not exist  
    fRootUISession = new TRint("aliroot", 0, 0, 0, 0);

    // set ownership of Root UI
    fRootUIOwner = true;
  }
}

//_____________________________________________________________________________
void TG4RunManager::FilterARGV(const G4String& arg)
{
/// Filter out the option argument from the arguments list fARGV,
/// if present.

  if (fARGC == 1) return;

  G4bool isArg = false;
  for (G4int i=0; i<fARGC; i++) {
    if (G4String(fARGV[i]) == arg) isArg = true;
    if (isArg && i+1 < fARGC) fARGV[i] = fARGV[i+1];  
  }

  if (isArg) fARGC--;
} 
 
 // public methods

//_____________________________________________________________________________
void TG4RunManager::Initialize()
{
/// Initialize G4.

  // initialize Geant4
  fRunManager->Initialize();

  // finish geometry
  TG4GeometryManager::Instance()->FinishGeometry();
  
  // initialize SD manager
  TG4SDManager::Instance()->Initialize();
}

//_____________________________________________________________________________
void TG4RunManager::LateInitialize()
{
/// Finish initialization of G4 after the G4Run initialization
/// is finished. 

  // define particles 
  TG4PhysicsManager::Instance()->DefineParticles();

  // set user limits
  TG4GeometryManager::Instance()
    ->SetUserLimits(*TG4G3PhysicsManager::Instance()->GetCutVector(),
                    *TG4G3PhysicsManager::Instance()->GetControlVector());

  // clear G3 tables
  TG4GeometryManager::Instance()->ClearG3TablesFinal();
      
  // activate/inactivate physics processes
  TG4PhysicsManager::Instance()->SetProcessActivation();

  // print statistics
  TG4GeometryServices::Instance()->PrintStatistics(true, false);  
  TG4SDServices::Instance()->PrintStatistics(false, true);  

  if (VerboseLevel() > 2) {
    TG4GeometryServices::Instance()->PrintLogicalVolumeStore();  
  }
}

//_____________________________________________________________________________
void TG4RunManager::ProcessEvent()
{
/// Not yet implemented.

  TG4Globals::Warning("TG4RunManager::ProcessEvent(): is not yet implemented.");
}
    
//_____________________________________________________________________________
Bool_t TG4RunManager::ProcessRun(G4int nofEvents)
{
/// Process Geant4 run.

  fRunManager->BeamOn(nofEvents); 

  G4bool result = ! TG4SDServices::Instance()->GetIsStopRun();
  TG4SDServices::Instance()->SetIsStopRun(false);
  
  return result;
}
    
//_____________________________________________________________________________
void TG4RunManager::StartGeantUI()
{ 
/// Start interactive/batch Geant4.

  if (!fGeantUISession) CreateGeantUI();
  if (fGeantUISession) {  
    // interactive session
    G4cout << "Welcome back in Geant4" << G4endl;
    fGeantUISession->SessionStart();
    G4cout << "Welcome back in Root" << G4endl;  
  }
  else {
    // execute Geant4 macro if file is specified as an argument 
    G4String fileName = fARGV[1];
    ProcessGeantMacro(fileName);
  }
}

//_____________________________________________________________________________
void TG4RunManager::StartRootUI()
{
/// Start interactive Root.

  if (!fRootUISession) CreateRootUI();
  if (fRootUISession) { 
    G4cout << "Welcome back in Root" << G4endl;
    fRootUISession->Run(kTRUE);
    G4cout << "Welcome back in Geant4" << G4endl;  
  }
}
 
//_____________________________________________________________________________
void TG4RunManager::ProcessGeantMacro(G4String macroName)
{
/// Process Geant4 macro.

  G4String command = "/control/execute " + macroName;
  ProcessGeantCommand(command);
}
 
//_____________________________________________________________________________
void TG4RunManager::ProcessRootMacro(G4String macroName)
{
/// Process Root macro.

  // load macro file
  G4String macroFile = macroName;
  macroFile.append(".C");
  gROOT->LoadMacro(macroFile);

  // execute macro function
  G4String macroFunction = macroName;
  macroFunction.append("()");
  gInterpreter->ProcessLine(macroFunction);
}
 
//_____________________________________________________________________________
void TG4RunManager::ProcessGeantCommand(G4String command)
{
/// Process Geant4 command.

  G4UImanager* pUI = G4UImanager::GetUIpointer();  
  pUI->ApplyCommand(command);
}

//_____________________________________________________________________________
void TG4RunManager::ProcessRootCommand(G4String command)
{
/// Process Root command.

  gInterpreter->ProcessLine(command);
}

//_____________________________________________________________________________
void TG4RunManager::UseG3Defaults() 
{
/// Control G3 defaults usage.

  TG4G3PhysicsManager::Instance()->SetG3DefaultCuts();
  TG4G3PhysicsManager::Instance()->SetG3DefaultControls();
}

//_____________________________________________________________________________
Int_t TG4RunManager::CurrentEvent() const
{
/// Return the number of the current event.

  G4int eventID = fRunManager->GetCurrentEvent()->GetEventID();
  return eventID;
}

//_____________________________________________________________________________
Bool_t  TG4RunManager::SecondariesAreOrdered() const 
{
///  Secondaries are ordered if the special stacking
/// (defined in TG4SpecialStackingAction) is activated.
// ----

  return fRunConfiguration->IsSpecialStacking();
}  

