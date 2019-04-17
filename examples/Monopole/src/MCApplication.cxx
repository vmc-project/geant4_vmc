//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file MCApplication.cxx
/// \brief Implementation of the MCApplication class
///
/// Geant4 Monopole example adapted to Virtual Monte Carlo \n
///
/// \date 15/07/2018
/// \author I. Hrivnacova; IPN, Orsay

#include "MCApplication.h"
#include "DetectorConstruction.h"
#include "Ex03MCStack.h"

#include <TMCRootManager.h>

#include <Riostream.h>
#include <TDatabasePDG.h>
#include <TGeoManager.h>
#include <TGeoMaterial.h>
#include <TH1.h>
#include <TInterpreter.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TVirtualMC.h>

using namespace std;

/// \cond CLASSIMP
ClassImp(VMC::Monopole::MCApplication)
  /// \endcond

  namespace
{
  int G4lrint(double ad)
  {
    return (ad > 0) ? static_cast<int>(ad + .5) : static_cast<int>(ad - .5);
  }
}

namespace VMC
{
namespace Monopole
{

std::vector<TH1D*> fHistograms;

//_____________________________________________________________________________
MCApplication::MCApplication(const char* name, const char* title)
  : TVirtualMCApplication(name, title),
    fRootManager(0),
    fStack(0),
    fDetConstruction(0),
    fMagField(0),
    fBinSize(0.),
    fOffsetX(0.),
    fProjRange(0.),
    fProjRange2(0.),
    fImedAl(0),
    fNofEvents(0),
    fIsMaster(kTRUE)
{
  /// Standard constructor
  /// \param name   The MC application name
  /// \param title  The MC application description

  // Create a user stack
  fStack = new Ex03MCStack(1000);

  // Create detector construction
  fDetConstruction = new DetectorConstruction();

  // Constant magnetic field (in kiloGauss)
  fMagField = new TGeoUniformMagField(0, 0, 2);
}

//_____________________________________________________________________________
MCApplication::MCApplication(const MCApplication& origin)
  : TVirtualMCApplication(origin.GetName(), origin.GetTitle()),
    fRootManager(0),
    fStack(0),
    fDetConstruction(origin.fDetConstruction),
    fMagField(0),
    fImedAl(origin.fImedAl),
    fNofEvents(0),
    fIsMaster(kFALSE)
{
  /// Copy constructor for cloning application on workers (in multithreading
  /// mode) \param origin   The source MC application

  // Create new user stack
  fStack = new Ex03MCStack(1000);

  // Constant magnetic field (in kiloGauss)
  fMagField = new TGeoUniformMagField(origin.fMagField->GetFieldValue()[0],
    origin.fMagField->GetFieldValue()[1], origin.fMagField->GetFieldValue()[2]);
}

//_____________________________________________________________________________
MCApplication::MCApplication()
  : TVirtualMCApplication(),
    fRootManager(0),
    fStack(0),
    fDetConstruction(0),
    fMagField(0),
    fBinSize(0.5),
    fImedAl(0),
    fNofEvents(0),
    fIsMaster(kTRUE)
{
  /// Default constructor
}

//_____________________________________________________________________________
MCApplication::~MCApplication()
{
  /// Destructor

  delete fStack;
  if (fIsMaster) delete fDetConstruction;
  delete fMagField;
  delete gMC;
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
  gMC->SetMagField(fMagField);
  gMC->Init();
  gMC->BuildPhysics();

  // Notify detector construction about initialization
  fDetConstruction->SetGeometryInitialized(true);

  // initialize projected range, tallies, Ebeam, and book histograms
  // Double_t maxStepSize = 0.5;
  fProjRange = 0.;
  fProjRange2 = 0.;

  Double_t length = fDetConstruction->GetAbsorberSizeX() * 10.; // in mm
  Double_t binSize = fBinSize * 10.;                            // in mm
  Int_t numBins = G4lrint(length / binSize);

  // Define offset for filling histogram
  fOffsetX = -0.5 * length; // in mm

  // cout << "numBins, length, offsetX "
  //      << numBins << ", " << length << ", " << fOffsetX << endl;

  // Create histograms
  // fHistograms.push_back(new TH1D("Edep_x", "Edep (MeV/mm) along absorber
  // (mm)", numBins, 0, length));
  fHistograms.push_back(
    new TH1D("h1", "Edep (MeV/mm) along absorber (mm)", numBins, 0, length));
  // fHistograms.push_back(new TH1D("h2", "DEDX (MeV/mm) of proton", 100,
  // -3., 7.)); fHistograms.push_back(new TH1D("h3", "DEDX (MeV/mm) of
  // monopole", 100, -3., 7.)); fHistograms.push_back(new TH1D("h4", "Range(mm)
  // of proton", 100, -3., 7.)); fHistograms.push_back(new TH1D("h5", "Range(mm)
  // of monopole", 100, -3., 7.)); Control histograms fHistograms.push_back(new
  // TH1D("x", "x", 100, -10, 10)); fHistograms.push_back(new TH1D("Edep (MeV)",
  // "Edep (Mev)", 100, 0, 100));

  RegisterStack();
}

//__________________________________________________________________________
void MCApplication::RunMC(Int_t nofEvents)
{
  /// Run MC.
  /// \param nofEvents Number of events to be processed

  gMC->ProcessRun(nofEvents);
  fNofEvents = nofEvents;
  FinishRun();
}

//_____________________________________________________________________________
void MCApplication::FinishRun()
{
  /// Finish MC run.

  // Get monopole mass from TDatabasePDG
  TDatabasePDG* pdgDB = TDatabasePDG::Instance();
  TParticlePDG* monopole = pdgDB->GetParticle(60000000);
  Double_t mass = 0;
  if (monopole) {
    mass = monopole->Mass();
  }

  // run conditions
  cout << "\n The run consists of " << fNofEvents << " monopole "
       << " of " << mass << " GeV "
       << " through " << fDetConstruction->GetAbsorberSizeX() << " cm of "
       << fDetConstruction->GetAbsorberMaterial()
       // << " (density: "  << G4BestUnit(density,"Volumic Mass") << ")"
       << endl;

  // compute projected range and straggling

  fProjRange /= fNofEvents;
  fProjRange2 /= fNofEvents;
  Double_t rms = fProjRange2 - fProjRange * fProjRange;
  if (rms > 0.) {
    rms = std::sqrt(rms);
  }
  else {
    rms = 0.;
  }

  cout.precision(5);
  cout << "\n projected Range= " << fProjRange << " cm "
       << "   rms= " << rms << " cm " << endl;

  /*
    G4double ekin[100], dedxproton[100], dedxmp[100];
    G4EmCalculator calc;
    calc.SetVerbose(0);
    G4int i;
    for(i = 0; i < 100; ++i) {
      ekin[i] = std::pow(10., 0.1*G4double(i)) * keV;
      dedxproton[i] =
        calc.ComputeElectronicDEDX(ekin[i], "proton", matName);
      dedxmp[i] =
        calc.ComputeElectronicDEDX(ekin[i], "monopole", matName);
    }

    if(GetVerbose() > 0){
      G4cout << "### Stopping Powers" << G4endl;
      for(i=0; i<100; i++) {
        G4cout << " E(MeV)= " << ekin[i] << "  dedxp= " << dedxproton[i]
               << " dedxmp= " << dedxmp[i]
               << G4endl;
      }
    }
    G4cout << "### End of stopping power table" << G4endl;

    if(GetVerbose() > 0){
      G4cout << "Range table for " << matName << G4endl;
    }

    for(i=0; i<100; ++i) {
      G4double e = std::log10(ekin[i] / MeV) + 0.05;
      fHisto->Fill(1, e, dedxproton[i]);
      fHisto->Fill(2, e, dedxmp[i]);
      fHisto->Fill(3, e,
    std::log10(calc.GetRange(ekin[i],"proton",matName)/mm)); fHisto->Fill(4, e,
    std::log10(calc.GetRange(ekin[i],"monopole",matName)/mm));
    }
  */
  // normalize histogram
  // G4double fac = (mm/MeV) / (nEvents * fBinLength);
  Double_t fac = 1. / (fNofEvents * fBinSize * 10.);
  fHistograms[0]->Scale(fac);

  if (fRootManager) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

/*

//_____________________________________________________________________________
TVirtualMCApplication* MCApplication::CloneForWorker() const
{
  return new MCApplication(*this);
}

//_____________________________________________________________________________
void MCApplication::InitForWorker() const
{
  // Create Root manager
  fRootManager
    = new TMCRootManager(GetName(), TMCRootManager::kWrite);
  //fRootManager->SetDebug(true);

  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);

  RegisterStack();
}

//_____________________________________________________________________________
void MCApplication::FinishWorkerRun() const
{
  //cout << "Ex03MCApplication::FinishWorkerRun: " << endl;
  if ( fRootManager ) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}
*/

//_____________________________________________________________________________
void MCApplication::ConstructGeometry()
{
  /// Construct geometry using detector contruction class.
  /// The detector contruction class is using TGeo functions.

  fDetConstruction->ConstructGeometry();
}

//_____________________________________________________________________________
void MCApplication::InitGeometry()
{
  /// Initialize geometry.

  fImedAl = gMC->MediumId("Aluminium");
}

//_____________________________________________________________________________
void MCApplication::GeneratePrimaries()
{
  /// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  // Track ID (filled by stack)
  Int_t ntr;

  // Option: to be tracked
  Int_t toBeDone = 1;

  // Monopole
  Int_t pdg = 60000000;

  // Polarization
  Double_t polx = 0.;
  Double_t poly = 0.;
  Double_t polz = 0.;

  // Position
  Double_t vx = -0.5 * (fDetConstruction->GetWorldSizeX()) + 1e-10; // + 1*um
  Double_t vy = 0.;
  Double_t vz = 0.;
  Double_t tof = 0.08;

  // Energy (in GeV)
  Double_t kinEnergy = 100.;
  Double_t mass = 100.;
  Double_t e = mass + kinEnergy;

  // Particle momentum
  Double_t px, py, pz;
  px = sqrt(e * e - mass * mass);
  py = 0.;
  pz = 0.;

  // Add particle to stack
  fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx,
    poly, polz, kPPrimary, ntr, 1., 0);
}

//_____________________________________________________________________________
void MCApplication::BeginEvent()
{
  /// User actions at beginning of event.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void MCApplication::BeginPrimary()
{
  /// User actions at beginning of a primary track.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void MCApplication::PreTrack()
{
  /// User actions at beginning of each track.
  /// Print info message.

  // Check the monopole properties set via configuration macro
  // cout << endl;
  // std::cout << "&&&&& Track mass: " << gMC->TrackMass() << std::endl;
  // std::cout << "&&&&& Track charge: " << gMC->TrackCharge() << std::endl;
}

//_____________________________________________________________________________
void MCApplication::Stepping()
{
  /// User actions at each step.
  /// Print track position, the current volume and current medium names.

  static TLorentzVector prevPosition;

  TLorentzVector position;
  gMC->TrackPosition(position);

  Double_t edep = gMC->Edep();
  if (edep <= 0.) {
    prevPosition = position;
    return;
  }

  edep *= 1.e03; // convert in MeV

  // Bragg curve
  Double_t x = position.X() * 10.;
  Double_t dx = (position.X() - prevPosition.X()) * 10.;
  // cout << "prevPosition "
  //      << prevPosition.X() << ", "  << prevPosition.Y() << ", "  <<
  //      prevPosition.Z() << endl;
  // cout << "position "
  //      << position.X() << ", "  << position.Y() << ", "  << position.Z() <<
  //      endl;
  // cout << "dx= " << dx << endl;

  Double_t random = gRandom->Uniform(0, 1);
  x += dx * random - fOffsetX;

  fHistograms[0]->Fill(x, edep);
  // fHistograms[1]->Fill(x);
  // fHistograms[2]->Fill(edep);

  // keep position from previous step
  prevPosition = position;
}

//_____________________________________________________________________________
void MCApplication::PostTrack()
{
  /// User actions after finishing of each track

  // Skip secondary tracks
  if (fStack->GetCurrentParentTrackNumber() != -1) return;

  TLorentzVector position;
  gMC->TrackPosition(position);

  Double_t x = position.X() - fOffsetX / 10.; // in cm
  fProjRange += x;
  fProjRange2 += x * x;
}

//_____________________________________________________________________________
void MCApplication::FinishPrimary()
{
  /// User actions after finishing of a primary track.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void MCApplication::FinishEvent()
{
  /// User actions after finishing of an event

  // fRootManager->Fill();
  fStack->Reset();
}

//_____________________________________________________________________________
void MCApplication::SetBinSize(Double_t binSize)
{
  /// Set Edep histogram bin size (in cm)

  if (fDetConstruction->GetGeometryInitialized()) {
    cerr << "Geometry alredy initialized: cannot set Edep histogram bin size"
         << endl;
    return;
  }

  fBinSize = binSize;
}

} // namespace Monopole
} // namespace VMC
