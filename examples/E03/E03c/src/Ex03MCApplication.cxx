//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03MCApplication.cxx
/// \brief Implementation of the Ex03MCApplication class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \date 21/08/2019
/// \author Benedikt Volkel, CERN

#include "Ex03MCApplication.h"
#include "Ex03MCStack.h"
#include "Ex03PrimaryGenerator.h"

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
ClassImp(Ex03MCApplication)
  /// \endcond

  //_____________________________________________________________________________
  Ex03MCApplication::Ex03MCApplication(
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
    fG4Id(-1)
{
  /// Standard constructor
  /// \param name   The MC application name
  /// \param title  The MC application description

  if (splitSimulation && !isMulti) {
    Fatal("Ex03MCApplication",
      "Cannot split simulation between engines without \"isMulti\" being "
      "switched on");
  }

  // Create a user stack
  fStack = new Ex03MCStack(1000);

  // Create detector construction
  fDetConstruction = new Ex03DetectorConstruction();

  // Create a calorimeter SD
  fCalorimeterSD = new Ex03CalorimeterSD("Calorimeter", fDetConstruction);

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
Ex03MCApplication::Ex03MCApplication(const Ex03MCApplication& origin)
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
    fIsMaster(kFALSE)
{
  /// Copy constructor for cloning application on workers (in multithreading
  /// mode) \param origin   The source MC application

  // Create new user stack
  fStack = new Ex03MCStack(1000);

  // Create a calorimeter SD
  fCalorimeterSD =
    new Ex03CalorimeterSD(*(origin.fCalorimeterSD), fDetConstruction);

  // Create a primary generator
  fPrimaryGenerator =
    new Ex03PrimaryGenerator(*(origin.fPrimaryGenerator), fStack);

  // Constant magnetic field (in kiloGauss)
  fMagField = new TGeoUniformMagField(origin.fMagField->GetFieldValue()[0],
    origin.fMagField->GetFieldValue()[1], origin.fMagField->GetFieldValue()[2]);
}

//_____________________________________________________________________________
Ex03MCApplication::Ex03MCApplication()
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
    fG4Id(-1)
{
  /// Default constructor
}

//_____________________________________________________________________________
Ex03MCApplication::~Ex03MCApplication()
{
  /// Destructor

  // cout << "Ex03MCApplication::~Ex03MCApplication " << this << endl;

  delete fRootManager;
  delete fStack;
  if (fIsMaster) delete fDetConstruction;
  delete fCalorimeterSD;
  delete fPrimaryGenerator;
  delete fMagField;
  if (!fIsMultiRun) {
    delete fMC;
  }

  // cout << "Done Ex03MCApplication::~Ex03MCApplication " << this << endl;
}

//
// private methods
//

//_____________________________________________________________________________
void Ex03MCApplication::RegisterStack() const
{
  /// Register stack in the Root manager.

  if (fRootManager) {
    // cout << "Ex03MCApplication::RegisterStack: " << endl;
    fRootManager->Register("stack", "Ex03MCStack", &fStack);
  }
}

//
// public methods
//

//_____________________________________________________________________________
void Ex03MCApplication::InitMC(const char* setup)
{
  /// Initialize MC.
  /// The selection of the concrete MC is done in the macro.
  /// \param setup The name of the configuration macro

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

  Info("InitMC", "Single run initialised");
}

//_____________________________________________________________________________
void Ex03MCApplication::InitMC(std::initializer_list<const char*> setupMacros)
{
  /// Initialize MC.
  /// The selection of the concrete MC is done in the macro.
  /// \param setup The name of the configuration macro

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

  Info("InitMC", "Multi run initialised");
  fG3Id = fMCManager->GetEngineId("TGeant3TGeo");
  fG4Id = fMCManager->GetEngineId("TGeant4");
  std::cout << "Engine IDs\n"
            << "TGeant3TGeo: " << fG3Id << "\n"
            << "TGeant4: " << fG4Id << std::endl;
}

//_____________________________________________________________________________
void Ex03MCApplication::RunMC(Int_t nofEvents)
{
  /// Run MC.
  /// \param nofEvents Number of events to be processed

  fVerbose.RunMC(nofEvents);

  // Prepare a timer
  TTimer timer;

  if (!fIsMultiRun) {
    Info("RunMC", "Start single run");
    std::cout << "Simulation entirely done with engine " << fMC->GetName()
              << std::endl;
    timer.Start();
    fMC->ProcessRun(nofEvents);
  }
  else {
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
    fMCManager->Run(nofEvents);
  }
  timer.Stop();
  Info("RunMC", "Transport finished.");
  timer.Print();
  FinishRun();
}

//_____________________________________________________________________________
void Ex03MCApplication::FinishRun()
{
  /// Finish MC run.

  fVerbose.FinishRun();
  // cout << "Ex03MCApplication::FinishRun: " << endl;
  if (fRootManager) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

//_____________________________________________________________________________
TVirtualMCApplication* Ex03MCApplication::CloneForWorker() const
{
  return new Ex03MCApplication(*this);
}

//_____________________________________________________________________________
void Ex03MCApplication::InitForWorker() const
{
  // cout << "Ex03MCApplication::InitForWorker " << this << endl;

  // Create Root manager
  fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
  // fRootManager->SetDebug(true);

  // Set data to MC
  fMC->SetStack(fStack);
  fMC->SetMagField(fMagField);

  RegisterStack();
}

//_____________________________________________________________________________
void Ex03MCApplication::FinishWorkerRun() const
{
  // cout << "Ex03MCApplication::FinishWorkerRun: " << endl;
  if (fRootManager) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

//_____________________________________________________________________________
void Ex03MCApplication::ReadEvent(Int_t i)
{
  /// Read \em i -th event and prints hits.
  /// \param i The number of event to be read

  fCalorimeterSD->Register();
  RegisterStack();
  fRootManager->ReadEvent(i);
}

//_____________________________________________________________________________
void Ex03MCApplication::ConstructGeometry()
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
void Ex03MCApplication::InitGeometry()
{
  /// Initialize geometry

  fVerbose.InitGeometry();

  fDetConstruction->SetCuts();

  if (fIsControls) fDetConstruction->SetControls();

  fCalorimeterSD->Initialize();
}

//_____________________________________________________________________________
void Ex03MCApplication::AddParticles()
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
void Ex03MCApplication::AddIons()
{
  /// Example of user defined ion

  fVerbose.AddIons();
  fMC->DefineIon("MyIon", 34, 70, 12, 0.);
}

//_____________________________________________________________________________
void Ex03MCApplication::GeneratePrimaries()
{
  /// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  fVerbose.GeneratePrimaries();

  TVector3 origin(fDetConstruction->GetWorldSizeX(),
    fDetConstruction->GetCalorSizeYZ(), fDetConstruction->GetCalorSizeYZ());

  fPrimaryGenerator->GeneratePrimaries(origin);
}

//_____________________________________________________________________________
void Ex03MCApplication::BeginEvent()
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
void Ex03MCApplication::BeginPrimary()
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
void Ex03MCApplication::PreTrack()
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
      // via Ex03MCStack

      cout << "      Current track " << fStack->GetCurrentTrack()->GetName()
           << "  is a decay product of Parent ID = "
           << fStack->GetCurrentParentTrackNumber() << endl;
    }
  }
}

//_____________________________________________________________________________
void Ex03MCApplication::Stepping()
{
  /// User actions at each step

  fVerbose.Stepping();

  fCalorimeterSD->ProcessHits();

  TLorentzVector pos;
  TLorentzVector mom;

  fMC->TrackPosition(pos);
  fMC->TrackMomentum(mom);

  if (fVerbose.GetLevel() > 2) {

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
      if (fVerbose.GetLevel() > 2) {
        Info("Stepping", "Transfer track");
      }
      fMCManager->TransferTrack(targetId);
    }
  }
}

//_____________________________________________________________________________
void Ex03MCApplication::PostTrack()
{
  /// User actions after finishing of each track
  fVerbose.PostTrack();
}

//_____________________________________________________________________________
void Ex03MCApplication::FinishPrimary()
{
  /// User actions after finishing of a primary track

  fVerbose.FinishPrimary();

  if (fPrimaryGenerator->GetUserDecay()) {
    cout << endl;
  }
}

//_____________________________________________________________________________
void Ex03MCApplication::FinishEvent()
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
