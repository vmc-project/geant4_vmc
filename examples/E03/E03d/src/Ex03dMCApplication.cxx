//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03dMCApplication.cxx
/// \brief Implementation of the Ex03dMCApplication class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \date 07/07/2026
/// \author Radoslaw Karabowicz; GSI

#include "Ex03dMCApplication.h"
#include "Ex03DetectorConstructionOld.h"
#include "Ex03PrimaryGenerator.h"
#include "Ex03dMCStack.h"

#include <TMCRootManager.h>

#include <Riostream.h>
#include <TGeoManager.h>
#include <TGeoUniformMagField.h>
#include <TInterpreter.h>
#include <TPDGCode.h>
#include <TParticle.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TVector3.h>
#include <TVirtualGeoTrack.h>
#include <TVirtualMC.h>

using namespace std;

/// \cond CLASSIMP
ClassImp(Ex03dMCApplication)
  /// \endcond

  //_____________________________________________________________________________
  Ex03dMCApplication::Ex03dMCApplication(const char* name, const char* title)
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
    fIsMaster(kTRUE)
{
  /// Standard constructor
  /// \param name   The MC application name
  /// \param title  The MC application description

  cout << "--------------------------------------------------------------"
       << endl;
  cout << " VMC Example E03" << endl;
  cout << "--------------------------------------------------------------"
       << endl;

  // Create a user stack
  fStack = new Ex03dMCStack(1000);

  // Create detector construction
  fDetConstruction = new Ex03DetectorConstruction();

  // Create a calorimeter SD
  fCalorimeterSD = new Ex03dCalorimeterSD("Calorimeter", fDetConstruction);

  // Create a primary generator
  fPrimaryGenerator = new Ex03PrimaryGenerator(fStack);

  // Constant magnetic field (in kiloGauss)
  fMagField = new TGeoUniformMagField();
}

//_____________________________________________________________________________
Ex03dMCApplication::Ex03dMCApplication(const Ex03dMCApplication& origin)
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
    fIsMaster(kFALSE),
    fStorageMode(origin.fStorageMode),
    fParallelRNTupleWriter(origin.fParallelRNTupleWriter)
{
  /// Copy constructor for cloning application on workers (in multithreading
  /// mode) \param origin   The source MC application

  // Create new user stack
  fStack = new Ex03dMCStack(1000);

  // Create a calorimeter SD
  fCalorimeterSD =
    new Ex03dCalorimeterSD(*(origin.fCalorimeterSD), fDetConstruction);

  // Create a primary generator
  fPrimaryGenerator =
    new Ex03PrimaryGenerator(*(origin.fPrimaryGenerator), fStack);

  // Constant magnetic field (in kiloGauss)
  fMagField = new TGeoUniformMagField(origin.fMagField->GetFieldValue()[0],
    origin.fMagField->GetFieldValue()[1], origin.fMagField->GetFieldValue()[2]);
}

//_____________________________________________________________________________
Ex03dMCApplication::Ex03dMCApplication()
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
    fIsMaster(kTRUE)
{
  /// Default constructor
}

//_____________________________________________________________________________
Ex03dMCApplication::~Ex03dMCApplication()
{
  /// Destructor

  delete fRootManager;
  delete fStack;
  if (fIsMaster) delete fDetConstruction;
  delete fCalorimeterSD;
  delete fPrimaryGenerator;
  delete fMagField;
  delete gMC;
}

//
// private methods
//

//_____________________________________________________________________________
void Ex03dMCApplication::RegisterStack()
{
  /// Register stack in the Root manager.

  if (fRootManager) {
    // cout << "Ex03dMCApplication::RegisterStack: " << endl;
    fStack->Register();
  }
}

//
// public methods
//

//_____________________________________________________________________________
void Ex03dMCApplication::InitMC(
  const char* setup, TMCRootManager::StorageMode storageMode)
{
  fStorageMode = storageMode;
  /// Initialize MC.
  /// The selection of the concrete MC is done in the macro.
  /// \param setup The name of the configuration macro
  cout << "InitMC with "
       << (fStorageMode == TMCRootManager::kTTree ? "TTree" : "RNTuple")
       << " storage" << endl;

  fVerbose.InitMC();

  if (TString(setup) != "") {
    gROOT->LoadMacro(setup);
    gInterpreter->ProcessLine("Config()");
    if (!gMC) {
      Fatal(
        "InitMC", "Processing Config() has failed. (No MC is instantiated.)");
    }
  }

  TString fileModifier = "T";
  if (fStorageMode == TMCRootManager::kRNTuple) fileModifier = "R";

// MT support available from root v 5.34/18
#if ROOT_VERSION_CODE >= 336402
  // Create Root manager
  if (!gMC->IsMT()) {
    fRootManager = new TMCRootManager(
      fileModifier + GetName(), fStorageMode, TMCRootManager::kWrite);
    // fRootManager->SetDebug(true);
  }
  else if (fStorageMode == TMCRootManager::kRNTuple) {
    fRootManager = new TMCRootManager(
      fileModifier + GetName(), fStorageMode, TMCRootManager::kWrite);
  }
#else
  // Create Root manager
  fRootManager =
    new TMCRootManager(fileModifier + GetName(), TMCRootManager::kWrite);
  // fRootManager->SetDebug(true);
#endif

  RegisterStack();

  if (fStorageMode == TMCRootManager::kRNTuple) {
    if (!gMC->IsMT()) {
      fRootManager->CreateRNTuple();
    }
    else {
      fRootManager->CreateRNTuple(true);
      fParallelRNTupleWriter =
        std::move(fRootManager->GetParallelRNTupleWriter());
    }
  }

  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);
  gMC->Init();
  gMC->BuildPhysics();
}

//_____________________________________________________________________________
void Ex03dMCApplication::RunMC(Int_t nofEvents)
{
  /// Run MC.
  /// \param nofEvents Number of events to be processed

  fVerbose.RunMC(nofEvents);

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void Ex03dMCApplication::FinishRun()
{
  /// Finish MC run.

  fVerbose.FinishRun();
  // cout << "Ex03dMCApplication::FinishRun: " << endl;
  if (fRootManager) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

//_____________________________________________________________________________
TVirtualMCApplication* Ex03dMCApplication::CloneForWorker() const
{
  return new Ex03dMCApplication(*this);
}

//_____________________________________________________________________________
void Ex03dMCApplication::InitOnWorker()
{
  // Create Root manager
  Int_t threadRank = 1;
  // The real thread rank will be set in MCRootManager
  if (fStorageMode == TMCRootManager::kRNTuple) {
    fRootManager = new TMCRootManager(fParallelRNTupleWriter);
  }
  else {
    TString fileModifier = "T";
    fRootManager = new TMCRootManager(fileModifier + GetName(), fStorageMode,
      TMCRootManager::kWrite, threadRank);
  }

  // Set data to MC
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);

  RegisterStack();

  if (fStorageMode == TMCRootManager::kRNTuple) {
    fRootManager->CreateRNTuple(true, true);
  }
}

//_____________________________________________________________________________
void Ex03dMCApplication::FinishRunOnWorker()
{
  // cout << "Ex03dMCApplication::FinishWorkerRun: " << endl;
  if (fRootManager) {
    fRootManager->WriteAll();
    if (fStorageMode != TMCRootManager::kRNTuple) fRootManager->Close();
  }
}

//_____________________________________________________________________________
void Ex03dMCApplication::ReadEvent(Int_t i)
{
  /// Read \em i -th event and prints hits.
  /// \param i The number of event to be read
  if (!fRootManager) {
    fRootManager = new TMCRootManager(GetName(), TMCRootManager::kRead);
  }

  fCalorimeterSD->Register();
  RegisterStack();
  fRootManager->ReadEvent(i);
}

//_____________________________________________________________________________
void Ex03dMCApplication::ConstructGeometry()
{
  /// Construct geometry using detector contruction class.
  /// The detector contruction class is using TGeo functions or
  /// TVirtualMC functions (if oldGeometry is selected)

  fVerbose.ConstructGeometry();

  if (!fOldGeometry) {
    fDetConstruction->ConstructMaterials();
    fDetConstruction->ConstructGeometry();
    // TGeoManager::Import("geometry.root");
    // gMC->SetRootGeometry();
  }
  else {
    Ex03DetectorConstructionOld detConstructionOld;
    detConstructionOld.ConstructMaterials();
    detConstructionOld.ConstructGeometry();
  }
}

//_____________________________________________________________________________
void Ex03dMCApplication::InitGeometry()
{
  /// Initialize geometry
  fVerbose.InitGeometry();

  fDetConstruction->SetCuts();

  if (fIsControls) fDetConstruction->SetControls();

  fCalorimeterSD->Initialize();
}

//_____________________________________________________________________________
void Ex03dMCApplication::AddParticles()
{
  /// Example of user defined particle with user defined decay mode

  fVerbose.AddParticles();

  // Define particle
  gMC->DefineParticle(1000020050, "He5", kPTHadron, 5.03427, 2.0, 0.002, "Ion",
    0.0, 0, 1, 0, 0, 0, 0, 0, 5, kFALSE);

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

  gMC->SetDecayMode(1000020050, bratio, mode);

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

  gMC->SetDecayMode(kK0Short, bratio2, mode2);
}

//_____________________________________________________________________________
void Ex03dMCApplication::AddIons()
{
  /// Example of user defined ion

  fVerbose.AddIons();

  gMC->DefineIon("MyIon", 34, 70, 12, 0.);
}

//_____________________________________________________________________________
void Ex03dMCApplication::GeneratePrimaries()
{
  /// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  fVerbose.GeneratePrimaries();

  TVector3 origin(fDetConstruction->GetWorldSizeX(),
    fDetConstruction->GetCalorSizeYZ(), fDetConstruction->GetCalorSizeYZ());

  fPrimaryGenerator->GeneratePrimaries(origin);
}

//_____________________________________________________________________________
void Ex03dMCApplication::BeginEvent()
{
  /// User actions at beginning of event

  fVerbose.BeginEvent();

  // Clear TGeo tracks (if filled)
  if (TString(gMC->GetName()) == "TGeant3TGeo" &&
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
void Ex03dMCApplication::BeginPrimary()
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
void Ex03dMCApplication::PreTrack()
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
      // via Ex03dMCStack

      cout << "      Current track " << fStack->GetCurrentTrack()->GetName()
           << "  is a decay product of Parent ID = "
           << fStack->GetCurrentParentTrackNumber() << endl;
    }
  }
}

//_____________________________________________________________________________
void Ex03dMCApplication::Stepping()
{
  /// User actions at each step

  // Work around for Fluka VMC, which does not call
  // MCApplication::PreTrack()
  //

  static Int_t trackId = 0;
  if (TString(gMC->GetName()) == "TFluka" &&
      gMC->GetStack()->GetCurrentTrackNumber() != trackId) {
    fVerbose.PreTrack();
    trackId = gMC->GetStack()->GetCurrentTrackNumber();
  }

  fVerbose.Stepping();

  fCalorimeterSD->ProcessHits();
}

//_____________________________________________________________________________
void Ex03dMCApplication::PostTrack()
{
  /// User actions after finishing of each track

  fVerbose.PostTrack();
}

//_____________________________________________________________________________
void Ex03dMCApplication::FinishPrimary()
{
  /// User actions after finishing of a primary track

  fVerbose.FinishPrimary();

  if (fPrimaryGenerator->GetUserDecay()) {
    cout << endl;
  }
}

//_____________________________________________________________________________
void Ex03dMCApplication::FinishEvent()
{
  /// User actions after finishing of an event

  fVerbose.FinishEvent();

  // Geant3 + TGeo
  // (use TGeo functions for visualization)
  if (TString(gMC->GetName()) == "TGeant3TGeo") {

    // Draw volume
    gGeoManager->SetVisOption(0);
    gGeoManager->SetTopVisible();
    gGeoManager->GetTopVolume()->Draw();

    // Draw tracks (if filled)
    // Available when this feature is activated via
    // gMC->SetCollectTracks(kTRUE);
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
