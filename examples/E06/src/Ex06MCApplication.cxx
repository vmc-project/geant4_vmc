//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex06MCApplication.cxx
/// \brief Implementation of the Ex06MCApplication class 
///
/// Geant4 ExampleN06 adapted to Virtual Monte Carlo
///
/// \date 16/05/2005
/// \author I. Hrivnacova; IPN, Orsay

#include <TROOT.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <Riostream.h>
#include <TGeoManager.h>
#include <TGeoUniformMagField.h>
#include <TVirtualGeoTrack.h>
#include <TLorentzVector.h>

#include "Ex06MCApplication.h"
#include "Ex03MCStack.h"
#include "Ex06DetectorConstruction.h"
#include "Ex06DetectorConstructionOld.h"
#include "Ex06PrimaryGenerator.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(Ex06MCApplication)
/// \endcond

//_____________________________________________________________________________
Ex06MCApplication::Ex06MCApplication(const char *name, const char *title) 
  : TVirtualMCApplication(name,title),
    fGammaCounter(0),
    fFeedbackCounter(0),
    fRunGammaCounter(0),
    fRunFeedbackCounter(0),
    fVerbose(0),
    fStack(0),
    fMagField(0),
    fDetConstruction(0),
    fPrimaryGenerator(0),
    fOldGeometry(kFALSE),
    fTestStackPopper(kFALSE),
    fIsMaster(kTRUE)
{
/// Standard constructor
/// \param name   The MC application name 
/// \param title  The MC application description

  // Create a user stack
  fStack = new Ex03MCStack(1000);
  
  // create magnetic field (with zero value)
  fMagField = new TGeoUniformMagField(); 

  // Create detector construction
  fDetConstruction = new Ex06DetectorConstruction();
  
  // Create a primary generator
  fPrimaryGenerator = new Ex06PrimaryGenerator(fStack);
}

//_____________________________________________________________________________
Ex06MCApplication::Ex06MCApplication(const Ex06MCApplication& origin)
  : TVirtualMCApplication(origin.GetName(),origin.GetTitle()),
    fGammaCounter(0),
    fFeedbackCounter(0),
    fRunGammaCounter(0),
    fRunFeedbackCounter(0),
    fVerbose(origin.fVerbose),
    fStack(0),
    fMagField(0),
    fDetConstruction(origin.fDetConstruction),
    fPrimaryGenerator(0),
    fOldGeometry(origin.fOldGeometry),
    fTestStackPopper(origin.fTestStackPopper),
    fIsMaster(kFALSE)
{
/// Copy constructor (for clonig on worker thread in MT mode).
/// \param origin  The source object (on master).

  // Create a user stack
  fStack = new Ex03MCStack(1000);

  // create magnetic field (with zero value)
  // TODO: check copying
  fMagField = new TGeoUniformMagField();

  // Create a primary generator
  fPrimaryGenerator
    = new Ex06PrimaryGenerator(*(origin.fPrimaryGenerator), fStack);
}

//_____________________________________________________________________________
Ex06MCApplication::Ex06MCApplication()
  : TVirtualMCApplication(),
    fGammaCounter(0),
    fFeedbackCounter(0),
    fRunGammaCounter(0),
    fRunFeedbackCounter(0),
    fVerbose(0),
    fStack(0),
    fMagField(0),
    fDetConstruction(0),
    fPrimaryGenerator(0),
    fOldGeometry(kFALSE),
    fTestStackPopper(kFALSE),
    fIsMaster(kTRUE)
{    
/// Default constructor
}

//_____________________________________________________________________________
Ex06MCApplication::~Ex06MCApplication() 
{
/// Destructor  
  

  //cout << "Ex06MCApplication::~Ex06MCApplication " << this << endl;

  delete fStack;
  delete fMagField;
  if ( fIsMaster) delete fDetConstruction;
  delete fPrimaryGenerator;
  delete gMC;

  //cout << "Done Ex06MCApplication::~Ex06MCApplication " << this << endl;
}

//
// public methods
//

//_____________________________________________________________________________
void Ex06MCApplication::InitMC(const char* setup)
{    
/// Initialize MC.
/// The selection of the concrete MC is done in the macro.
/// \param setup The name of the configuration macro 

  fVerbose.InitMC();

  if ( TString(setup) != "" ) {
    gROOT->LoadMacro(setup);
    gInterpreter->ProcessLine("Config()");
    if ( ! gMC ) {
      Fatal("InitMC",
            "Processing Config() has failed. (No MC is instantiated.)");
    }
  }  
 
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);
  gMC->Init();
  gMC->BuildPhysics(); 
}

//_____________________________________________________________________________
void Ex06MCApplication::RunMC(Int_t nofEvents)
{    
/// Run MC.
/// \param nofEvents Number of events to be processed

  fVerbose.RunMC(nofEvents);

  gMC->ProcessRun(nofEvents);

  FinishRun();
}

//_____________________________________________________________________________
TVirtualMCApplication* Ex06MCApplication::CloneForWorker() const
{
  return new Ex06MCApplication(*this);
}

//_____________________________________________________________________________
void Ex06MCApplication::InitForWorker() const
{
  //cout << "Ex06MCApplication::InitForWorker " << this << endl;

  // Set data to MC
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);
}

//_____________________________________________________________________________
void Ex06MCApplication::Merge(TVirtualMCApplication* localMCApplication)
{
  //cout << "Ex06MCApplication::Merge " << this << endl;

  Ex06MCApplication* ex06LocalMCApplication
    = static_cast<Ex06MCApplication*>(localMCApplication);

  fRunGammaCounter += ex06LocalMCApplication->fRunGammaCounter;
  fRunFeedbackCounter += ex06LocalMCApplication->fRunFeedbackCounter;
}
 
//_____________________________________________________________________________
void Ex06MCApplication::ConstructGeometry()
{    
/// Construct geometry using detector contruction class.
/// The detector contruction class is using TGeo functions or
/// TVirtualMC functions (if oldGeometry is selected)

  fVerbose.ConstructGeometry();

  // Cannot use Root geometry if not supported with 
  // selected MC
  if ( !fOldGeometry && ! gMC->IsRootGeometrySupported() ) {
    cerr << "Selected MC does not support TGeo geometry"<< endl;
    cerr << "Exiting program"<< endl;
    exit(1);
  } 

  if ( ! fOldGeometry ) {
    cout << "Geometry will be defined via TGeo" << endl;
    fDetConstruction->ConstructMaterials();  
    fDetConstruction->ConstructGeometry();  
  }
  else {
    cout << "Geometry will be defined via VMC" << endl;
    Ex06DetectorConstructionOld detConstructionOld;
    detConstructionOld.ConstructMaterials(); 
    detConstructionOld.ConstructGeometry();
  }    
}

//_____________________________________________________________________________
void Ex06MCApplication::ConstructOpGeometry()
{    
/// Define material optical properties

  fVerbose.ConstructGeometry();

  fDetConstruction->ConstructOpGeometry();  
}

//_____________________________________________________________________________
void Ex06MCApplication::InitGeometry()
{    
/// Initialize geometry
  
  fVerbose.InitGeometry();
}

//_____________________________________________________________________________
void Ex06MCApplication::GeneratePrimaries()
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.
  
  fVerbose.GeneratePrimaries();

  fPrimaryGenerator->GeneratePrimaries();
}

//_____________________________________________________________________________
void Ex06MCApplication::BeginEvent()
{    
/// User actions at beginning of event

  fVerbose.BeginEvent();

  fGammaCounter = 0;
  fFeedbackCounter = 0;
}

//_____________________________________________________________________________
void Ex06MCApplication::BeginPrimary()
{    
/// User actions at beginning of a primary track

  fVerbose.BeginPrimary();
}

//_____________________________________________________________________________
void Ex06MCApplication::PreTrack()
{    
/// User actions at beginning of each track

  fVerbose.PreTrack();
  
  if (gMC->TrackPid() == 50000050 ) {
    fGammaCounter++;
    fRunGammaCounter++;
  }
  if (gMC->TrackPid() == 50000051 ) {
    fFeedbackCounter++;
    fRunFeedbackCounter++;
  }
}

//_____________________________________________________________________________
void Ex06MCApplication::Stepping()
{    
/// User actions at each step

  fVerbose.Stepping();

  // Work around for Fluka VMC, which does not call
  // MCApplication::PreTrack()
  //
  static Int_t trackId = 0;
  if ( TString(gMC->GetName()) == "TFluka" &&
       gMC->GetStack()->GetCurrentTrackNumber() != trackId ) {

    fVerbose.PreTrack();
    trackId = gMC->GetStack()->GetCurrentTrackNumber();
    if (gMC->TrackPid() == 50000050 ) fGammaCounter++;
    if (gMC->TrackPid() == 50000051 ) fFeedbackCounter++;
  }

  if ( ! fTestStackPopper ) return;

  // Stack popper test
  // Add 1 feedback photon (50000051, fixed momentum) when a charged track enters TANK
  // and 3 feedback photons (momentum in opposite direction to parent photon)
  //  when a photon is stopped in TANK

  // Charged particles entering in TANK
  TString volName = gMC->CurrentVolName();
  if ( volName != "TANK" ) return;


  if ( ( gMC->TrackCharge() != 0. ) && ( gMC->IsTrackEntering() ) ) {
    // 1 keV
    Double_t energy = 1e-06;
    TLorentzVector momentum(energy, 0., 0., energy);
    GenerateFeedback(1,momentum );
  }

  if ( ( gMC->TrackPid() == 50000050 ) && ( gMC->Edep() > 0. ) ) {
    TLorentzVector momentum;
    gMC->TrackMomentum(momentum);
    momentum = -1.*momentum;
    GenerateFeedback(3, momentum);
  }
}

//_____________________________________________________________________________
void Ex06MCApplication::GenerateFeedback(Int_t nofPhotons,
                                         TLorentzVector momentum)
{
/// Generate FeedBack photons for the current particle.

  for (Int_t i=0; i<nofPhotons; ++i) {
     // same position as the parent track
      TLorentzVector position;
      gMC->TrackPosition(position);
      // Feedback photon
      Int_t pdgEncoding = 50000051;
      // Fixed polarization
      Double_t  polarization[3];
      polarization[0] = 0.3;
      polarization[1] = 0.4;
      polarization[2] = 0.866025403784438597;

      Int_t ntrack;
      gMC->GetStack()->PushTrack(
                         1,
                         gMC->GetStack()->GetCurrentTrackNumber(),
                         pdgEncoding,
		         momentum.X(), momentum.Y(), momentum.Z(), momentum.T(),
		         position.X(), position.Y(), position.Z(), position.T(),
                         polarization[0],polarization[1],polarization[2],
		         kPFeedBackPhoton,
                         ntrack, 1.0, 0);
  }
}

//_____________________________________________________________________________
void Ex06MCApplication::PostTrack()
{    
/// User actions after finishing of each track

  fVerbose.PostTrack();
}

//_____________________________________________________________________________
void Ex06MCApplication::FinishPrimary()
{    
/// User actions after finishing of a primary track

  fVerbose.FinishPrimary();
}

//_____________________________________________________________________________
void Ex06MCApplication::FinishEvent()
{    
/// User actions after finishing of an event

  fVerbose.FinishEvent();
  
  // Geant3 + TGeo
  // (use TGeo functions for visualization)
  if ( TString(gMC->GetName()) == "TGeant3TGeo") {
  
     // Draw volume 
     gGeoManager->SetVisOption(0);	 
     gGeoManager->SetTopVisible();
     gGeoManager->GetTopVolume()->Draw();

     // Draw tracks (if filled)
     // Available when this feature is activated via
     // gMC->SetCollectTracks(kTRUE);
     if ( gGeoManager->GetListOfTracks() &&
          gGeoManager->GetTrack(0) &&
        ((TVirtualGeoTrack*)gGeoManager->GetTrack(0))->HasPoints() ) {
       
       gGeoManager->DrawTracks("/*");  // this means all tracks
    }	  
  }    

  cout << "Number of optical photons produced in this event : "
       << fGammaCounter << endl;

  if ( fTestStackPopper) {
    cout << "Number of feedback photons produced in this event : "
         << fFeedbackCounter << endl;
  }
  fStack->Reset();
} 

//_____________________________________________________________________________
void Ex06MCApplication::FinishRun()
{
/// User actions after finishing of a run

  fVerbose.FinishRun();

  cout << "Number of optical photons produced in this run : "
       << fRunGammaCounter << endl;

  if ( fTestStackPopper) {
    cout << "Number of feedback photons produced in this run : "
         << fRunFeedbackCounter << endl;
  }
}
