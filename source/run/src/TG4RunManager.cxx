//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RunManager.cxx
/// \brief Implementation of the TG4RunManager class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunManager.h"
#include "TG4RunConfiguration.h"
#include "TG4Globals.h"
#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4SDManager.h"
#include "TG4SDServices.h"
#include "TG4PhysicsManager.h"
#include "TG4G3PhysicsManager.h"
#include "TG4StateManager.h"
#include "TG4ActionInitialization.h"
#include "TG4WorkerInitialization.h"
#include "TG4DetConstruction.h"
#include "TG4PostDetConstruction.h"
#include "TG4SDConstruction.h"
#include "TG4RegionsManager.h"

#ifdef G4MULTITHREADED  
#include <G4MTRunManager.hh>
#else
#include <G4RunManager.hh>
#endif

#include <Randomize.hh>
#include <G4UIsession.hh>
#include <G4UImanager.hh>
#ifdef G4UI_USE
#include <G4UIExecutive.hh>
#endif

#ifdef USE_G4ROOT
#include <TG4RootNavMgr.h>
#endif

#include <TROOT.h> 
#include <TRint.h>
#include <TInterpreter.h>
#include <TGeoManager.h>
#include <TRandom.h>
#include <TVirtualMCApplication.h>

//_____________________________________________________________________________
TG4RunManager* TG4RunManager::fgMasterInstance = 0;
G4ThreadLocal TG4RunManager* TG4RunManager::fgInstance = 0;

//_____________________________________________________________________________
TG4RunManager::TG4RunManager(TG4RunConfiguration* runConfiguration, 
                             int argc, char** argv)                  
  : TG4Verbose("runManager"),
    fRunManager(0),
    fMessenger(this),
    fRunConfiguration(runConfiguration),
    fRegionsManager(0),
    fGeantUISession(0),
    fRootUISession(0),
    fRootUIOwner(false),
    fARGC(argc),
    fARGV(argv),  
    fUseRootRandom(true) 
{
/// Standard constructor

  if (VerboseLevel() > 1) {
    G4cout << "TG4RunManager::TG4RunManager " << this << G4endl;
  }  

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4RunManager", "TG4RunManager",
      "Cannot create two instances of singleton.");
  }
      
  if (!fRunConfiguration) {
    TG4Globals::Exception(
      "TG4RunManager", "TG4RunManager",
      "Cannot create instance without runConfiguration.");
  }
      
  fgInstance = this; 

  G4bool isMaster = ! G4Threading::IsWorkerThread();
  
  if ( isMaster ) {
    fgMasterInstance = this;
    
    // create and configure G4 run manager
    ConfigureRunManager();
    
    // get/create Root UI
    CreateRootUI();
  }  
  else {
    // Get G4 worker run manager 
    fRunManager = G4RunManager::GetRunManager();
    
    // Clone G4Root navigator if needed
    CloneRootNavigatorForWorker();

    fRegionsManager = fgMasterInstance->fRegionsManager;
    fRootUISession = fgMasterInstance->fRootUISession;
    fGeantUISession = fgMasterInstance->fGeantUISession;
  }     

  if (VerboseLevel() > 1) {
    G4cout << "TG4RunManager has been created." << this << G4endl;
  }  
}

//_____________________________________________________________________________
TG4RunManager::~TG4RunManager() 
{
/// Destructor

  G4bool isMaster = ! G4Threading::IsWorkerThread();

  if ( isMaster ) {
    delete fRunConfiguration;
    delete fRegionsManager;
#ifdef G4UI_USE
    delete fGeantUISession;
#endif
    delete fRunManager;
    if (fRootUIOwner) delete fRootUISession;
  }
}

//
// private methods
//

//_____________________________________________________________________________
void TG4RunManager::ConfigureRunManager()
{
/// Set the user action classes defined by TG4RunConfiguration to G4RunManager.

  // Geometry construction and navigator
  //
  if ( VerboseLevel() > 1 )
    G4cout << "TG4RunManager::ConfigureRunManager " << this << G4endl;

  TString userGeometry = fRunConfiguration->GetUserGeometry();
  
  TG4GeometryManager::Instance()
    ->SetUserRegionConstruction(
        fRunConfiguration->CreateUserRegionConstruction());
    
  // Root navigator
#ifdef USE_G4ROOT
  TG4RootNavMgr* rootNavMgr = 0;
  if ( userGeometry == "VMCtoRoot" || userGeometry == "Root" ) {

    // Construct geometry via VMC application
    if ( TG4GeometryManager::Instance()->VerboseLevel() > 0 ) 
      G4cout << "Running TVirtualMCApplication::ConstructGeometry"; 

    TG4StateManager::Instance()->SetNewState(kConstructGeometry);
    TVirtualMCApplication::Instance()->ConstructGeometry();
    TG4StateManager::Instance()->SetNewState(kNotInApplication);
    
    // Set top volume and close Root geometry if not yet done
    if ( ! gGeoManager->IsClosed() ) {
      TGeoVolume *top = (TGeoVolume*)gGeoManager->GetListOfVolumes()->First();
      gGeoManager->SetTopVolume(top);
      gGeoManager->CloseGeometry();  
    }
      
    // Now that we have the ideal geometry, call application misalignment code
    TG4StateManager::Instance()->SetNewState(kMisalignGeometry);
    TVirtualMCApplication::Instance()->MisalignGeometry();
    TG4StateManager::Instance()->SetNewState(kNotInApplication);
    
    // Pass geometry to G4Root navigator
    rootNavMgr = TG4RootNavMgr::GetInstance(gGeoManager);
    G4cout << "TG4RootNavMgr has been created." << rootNavMgr << G4endl;
  }
#endif

  // G4 run manager
#ifdef G4MULTITHREADED
  if (  fRunConfiguration->IsMTApplication() ) {
     fRunManager = new G4MTRunManager();
     fRunManager
       ->SetUserInitialization(new TG4WorkerInitialization());
   }
   else {
     fRunManager =  new G4RunManager();
   }
#else
  fRunManager =  new G4RunManager();
#endif
  if (VerboseLevel() > 1) {
    G4cout << "G4RunManager has been created." << G4endl;
  }

  if ( userGeometry != "VMCtoRoot" && userGeometry != "Root" ) {
    fRunManager
      ->SetUserInitialization(fRunConfiguration->CreateDetectorConstruction());
    if ( VerboseLevel() > 1 )
      G4cout << "CreateDetectorConstruction done." << G4endl;
  }    
  else {
#ifdef USE_G4ROOT
    G4int nthreads = 1;
#ifdef G4MULTITHREADED
    if (  fRunConfiguration->IsMTApplication() ) {
      nthreads = G4MTRunManager::GetMasterRunManager()->GetNumberOfThreads();
    }
#endif
    rootNavMgr->Initialize(new TG4PostDetConstruction(), nthreads);
    rootNavMgr->ConnectToG4();  
#else
   TG4Globals::Exception("TG4RunManager", "ConfigureRunManagerTG4MCGeometry",
     "geomVMCtoRoot and geomRoot options require Geant4 VMC built with G4Root.");
#endif
  }  
    
  // Other mandatory classes
  //  
  fRunManager
    ->SetUserInitialization(fRunConfiguration->CreatePhysicsList());
  if ( VerboseLevel() > 1 )
    G4cout << "CreatePhysicsList done." << G4endl;
 
  fRunManager
    ->SetUserInitialization(new TG4ActionInitialization(fRunConfiguration));      
  if ( VerboseLevel() > 1 )
    G4cout << "Create ActionInitialization done." << G4endl;
  
  // Regions manager
  //
  fRegionsManager = new TG4RegionsManager();
  
  if ( VerboseLevel() > 1 )
    G4cout << "TG4RunManager::ConfigureRunManager done " << this << G4endl;
}

//_____________________________________________________________________________
void TG4RunManager::CloneRootNavigatorForWorker()
{
  // Clone Root navigator for worker thread
  //
#ifdef USE_G4ROOT
  TString userGeometry = fRunConfiguration->GetUserGeometry();
  if ( userGeometry != "VMCtoRoot" && userGeometry != "Root" )  return;

  if ( VerboseLevel() > 1 )
    G4cout << "TG4RunManager::CloneRootNavigatorForWorker " << this << G4endl;

  // Master Root navigator
  TG4RootNavMgr* masterRootNavMgr = TG4RootNavMgr::GetMasterInstance();

  // Create G4Root navigator on worker
  TG4RootNavMgr* rootNavMgr = TG4RootNavMgr::GetInstance(*masterRootNavMgr);
  G4cout << "TG4RootNavMgr has been created." << rootNavMgr << G4endl;

  //rootNavMgr->Initialize(new TG4PostDetConstruction());
  rootNavMgr->ConnectToG4();

  if ( VerboseLevel() > 1 )
    G4cout << "TG4RunManager::CloneRootNavigatorForWorker done " << this << G4endl;
#endif
}

//_____________________________________________________________________________
void TG4RunManager::CreateRootUI()
{
/// Get/Create Root interactive session

  // get Root UI session if it exists
  fRootUISession = gROOT->GetApplication();
  if (fRootUISession) {
    fARGC = fRootUISession->Argc();
    fARGV = fRootUISession->Argv();
  }

  // filter out "-splash" from argument list
  FilterARGV("-splash");

  // create root UI if it does not exist
  if ( ! fRootUISession ) {
    // copy only first command line argument (name of program)
    // (use the same way as in TApplication.cxx)
    int argc = 1;
    char** argv = (char **)new char*[argc];
    argv[0] = StrDup(fARGV[0]);

    fRootUISession = new TRint("rootSession", &argc, argv, 0, 0);
    fRootUIOwner = true;
  }
}

//_____________________________________________________________________________
void TG4RunManager::FilterARGV(const G4String& arg)
{
/// Filter out the option argument from the arguments list fARGV,
/// if present.

  if (fARGC <= 1) return;

  G4bool isArg = false;
  for (G4int i=0; i<fARGC; i++) {
    if (G4String(fARGV[i]) == arg) isArg = true;
    if (isArg && i+1 < fARGC) fARGV[i] = fARGV[i+1];  
  }

  if (isArg) fARGC--;
} 
 
//_____________________________________________________________________________
void TG4RunManager::SetRandomSeed()
{
/// Pass the random number seed from fRandom to Geant4 random number
/// generator

  long seeds[10];
  seeds[0] = gRandom->GetSeed();    
  seeds[1] = gRandom->GetSeed();    
  seeds[2] = 0;    
  CLHEP::HepRandom::setTheSeeds(seeds);
}

// public methods

//_____________________________________________________________________________
void TG4RunManager::Initialize()
{
/// Initialize G4.

  if ( VerboseLevel() > 1 )
    G4cout << "TG4RunManager::Initialize " << this << G4endl;

  // create G4RunManager
  //ConfigureRunManager();

  // initialize Geant4
  fRunManager->Initialize();

  // finish geometry
  TG4GeometryManager::Instance()->FinishGeometry();
  
  // initialize SD manager
  // TG4SDManager::Instance()->Initialize();

  if ( VerboseLevel() > 1 )
    G4cout << "TG4RunManager::Initialize done " << this << G4endl;
}

//_____________________________________________________________________________
void TG4RunManager::LateInitialize()
{
/// Finish initialization of G4 after the G4Run initialization
/// is finished. 

  if ( VerboseLevel() > 1 )
    G4cout << "TG4RunManager::LateInitialize " << this << G4endl;

  G4bool isMaster = ! G4Threading::IsWorkerThread();

  // define particles 
  TG4PhysicsManager::Instance()->DefineParticles();

  // set user limits
  if ( isMaster ) {
    TG4GeometryManager::Instance()
      ->SetUserLimits(*TG4G3PhysicsManager::Instance()->GetCutVector(),
                      *TG4G3PhysicsManager::Instance()->GetControlVector());

  // pass info if cut on e+e- pair is activated to stepping action  
  // TO DO LATER - Stepping Action NOT AVAILABLE                  
  //((TG4SteppingAction*)fRunManager->GetUserSteppingAction())
  //  ->SetIsPairCut((*TG4G3PhysicsManager::Instance()->GetIsCutVector())[kEplus]);                   

    // convert tracking cuts in range cuts per regions
    if ( fRunConfiguration->IsSpecialCuts() ) fRegionsManager->DefineRegions();
  }

  // activate/inactivate physics processes
  TG4PhysicsManager::Instance()->SetProcessActivation();
  TG4PhysicsManager::Instance()->RetrieveOpBoundaryProcess();

  // print statistics
  TG4GeometryServices::Instance()->PrintStatistics(true, false);  
  TG4SDServices::Instance()->PrintStatistics(false, true);  

  if (VerboseLevel() > 2) {
    TG4GeometryServices::Instance()->PrintLogicalVolumeStore();  
  }
  
  // set the random number seed
  if ( fUseRootRandom ) SetRandomSeed();

  if ( VerboseLevel() > 1 )
    G4cout << "TG4RunManager::LateInitialize done " << this << G4endl;
}

//_____________________________________________________________________________
void TG4RunManager::ProcessEvent()
{
/// Not yet implemented.

  TG4Globals::Warning(
    "TG4RunManager", "ProcessEvent", "Not implemented.");
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
void TG4RunManager::CreateGeantUI()
{
/// Create interactive Geant4.

  if ( fGeantUISession ) return;

#ifdef G4UI_USE
  // create session if it does not exist
  // G4cout << fARGC << "  "  << fARGV << G4endl;
  fGeantUISession = new G4UIExecutive(fARGC, fARGV);
#endif
}

//_____________________________________________________________________________
void TG4RunManager::StartGeantUI()
{ 
/// Start interactive/batch Geant4.

  if ( ! fGeantUISession ) CreateGeantUI();
  
  if ( fGeantUISession ) {  
#ifdef G4UI_USE
    // interactive session
    G4cout << "Welcome (back) in Geant4" << G4endl;
    fGeantUISession->GetSession()->SessionStart();
    G4cout << "Welcome (back) in Root" << G4endl;
#endif    
  }
  else {
    G4cout << "Geant4 UI not available." << G4endl;
  }
}

//_____________________________________________________________________________
void TG4RunManager::StartRootUI()
{
/// Start interactive Root.

  if (!fRootUISession) CreateRootUI();
  if (fRootUISession) { 
    G4cout << "Welcome (back) in Root" << G4endl;
    fRootUISession->Run(kTRUE);
    G4cout << "Welcome (back) in Geant4" << G4endl;
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
  G4int result = pUI->ApplyCommand(command);

  // From G4UIbatch::ExecCommand():
  switch ( result )  {
    case fCommandSucceeded:
      break;
    case fCommandNotFound:
      G4cerr << "***** COMMAND NOT FOUND <"
             << command << "> *****" << G4endl;
      break;
    case fIllegalApplicationState:
      G4cerr << "***** Illegal application state <"
             << command << "> *****" << G4endl;
      break;
    default:
      G4int pn = result%100;
      G4cerr << "***** Illegal parameter (" << pn << ") <"
             << command << "> *****" << G4endl;
  }
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
///  Since transition to G4SmartTrackStack in Geant4 9.6.x 
///  secondaries are not ordered even when the special stacking is activated.

  return false;
}  

