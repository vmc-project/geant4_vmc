//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file ExGarfield/src/MCApplication.cxx
/// \brief Implementation of the ExGarfield::MCApplication class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "MCApplication.h"
#include "Ex03MCStack.h"
#include "Hit.h"
#include "PrimaryGenerator.h"

#include <TMCRootManager.h>

#include <Riostream.h>
#include <TGeoManager.h>
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
ClassImp(VMC::ExGarfield::MCApplication)
  /// \endcond

  namespace VMC
{
  namespace ExGarfield
  {

  //_____________________________________________________________________________
  MCApplication::MCApplication(const char* name, const char* title)
    : TVirtualMCApplication(name, title),
      fRootManager(0),
      fEventNo(0),
      fVerbose(0),
      fStack(0),
      fDetConstruction(0),
      fSensitiveDetector(0),
      fPrimaryGenerator(0),
      fIsMaster(kTRUE)
  {
    /// Standard constructor
    /// \param name   The MC application name
    /// \param title  The MC application description

    // Create a user stack
    fStack = new Ex03MCStack(1000);

    // Create detector construction
    fDetConstruction = new DetectorConstruction();

    // Create a calorimeter SD
    fSensitiveDetector = new SensitiveDetector("Calorimeter");

    // Create a primary generator
    fPrimaryGenerator = new PrimaryGenerator(fStack);
  }

  //_____________________________________________________________________________
  MCApplication::MCApplication(const MCApplication& origin)
    : TVirtualMCApplication(origin.GetName(), origin.GetTitle()),
      fRootManager(0),
      fEventNo(0),
      fVerbose(origin.fVerbose),
      fStack(0),
      fDetConstruction(origin.fDetConstruction),
      fSensitiveDetector(0),
      fPrimaryGenerator(0),
      fIsMaster(kFALSE)
  {
    /// Copy constructor for cloning application on workers (in multithreading
    /// mode) \param origin   The source MC application

    // Create new user stack
    fStack = new Ex03MCStack(1000);

    // Create a calorimeter SD
    fSensitiveDetector = new SensitiveDetector(*(origin.fSensitiveDetector));

    // Create a primary generator
    fPrimaryGenerator =
      new PrimaryGenerator(*(origin.fPrimaryGenerator), fStack);
  }

  //_____________________________________________________________________________
  MCApplication::MCApplication()
    : TVirtualMCApplication(),
      fRootManager(0),
      fEventNo(0),
      fStack(0),
      fDetConstruction(0),
      fSensitiveDetector(0),
      fPrimaryGenerator(0),
      fIsMaster(kTRUE)
  {
    /// Default constructor
  }

  //_____________________________________________________________________________
  MCApplication::~MCApplication()
  {
    /// Destructor

    // cout << "MCApplication::~MCApplication " << this << endl;

    delete fRootManager;
    delete fStack;
    if (fIsMaster) delete fDetConstruction;
    delete fSensitiveDetector;
    delete fPrimaryGenerator;
    delete gMC;

    // cout << "Done MCApplication::~MCApplication " << this << endl;
  }

  //
  // private methods
  //

  //_____________________________________________________________________________
  void MCApplication::RegisterStack() const
  {
    /// Register stack in the Root manager.

    if (fRootManager) {
      // cout << "MCApplication::RegisterStack: " << endl;
      fRootManager->Register("stack", "Ex03MCStack", &fStack);
    }
  }

  //
  // public methods
  //

  //_____________________________________________________________________________
  void MCApplication::InitMC(const char* setup)
  {
    /// Initialize MC.
    /// The selection of the concrete MC is done in the macro.
    /// \param setup The name of the configuration macro

    fVerbose.InitMC();

    if (TString(setup) != "") {
      gROOT->LoadMacro(setup);
      gInterpreter->ProcessLine("Config()");
      if (!gMC) {
        Fatal(
          "InitMC", "Processing Config() has failed. (No MC is instantiated.)");
      }
    }

// MT support available from root v 5.34/18
#if ROOT_VERSION_CODE >= 336402
    // Create Root manager
    if (!gMC->IsMT()) {
      fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
      // fRootManager->SetDebug(true);
    }
#else
    // Create Root manager
    fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
    // fRootManager->SetDebug(true);
#endif

    gMC->SetStack(fStack);
    gMC->Init();
    gMC->BuildPhysics();

    RegisterStack();
  }

  //_____________________________________________________________________________
  void MCApplication::RunMC(Int_t nofEvents)
  {
    /// Run MC.
    /// \param nofEvents Number of events to be processed

    fVerbose.RunMC(nofEvents);

    gMC->ProcessRun(nofEvents);
    FinishRun();
  }

  //_____________________________________________________________________________
  void MCApplication::FinishRun()
  {
    /// Finish MC run.

    fVerbose.FinishRun();
    // cout << "MCApplication::FinishRun: " << endl;
    if (fRootManager) {
      fRootManager->WriteAll();
      fRootManager->Close();
    }
  }

  //_____________________________________________________________________________
  TVirtualMCApplication* MCApplication::CloneForWorker() const
  {
    return new MCApplication(*this);
  }

  //_____________________________________________________________________________
  void MCApplication::InitForWorker() const
  {
    // cout << "MCApplication::InitForWorker " << this << endl;

    // Create Root manager
    fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
    // fRootManager->SetDebug(true);

    // Set data to MC
    gMC->SetStack(fStack);

    RegisterStack();
  }

  //_____________________________________________________________________________
  void MCApplication::FinishWorkerRun() const
  {
    // cout << "MCApplication::FinishWorkerRun: " << endl;
    if (fRootManager) {
      fRootManager->WriteAll();
      fRootManager->Close();
    }
  }

  //_____________________________________________________________________________
  void MCApplication::ReadEvent(Int_t i)
  {
    /// Read \em i -th event and prints hits.
    /// \param i The number of event to be read

    fSensitiveDetector->Register();
    RegisterStack();
    fRootManager->ReadEvent(i);
  }

  //_____________________________________________________________________________
  void MCApplication::ConstructGeometry()
  {
    /// Construct geometry using detector contruction class.
    /// The detector contruction class is using TGeo functions or
    /// TVirtualMC functions (if oldGeometry is selected)

    fVerbose.ConstructGeometry();

    fDetConstruction->Construct();
  }

  //_____________________________________________________________________________
  void MCApplication::InitGeometry()
  {
    /// Initialize geometry

    fVerbose.InitGeometry();

    fSensitiveDetector->Initialize();
  }

  //_____________________________________________________________________________
  void MCApplication::GeneratePrimaries()
  {
    /// Fill the user stack (derived from TVirtualMCStack) with primary
    /// particles.

    fVerbose.GeneratePrimaries();

    TVector3 origin;
    fPrimaryGenerator->GeneratePrimaries(origin);
  }

  //_____________________________________________________________________________
  void MCApplication::BeginEvent()
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
    cout << " Start generating event Nr " << fEventNo << endl;
  }

  //_____________________________________________________________________________
  void MCApplication::BeginPrimary()
  {
    /// User actions at beginning of a primary track.
    /// If test for user defined decay is activated,
    /// the primary track ID is printed on the screen.

    fVerbose.BeginPrimary();
  }

  //_____________________________________________________________________________
  void MCApplication::PreTrack()
  {
    /// User actions at beginning of each track
    /// If test for user defined decay is activated,
    /// the decay products of the primary track (K0Short)
    /// are printed on the screen.

    fVerbose.PreTrack();
  }

  //_____________________________________________________________________________
  void MCApplication::Stepping()
  {
    /// User actions at each step

    // Work around for Fluka VMC, which does not call
    // MCApplication::PreTrack()
    //
    // cout << "MCApplication::Stepping" << this << endl;
    static Int_t trackId = 0;
    if (TString(gMC->GetName()) == "TFluka" &&
        gMC->GetStack()->GetCurrentTrackNumber() != trackId) {
      fVerbose.PreTrack();
      trackId = gMC->GetStack()->GetCurrentTrackNumber();
    }

    fVerbose.Stepping();

    fSensitiveDetector->ProcessHits();
  }

  //_____________________________________________________________________________
  void MCApplication::PostTrack()
  {
    /// User actions after finishing of each track

    fVerbose.PostTrack();
  }

  //_____________________________________________________________________________
  void MCApplication::FinishPrimary()
  {
    /// User actions after finishing of a primary track

    fVerbose.FinishPrimary();
  }

  //_____________________________________________________________________________
  void MCApplication::FinishEvent()
  {
    /// User actions after finishing of an event

    fVerbose.FinishEvent();

    // update hit info from Garfield
    fSensitiveDetector->UpdateFromGarfield();
    fRootManager->Fill();

    // reset data
    fSensitiveDetector->EndOfEvent();
    fStack->Reset();
  }

  } // namespace ExGarfield
}
