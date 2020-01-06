//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03cMCApplication.cxx
/// \brief Implementation of the Ex03cMCApplication class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \date 21/08/2019
/// \author Benedikt Volkel, CERN

#include "Ex03cMCApplication.h"
#include "Ex03PrimaryGenerator.h"
#include "Ex03cMCStack.h"

#include <TMCRootManager.h>

#include <TMCManager.h>

#include <Riostream.h>
#include <TGeoManager.h>
#include <TGeoUniformMagField.h>
#include <TInterpreter.h>
#include <TPDGCode.h>
#include <TParticle.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TTimer.h>
#include <TVector3.h>
#include <TVirtualGeoTrack.h>
#include <TVirtualMC.h>

#include <TLorentzVector.h>

using namespace std;

/// \cond CLASSIMP
ClassImp(Ex03cMCApplication)
  /// \endcond

  //_____________________________________________________________________________
  Ex03cMCApplication::Ex03cMCApplication(
    const char* name, const char* title, Bool_t isMulti, Bool_t splitSimulation)
  : TVirtualMCApplication(name, title),
    fRootManager(0),
    fPrintModulo(1),
    fEventNo(0),
    fVerbose(0),
    fStack(0),
    fDetConstruction(0),
    fCalorimeterSD(0),
    fPrimaryGenerator(0),
    fMagField(0),
    fOldGeometry(kFALSE),
    fIsControls(kFALSE),
    fIsMaster(kTRUE),
    fIsMultiRun(isMulti),
    fSplitSimulation(splitSimulation),
    fG3Id(-1),
    fG4Id(-1),
    fDebug(0)
{
  /// Standard constructor
  /// \param name   The MC application name
  /// \param title  The MC application description

  if (splitSimulation && !isMulti) {
    Fatal("Ex03cMCApplication",
      "Cannot split simulation between engines without \"isMulti\" being "
      "switched on");
  }

  cout << "--------------------------------------------------------------"
       << endl;
  cout << " VMC Example E03c: new version with multiple engines" << endl;
  cout << "--------------------------------------------------------------"
       << endl;

  // Create a user stack
  fStack = new Ex03cMCStack(1000);

  // Create detector construction
  fDetConstruction = new Ex03cDetectorConstruction();

  // Create a calorimeter SD
  fCalorimeterSD = new Ex03cCalorimeterSD("Calorimeter", fDetConstruction);

  // Create a primary generator
  fPrimaryGenerator = new Ex03PrimaryGenerator(fStack);

  // Constant magnetic field (in kiloGauss)
  fMagField = new TGeoUniformMagField();

  if (isMulti) {
    RequestMCManager();
    fMCManager->SetUserStack(fStack);
  }
}

//_____________________________________________________________________________
Ex03cMCApplication::Ex03cMCApplication(const Ex03cMCApplication& origin)
  : TVirtualMCApplication(origin.GetName(), origin.GetTitle()),
    fRootManager(0),
    fPrintModulo(origin.fPrintModulo),
    fEventNo(0),
    fVerbose(origin.fVerbose),
    fStack(0),
    fDetConstruction(origin.fDetConstruction),
    fCalorimeterSD(0),
    fPrimaryGenerator(0),
    fMagField(0),
    fOldGeometry(origin.fOldGeometry),
    fIsControls(origin.fIsControls),
    fIsMaster(kFALSE),
    fIsMultiRun(origin.fIsMultiRun),
    fSplitSimulation(origin.fSplitSimulation),
    fG3Id(origin.fG3Id),
    fG4Id(origin.fG4Id),
    fDebug(origin.fDebug)
{
  /// Copy constructor for cloning application on workers (in multithreading
  /// mode) \param origin   The source MC application

  // Create new user stack
  fStack = new Ex03cMCStack(1000);

  // Create a calorimeter SD
  fCalorimeterSD =
    new Ex03cCalorimeterSD(*(origin.fCalorimeterSD), fDetConstruction);

  // Create a primary generator
  fPrimaryGenerator =
    new Ex03PrimaryGenerator(*(origin.fPrimaryGenerator), fStack);

  // Constant magnetic field (in kiloGauss)
  fMagField = new TGeoUniformMagField(origin.fMagField->GetFieldValue()[0],
    origin.fMagField->GetFieldValue()[1], origin.fMagField->GetFieldValue()[2]);
}

//_____________________________________________________________________________
Ex03cMCApplication::Ex03cMCApplication()
  : TVirtualMCApplication(),
    fRootManager(0),
    fPrintModulo(1),
    fEventNo(0),
    fStack(0),
    fDetConstruction(0),
    fCalorimeterSD(0),
    fPrimaryGenerator(0),
    fMagField(0),
    fOldGeometry(kFALSE),
    fIsControls(kFALSE),
    fIsMaster(kTRUE),
    fIsMultiRun(kFALSE),
    fSplitSimulation(kFALSE),
    fG3Id(-1),
    fG4Id(-1),
    fDebug(0)
{
  /// Default constructor
}

//_____________________________________________________________________________
Ex03cMCApplication::~Ex03cMCApplication()
{
  /// Destructor

  // cout << "Ex03cMCApplication::~Ex03cMCApplication " << this << endl;

  delete fRootManager;
  delete fStack;
  if (fIsMaster) delete fDetConstruction;
  delete fCalorimeterSD;
  delete fPrimaryGenerator;
  delete fMagField;
  if (!fIsMultiRun) {
    delete fMC;
  }

  // cout << "Done Ex03cMCApplication::~Ex03cMCApplication " << this << endl;
}

//
// private methods
//

//_____________________________________________________________________________
void Ex03cMCApplication::RegisterStack() const
{
  /// Register stack in the Root manager.

  if (fRootManager) {
    // cout << "Ex03cMCApplication::RegisterStack: " << endl;
    fRootManager->Register("stack", "Ex03cMCStack", &fStack);
  }
}

//
// public methods
//

//_____________________________________________________________________________
void Ex03cMCApplication::InitMC(const char* setup)
{
  /// Initialize MC.
  /// The selection (and creation) of the concrete MC is done in the macro.
  /// If no macro is provided, the MC must be created before calling this function.
  /// \param setup The name of the configuration macro.

  fVerbose.InitMC();

  if (TString(setup) != "") {
    gROOT->LoadMacro(setup);
    gInterpreter->ProcessLine("Config()");
    if (!fMC) {
      Fatal(
        "InitMC", "Processing Config() has failed. (No MC is instantiated.)");
    }
  }

// MT support available from root v 5.34/18
#if ROOT_VERSION_CODE >= 336402
  // Create Root manager
  if (!fMC->IsMT()) {
    fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
    // fRootManager->SetDebug(true);
  }
#else
  // Create Root manager
  fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
// fRootManager->SetDebug(true);
#endif

  fMC->SetStack(fStack);
  fMC->SetRootGeometry();
  fMC->SetMagField(fMagField);
  fMC->Init();
  fMC->BuildPhysics();

  RegisterStack();

  if (fDebug>0) {
    Info("InitMC", "Single run initialised");
  }
}

//_____________________________________________________________________________
void Ex03cMCApplication::InitMC(std::initializer_list<const char*> setupMacros)
{
  /// Initialize MC.
  /// New function for initialization with multiple engines.
  /// \param setupMacros The names of the configuration macros.

  fVerbose.InitMC();

  if (!fIsMultiRun) {
    Fatal("InitMC",
      "Initialisation of multiple engines not supported in single run");
  }

  if (setupMacros.size() > 0) {
    for (const char* setup : setupMacros) {
      gROOT->LoadMacro(setup);
      gInterpreter->ProcessLine("Config()");
      if (!fMC) {
        Fatal(
          "InitMC", "Processing Config() has failed. (No MC is instantiated.)");
      }
      // gInterpreter->UnloadAllSharedLibraryMaps();
      gInterpreter->UnloadFile(setup);
    }
  }

  InitMC();
}

//_____________________________________________________________________________
void Ex03cMCApplication::InitMC()
{
  /// Initialize MC.
  /// New function for initialization with multiple engines.
  /// The MCs must be created before calling this function.

  if (!fIsMultiRun) {
    Fatal("InitMC",
      "Initialisation of multiple engines not supported in single run");
  }

// MT support available from root v 5.34/18
#if ROOT_VERSION_CODE >= 336402
  // Create Root manager
  if (!fMC->IsMT()) {
    fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
    // fRootManager->SetDebug(true);
  }
#else
  // Create Root manager
  fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
// fRootManager->SetDebug(true);
#endif

  fMCManager->Init([this](TVirtualMC* mc) {
    mc->SetRootGeometry();
    mc->SetMagField(this->fMagField);
    mc->Init();
    mc->BuildPhysics();
  });
  RegisterStack();

  fG3Id = fMCManager->GetEngineId("TGeant3TGeo");
  fG4Id = fMCManager->GetEngineId("TGeant4");
  if (fDebug>0) {
    Info("InitMC", "Multi run initialised");
    std::cout << "Engine IDs\n"
              << "TGeant3TGeo: " << fG3Id << "\n"
              << "TGeant4: " << fG4Id << std::endl;
  }
}

//_____________________________________________________________________________
void Ex03cMCApplication::RunMC(Int_t nofEvents)
{
  /// Run MC.
  /// \param nofEvents Number of events to be processed

  fVerbose.RunMC(nofEvents);

  // Prepare a timer
  TTimer timer;

  if (!fIsMultiRun) {
    if (fDebug>0) {
      Info("RunMC", "Start single run");
      std::cout << "Simulation entirely done with engine " << fMC->GetName()
                << std::endl;
      timer.Start();
    }
    fMC->ProcessRun(nofEvents);
  }
  else {
    if (fDebug>0) {
      Info("RunMC", "Start multi run");
      if (fSplitSimulation) {
        std::cout << "GAPX simulated with engine "
                  << fMCManager->GetEngine(0)->GetName() << "\n"
                  << "ABSO simulated with engine "
                  << fMCManager->GetEngine(1)->GetName() << std::endl;
      }
      else {
        std::cout << "Simulation entirely done with engine "
                  << fMCManager->GetCurrentEngine()->GetName() << std::endl;
      }
      timer.Start();
    }
    fMCManager->Run(nofEvents);
  }
  if (fDebug>0) {
    timer.Stop();
    Info("RunMC", "Transport finished.");
    timer.Print();
  }
  FinishRun();
}

//_____________________________________________________________________________
void Ex03cMCApplication::FinishRun()
{
  /// Finish MC run.

  fVerbose.FinishRun();
  // cout << "Ex03cMCApplication::FinishRun: " << endl;
  if (fRootManager) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

//_____________________________________________________________________________
TVirtualMCApplication* Ex03cMCApplication::CloneForWorker() const
{
  return new Ex03cMCApplication(*this);
}

//_____________________________________________________________________________
void Ex03cMCApplication::InitForWorker() const
{
  // cout << "Ex03cMCApplication::InitForWorker " << this << endl;

  // Create Root manager
  fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
  // fRootManager->SetDebug(true);

  // Set data to MC
  fMC->SetStack(fStack);
  fMC->SetMagField(fMagField);

  RegisterStack();
}

//_____________________________________________________________________________
void Ex03cMCApplication::FinishWorkerRun() const
{
  // cout << "Ex03cMCApplication::FinishWorkerRun: " << endl;
  if (fRootManager) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

//_____________________________________________________________________________
void Ex03cMCApplication::ReadEvent(Int_t i)
{
  /// Read \em i -th event and prints hits.
  /// \param i The number of event to be read

  fCalorimeterSD->Register();
  RegisterStack();
  fRootManager->ReadEvent(i);
}

//_____________________________________________________________________________
void Ex03cMCApplication::ConstructGeometry()
{
  /// Construct geometry using detector contruction class.
  /// The detector contruction class is using TGeo functions or
  /// TVirtualMC functions (if oldGeometry is selected)

  fVerbose.ConstructGeometry();

  if (fOldGeometry) {
    Fatal("ConstructGeometry", "Cannot run with old geomtry");
  }
  fDetConstruction->ConstructMaterials();
  fDetConstruction->ConstructGeometry();
  // TGeoManager::Import("geometry.root");
  // fMC->SetRootGeometry();
}

//_____________________________________________________________________________
void Ex03cMCApplication::InitGeometry()
{
  /// Initialize geometry

  fVerbose.InitGeometry();

  fDetConstruction->SetCuts();

  if (fIsControls) fDetConstruction->SetControls();

  fCalorimeterSD->Initialize();
}

//_____________________________________________________________________________
void Ex03cMCApplication::AddParticles()
{
  /// Example of user defined particle with user defined decay mode

  fVerbose.AddParticles();

  // Define the 2 body  phase space decay  for He5
  Int_t mode[6][3];
  Float_t bratio[6];

  for (Int_t kz = 0; kz < 6; kz++) {
    bratio[kz] = 0.;
    mode[kz][0] = 0;
    mode[kz][1] = 0;
    mode[kz][2] = 0;
  }
  bratio[0] = 100.;
  mode[0][0] = kNeutron;   // neutron (2112)
  mode[0][1] = 1000020040; // alpha

  // Overwrite a decay mode already defined in MCs
  // Kaon Short: 310 normally decays in two modes
  // pi+, pi-  68.61 %
  // pi0, pi0  31.39 %
  // and we force only the mode pi0, pi0

  Int_t mode2[6][3];
  Float_t bratio2[6];

  for (Int_t kz = 0; kz < 6; kz++) {
    bratio2[kz] = 0.;
    mode2[kz][0] = 0;
    mode2[kz][1] = 0;
    mode2[kz][2] = 0;
  }
  bratio2[0] = 100.;
  mode2[0][0] = kPi0; // pi0 (111)
  mode2[0][1] = kPi0; // pi0 (111)

  // Define particle
  fMC->DefineParticle(1000020050, "He5", kPTHadron, 5.03427, 2.0, 0.002, "Ion",
    0.0, 0, 1, 0, 0, 0, 0, 0, 5, kFALSE);
  fMC->SetDecayMode(1000020050, bratio, mode);
  fMC->SetDecayMode(kK0Short, bratio2, mode2);
}

//_____________________________________________________________________________
void Ex03cMCApplication::AddIons()
{
  /// Example of user defined ion

  fVerbose.AddIons();
  fMC->DefineIon("MyIon", 34, 70, 12, 0.);
}

//_____________________________________________________________________________
void Ex03cMCApplication::GeneratePrimaries()
{
  /// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  fVerbose.GeneratePrimaries();

  TVector3 origin(fDetConstruction->GetWorldSizeX(),
    fDetConstruction->GetCalorSizeYZ(), fDetConstruction->GetCalorSizeYZ());

  fPrimaryGenerator->GeneratePrimaries(origin);
}

//_____________________________________________________________________________
void Ex03cMCApplication::BeginEvent()
{
  /// User actions at beginning of event

  fVerbose.BeginEvent();

  // Clear TGeo tracks (if filled)
  if (!fIsMultiRun && TString(fMC->GetName()) == "TGeant3TGeo" &&
      gGeoManager->GetListOfTracks() && gGeoManager->GetTrack(0) &&
      ((TVirtualGeoTrack*)gGeoManager->GetTrack(0))->HasPoints()) {

    gGeoManager->ClearTracks();
    // if (gPad) gPad->Clear();
  }

  fEventNo++;
  if (fEventNo % fPrintModulo == 0) {
    cout << "\n---> Begin of event: " << fEventNo << endl;
    // ??? How to do this in VMC
    // HepRandom::showEngineStatus();
  }
}

//_____________________________________________________________________________
void Ex03cMCApplication::BeginPrimary()
{
  /// User actions at beginning of a primary track.
  /// If test for user defined decay is activated,
  /// the primary track ID is printed on the screen.

  fVerbose.BeginPrimary();

  if (fPrimaryGenerator->GetUserDecay()) {
    cout << "   Primary track ID = " << fStack->GetCurrentTrackNumber() << endl;
  }
}

//_____________________________________________________________________________
void Ex03cMCApplication::PreTrack()
{
  /// User actions at beginning of each track
  /// If test for user defined decay is activated,
  /// the decay products of the primary track (K0Short)
  /// are printed on the screen.

  fVerbose.PreTrack();

  // print info about K0Short decay products
  if (fPrimaryGenerator->GetUserDecay()) {
    Int_t parentID = fStack->GetCurrentParentTrackNumber();

    if (parentID >= 0 &&
        fStack->GetParticle(parentID)->GetPdgCode() == kK0Short &&
        fStack->GetCurrentTrack()->GetUniqueID() == kPDecay) {
      // The production process is saved as TParticle unique ID
      // via Ex03cMCStack

      cout << "      Current track " << fStack->GetCurrentTrack()->GetName()
           << "  is a decay product of Parent ID = "
           << fStack->GetCurrentParentTrackNumber() << endl;
    }
  }
}

//_____________________________________________________________________________
void Ex03cMCApplication::Stepping()
{
  /// User actions at each step

  fVerbose.Stepping();

  fCalorimeterSD->ProcessHits();

  TLorentzVector pos;
  TLorentzVector mom;

  fMC->TrackPosition(pos);
  fMC->TrackMomentum(mom);

  if (fDebug>1) {
    std::cout << "Current engine " << fMC->GetName() << "\n"
              << "Track ID=" << fStack->GetCurrentTrackNumber() << "\n"
              << "Momentum\n"
              << "E=" << mom.T() << ", Px=" << mom.X() << ", Py=" << mom.Y()
              << ", Pz=" << mom.Z() << std::endl;
  }

  // Now transfer track
  if (fSplitSimulation) {
    Int_t targetId = -1;
    if (fMC->GetId() == 0 && strcmp(fMC->CurrentVolName(), "ABSO") == 0) {
      targetId = 1;
    }
    else if (fMC->GetId() == 1 && strcmp(fMC->CurrentVolName(), "GAPX") == 0) {
      targetId = 0;
    }
    if (targetId > -1) {
      if (fDebug>1) {
        Info("Stepping", "Transfer track");
      }
      fMCManager->TransferTrack(targetId);
    }
  }
}

//_____________________________________________________________________________
void Ex03cMCApplication::PostTrack()
{
  /// User actions after finishing of each track
  fVerbose.PostTrack();
}

//_____________________________________________________________________________
void Ex03cMCApplication::FinishPrimary()
{
  /// User actions after finishing of a primary track

  fVerbose.FinishPrimary();

  if (fPrimaryGenerator->GetUserDecay()) {
    cout << endl;
  }
}

//_____________________________________________________________________________
void Ex03cMCApplication::FinishEvent()
{
  /// User actions after finishing of an event

  fVerbose.FinishEvent();

  // Geant3 + TGeo
  // (use TGeo functions for visualization)
  if (!fIsMultiRun && TString(fMC->GetName()) == "TGeant3TGeo") {

    // Draw volume
    gGeoManager->SetVisOption(0);
    gGeoManager->SetTopVisible();
    gGeoManager->GetTopVolume()->Draw();

    // Draw tracks (if filled)
    // Available when this feature is activated via
    // fMC->SetCollectTracks(kTRUE);
    if (gGeoManager->GetListOfTracks() && gGeoManager->GetTrack(0) &&
        ((TVirtualGeoTrack*)gGeoManager->GetTrack(0))->HasPoints()) {

      gGeoManager->DrawTracks("/*"); // this means all tracks
    }
  }

  fRootManager->Fill();

  if (fEventNo % fPrintModulo == 0) fCalorimeterSD->PrintTotal();

  fCalorimeterSD->EndOfEvent();

  fStack->Reset();
}
