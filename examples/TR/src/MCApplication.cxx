//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/src/MCApplication.cxx
/// \brief Implementation of the MCApplication class
///
/// Geant4 TestEm10 adapted to Virtual Monte Carlo.
///
/// \date 18/12/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "MCApplication.h"
#include "DetectorConstruction.h"
#include "Ex03MCStack.h"
#include "PrimaryGenerator.h"
#include "SensitiveDetector.h"

#include <TMCRootManager.h>

#include <Riostream.h>
#include <TGeoManager.h>
#include <TGeoUniformMagField.h>
#include <TH1.h>
#include <TInterpreter.h>
#include <TPDGCode.h>
#include <TParticle.h>
#include <TROOT.h>
#include <TVirtualGeoTrack.h>
#include <TVirtualMC.h>

using namespace std;

/// \cond CLASSIMP
ClassImp(VMC::TR::MCApplication)
  /// \endcond

  namespace VMC
{
  namespace TR
  {

  std::vector<TH1D*> fHistograms;

  //_____________________________________________________________________________
  MCApplication::MCApplication(const char* name, const char* title)
    : TVirtualMCApplication(name, title),
      fRootManager(0),
      fPrintModulo(1),
      fEventNo(0),
      fVerbose(0),
      fStack(0),
      fDetConstruction(0),
      fSensitiveDetector(0),
      fPrimaryGenerator(0),
      fMagField(0),
      fOldGeometry(kFALSE),
      fIsControls(kFALSE),
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
    fSensitiveDetector = new SensitiveDetector("Absorber");

    // Create a primary generator
    fPrimaryGenerator = new PrimaryGenerator(fStack);

    // Constant magnetic field (in kiloGauss)
    fMagField = new TGeoUniformMagField();
  }

  //_____________________________________________________________________________
  MCApplication::MCApplication(const MCApplication& origin)
    : TVirtualMCApplication(origin.GetName(), origin.GetTitle()),
      fRootManager(0),
      fPrintModulo(origin.fPrintModulo),
      fEventNo(0),
      fVerbose(origin.fVerbose),
      fStack(0),
      fDetConstruction(origin.fDetConstruction),
      fSensitiveDetector(0),
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
    fSensitiveDetector = new SensitiveDetector(*(origin.fSensitiveDetector));

    // Create a primary generator
    fPrimaryGenerator =
      new PrimaryGenerator(*(origin.fPrimaryGenerator), fStack);

    // Constant magnetic field (in kiloGauss)
    fMagField = new TGeoUniformMagField(origin.fMagField->GetFieldValue()[0],
      origin.fMagField->GetFieldValue()[1],
      origin.fMagField->GetFieldValue()[2]);
  }

  //_____________________________________________________________________________
  MCApplication::MCApplication()
    : TVirtualMCApplication(),
      fRootManager(0),
      fPrintModulo(1),
      fEventNo(0),
      fStack(0),
      fDetConstruction(0),
      fSensitiveDetector(0),
      fPrimaryGenerator(0),
      fMagField(0),
      fOldGeometry(kFALSE),
      fIsControls(kFALSE),
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
    delete fMagField;
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

  //_____________________________________________________________________________
  void MCApplication::BookHisto() const
  {
    /// Create histograms

    fHistograms.push_back(new TH1D("1", "Edep", 100, 0., 0.1));
    fHistograms.push_back(new TH1D("2", "XTR Gamma spectrum", 100, 0., 0.1));
    fHistograms.push_back(
      new TH1D("3", "Secondary Gamma spectrum", 100, 0., 0.1));
    fHistograms.push_back(new TH1D("4", "Secondary e- spectrum", 100, 0., 0.1));
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
    // Cretae histograms
    BookHisto();

    gMC->SetStack(fStack);
    gMC->SetMagField(fMagField);
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

    // print run statisctics
    cout << " ================== run summary =====================" << endl;
    cout << " End of Run TotNbofEvents = " << fEventNo << endl;

    cout << " Mean energy deposit in absorber = " << fHistograms[0]->GetMean()
         << " +-" << fHistograms[0]->GetRMS() << "  MeV " << endl;

    cout << " Total number of XTR gammas = " << fHistograms[1]->GetEntries()
         << endl;

    cout << " Total number of all gammas = " << fHistograms[2]->GetEntries()
         << endl;

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
  void MCApplication::InitOnWorker()
  {
    // cout << "MCApplication::InitForWorker " << this << endl;

    // Create Root manager
    fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
    // fRootManager->SetDebug(true);

    // Set data to MC
    gMC->SetStack(fStack);
    gMC->SetMagField(fMagField);

    RegisterStack();
  }

  //_____________________________________________________________________________
  void MCApplication::FinishRunOnWorker()
  {
    // cout << "MCApplication::FinishWorkerRun: " << endl;
    if (fRootManager) {
      fRootManager->WriteAll();
      fRootManager->Close();
    }
  }

  //_____________________________________________________________________________
  void MCApplication::ConstructGeometry()
  {
    /// Construct geometry using detector contruction class.
    /// The detector contruction class is using TGeo functions or
    /// TVirtualMC functions (if oldGeometry is selected)

    fVerbose.ConstructGeometry();

    fDetConstruction->ConstructGeometry();
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

    fPrimaryGenerator->GeneratePrimaries();
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
    if (fEventNo % fPrintModulo == 0) {
      cout << "\n---> Begin of event: " << fEventNo << endl;
    }
  }

  //_____________________________________________________________________________
  void MCApplication::BeginPrimary()
  {
    /// User actions at beginning of a primary track.

    fVerbose.BeginPrimary();
  }

  //_____________________________________________________________________________
  void MCApplication::PreTrack()
  {
    /// User actions at beginning of each track.
    /// Fill spectra.

    fVerbose.PreTrack();

    // Gamma
    if (gMC->TrackPid() == kGamma) {
      // XTR gammas
      Int_t creatorProcess = fStack->GetCurrentTrack()->GetUniqueID();

// Available since 6.07/03 and 5.34/35
// kpTransitionRadiation = 49
#if ((ROOT_VERSION_CODE >= ROOT_VERSION(6, 7, 3)) ||  \
     ((ROOT_VERSION_CODE <= ROOT_VERSION(6, 0, 0)) && \
       (ROOT_VERSION_CODE >= ROOT_VERSION(5, 34, 35))))
      if (creatorProcess == kPTransitionRadiation) {
#else
      if (creatorProcess == kPNull) {
#endif
        fHistograms[1]->Fill(gMC->Etot() * 1e+03);
      }
      fHistograms[2]->Fill(gMC->Etot() * 1e+03);
    }

    // Secondary e-
    if (gMC->TrackPid() == kElectron &&
        fStack->GetCurrentParentTrackNumber() != -1) {

      fHistograms[3]->Fill((gMC->Etot() - gMC->TrackMass()) * 1e+03);
    }
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

    // Geant3 + TGeo
    // (use TGeo functions for visualization)
    // if ( TString(gMC->GetName()) == "TGeant3TGeo") {

    //    // Draw volume
    //    gGeoManager->SetVisOption(0);
    //    gGeoManager->SetTopVisible();
    //    gGeoManager->GetTopVolume()->Draw();

    //    // Draw tracks (if filled)
    //    // Available when this feature is activated via
    //    // gMC->SetCollectTracks(kTRUE);
    //    if ( gGeoManager->GetListOfTracks() &&
    //         gGeoManager->GetTrack(0) &&
    //       ((TVirtualGeoTrack*)gGeoManager->GetTrack(0))->HasPoints() ) {

    //      gGeoManager->DrawTracks("/*");  // this means all tracks
    //   }
    // }

    // Fill Edep histogram in MeV
    // cout << "Filling in h0 " <<
    // fSensitiveDetector->GetHit(0)->GetEdep()*1e+03 << endl;
    fHistograms[0]->Fill(fSensitiveDetector->GetEdep() * 1e+03);

    fRootManager->Fill();

    if (fEventNo % fPrintModulo == 0) {
      fSensitiveDetector->Print();
    }

    fSensitiveDetector->EndOfEvent();
    fStack->Reset();
  }

  } // namespace TR
}
