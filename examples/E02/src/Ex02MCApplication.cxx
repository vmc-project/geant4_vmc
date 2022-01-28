//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex02MCApplication.cxx
/// \brief Implementation of the Ex02MCApplication class
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex02MCApplication.h"
#include "Ex02DetectorConstructionOld.h"
#include "Ex02MCStack.h"
#include "Ex02MagField.h"

#include <Riostream.h>
#include <TGeoManager.h>
#include <TInterpreter.h>
#include <TMCRootManager.h>
#include <TPDGCode.h>
#include <TROOT.h>
#include <TVirtualGeoTrack.h>
#include <TVirtualMC.h>

using namespace std;

/// \cond CLASSIMP
ClassImp(Ex02MCApplication)
  /// \endcond

  //_____________________________________________________________________________
  Ex02MCApplication::Ex02MCApplication(const char* name, const char* title)
  : TVirtualMCApplication(name, title),
    fRootManager(0),
    fStack(0),
    fDetConstruction(),
    fTrackerSD(0),
    fMagField(0),
    fOldGeometry(kFALSE)
{
  /// Standard constructor
  /// \param name   The MC application name
  /// \param title  The MC application description

  // cout << "Ex02MCApplication::Ex02MCApplication " << this << endl;

  // Create application data

  // Create SD
  fTrackerSD = new Ex02TrackerSD("Tracker Chamber");
  // Create a user stack
  fStack = new Ex02MCStack(100);
  // Constant magnetic field (in kiloGauss)
  fMagField = new Ex02MagField(20., 0., 0.);
  // It si also possible to use TGeoUniformMagField class:
  // fMagField = new TGeoUniformMagField(20., 0., 0.);
}

//_____________________________________________________________________________
Ex02MCApplication::Ex02MCApplication(const Ex02MCApplication& origin)
  : TVirtualMCApplication(origin.GetName(), origin.GetTitle()),
    fRootManager(0),
    fStack(0),
    fDetConstruction(origin.fDetConstruction),
    fTrackerSD(0),
    fMagField(0),
    fOldGeometry(kFALSE)
{
  /// Copy constructor (for clonig on worker thread in MT mode).
  /// \param origin  The source object (on master).

  // cout << "Ex02MCApplication::Ex02MCApplication " << this << endl;

  // Create application data

  // Create SD
  fTrackerSD = new Ex02TrackerSD(*(origin.fTrackerSD));
  // Create a user stack
  fStack = new Ex02MCStack(100);
  // Constant magnetic field (in kiloGauss)
  fMagField = new Ex02MagField(20., 0., 0.);
  // It si also possible to use TGeoUniformMagField class:
  // fMagField = new TGeoUniformMagField(20., 0., 0.);
}

//_____________________________________________________________________________
Ex02MCApplication::Ex02MCApplication()
  : TVirtualMCApplication(),
    fRootManager(0),
    fStack(0),
    fDetConstruction(),
    fTrackerSD(),
    fMagField(0),
    fOldGeometry(kFALSE)
{
  /// Default constructor
}

//_____________________________________________________________________________
Ex02MCApplication::~Ex02MCApplication()
{
  /// Destructor

  // cout << "Ex02MCApplication::~Ex02MCApplication " << this << endl;

  delete fRootManager;
  delete fStack;
  delete fTrackerSD;
  delete fMagField;
  delete gMC;

  // cout << "Done Ex02MCApplication::~Ex02MCApplication " << this << endl;
}

//
// private methods
//
//_____________________________________________________________________________
void Ex02MCApplication::RegisterStack() const
{
  /// Register stack in the Root manager.

  if (fRootManager) {
    // cout << "Ex02MCApplication::RegisterStack: " << endl;
    fRootManager->Register("stack", "Ex02MCStack", &fStack);
  }
}

//
// public methods
//

//_____________________________________________________________________________
void Ex02MCApplication::InitMC(const char* setup)
{
  /// Initialize MC from Config.C macro
  /// The selection of the concrete MC is done in the macro.
  /// \param setup The name of the configuration macro

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

  // Set data to MC
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);

  // Init MC
  gMC->Init();
  gMC->BuildPhysics();

  RegisterStack();
}

//_____________________________________________________________________________
void Ex02MCApplication::RunMC(Int_t nofEvents)
{
  /// Run MC.
  /// \param nofEvents Number of events to be processed

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void Ex02MCApplication::FinishRun()
{
  /// Finish MC run.

  // cout << "Ex02MCApplication::FinishRun: " << endl;
  if (fRootManager) {
    // fRootManager->WriteAndClose();
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

//_____________________________________________________________________________
TVirtualMCApplication* Ex02MCApplication::CloneForWorker() const
{
  return new Ex02MCApplication(*this);
}

//_____________________________________________________________________________
void Ex02MCApplication::InitOnWorker()
{
  // cout << "Ex02MCApplication::InitForWorker " << this << endl;

  // Create Root manager
  fRootManager = new TMCRootManager(GetName(), TMCRootManager::kWrite);
  // fRootManager->SetDebug(true);

  // Set data to MC
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);

  RegisterStack();
}

//_____________________________________________________________________________
void Ex02MCApplication::FinishRunOnWorker()
{
  // cout << "Ex02MCApplication::FinishWorkerRun: " << endl;
  if (fRootManager) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

//_____________________________________________________________________________
void Ex02MCApplication::ConstructGeometry()
{
  /// Construct geometry using detector contruction class.
  /// The detector contruction class is using TGeo functions or
  /// TVirtualMC functions (if oldGeometry is selected)

  // Cannot use Root geometry if not supported with
  // selected MC
  if (!fOldGeometry && !gMC->IsRootGeometrySupported()) {
    cerr << "Selected MC does not support TGeo geometry" << endl;
    cerr << "Exiting program" << endl;
    exit(1);
  }

  if (!fOldGeometry) {
    cout << "Geometry will be defined via TGeo" << endl;
    fDetConstruction.ConstructMaterials();
    fDetConstruction.ConstructGeometry();
  }
  else {
    cout << "Geometry will be defined via VMC" << endl;
    Ex02DetectorConstructionOld detConstructionOld;
    detConstructionOld.ConstructMaterials();
    detConstructionOld.ConstructGeometry();
  }
}

//_____________________________________________________________________________
void Ex02MCApplication::InitGeometry()
{
  /// Initialize geometry

  fTrackerSD->Initialize();
}

//_____________________________________________________________________________
void Ex02MCApplication::GeneratePrimaries()
{
  /// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  // Track ID (filled by stack)
  Int_t ntr;

  // Option: to be tracked
  Int_t toBeDone = 1;

  // Particle type
  // Int_t pdg  = 0;    // geantino
  Int_t pdg = kProton;

  // Polarization
  Double_t polx = 0.;
  Double_t poly = 0.;
  Double_t polz = 0.;

  // Position
  Double_t vx = 0.;
  Double_t vy = 0.;
  Double_t vz = -0.5 * (fDetConstruction.GetWorldFullLength());
  Double_t tof = 0.;

  // Energy
  Double_t kinEnergy = 3.0;
  Double_t mass = 0.9382723;
  Double_t e = mass + kinEnergy;

  // Momentum
  Double_t px, py, pz;
  px = 0.;
  py = 0.;
  pz = sqrt(e * e - mass * mass);

  // Add particle to stack
  fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx,
    poly, polz, kPPrimary, ntr, 1., 0);
}

//_____________________________________________________________________________
void Ex02MCApplication::BeginEvent()
{
  /// User actions at beginning of event.
  /// Nothing to be done this example

  fStack->SetObjectNumber();
}

//_____________________________________________________________________________
void Ex02MCApplication::BeginPrimary()
{
  /// User actions at beginning of a primary track.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex02MCApplication::PreTrack()
{
  /// User actions at beginning of each track.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex02MCApplication::Stepping()
{
  /// User actions at each step

  fTrackerSD->ProcessHits();
}

//_____________________________________________________________________________
void Ex02MCApplication::PostTrack()
{
  /// User actions at each step.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex02MCApplication::FinishPrimary()
{
  /// User actions after finishing of a primary track.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex02MCApplication::FinishEvent()
{
  /// User actions after finishing of an event
  /// Nothing to be done this example

  // Geant4 own visualization is activated via G4 macro (g4config.in)

  // TGeo visualization
  if (gGeoManager && gGeoManager->GetListOfTracks() &&
      gGeoManager->GetTrack(0) &&
      ((TVirtualGeoTrack*)gGeoManager->GetTrack(0))->HasPoints()) {

    gGeoManager->SetVisOption(0);
    gGeoManager->SetTopVisible();
    gGeoManager->DrawTracks("/*"); // this means all tracks
  }

  fRootManager->Fill();

  fTrackerSD->EndOfEvent();

  fStack->Print();
  fStack->Reset();
}

//_____________________________________________________________________________
void Ex02MCApplication::ReadEvent(Int_t i)
{
  /// Read \em i -th event and prints hits.
  /// \param i The number of event to be read

  if (!fRootManager) {
    fRootManager = new TMCRootManager(GetName(), TMCRootManager::kRead);
  }

  fTrackerSD->Register();
  RegisterStack();
  fRootManager->ReadEvent(i);

  fStack->Print();
  fTrackerSD->Print();
}
