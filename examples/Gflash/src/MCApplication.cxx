//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Gflash/src/MCApplication.cxx
/// \brief Implementation of the Gflash::MCApplication class
///
/// Geant4 gflash adapted to Virtual Monte Carlo
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "MCApplication.h"
#include "Ex03MCStack.h"
#include "Hit.h"
#include "PrimaryGenerator.h"

#include <TMCRootManager.h>
#include <TMCRootManagerMT.h>

#include <TROOT.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TRandom.h>
#include <TPDGCode.h>
#include <TVector3.h>
#include <Riostream.h>
#include <TGeoManager.h>
#include <TVirtualGeoTrack.h>
#include <TParticle.h>

using namespace std;

/// \cond CLASSIMP
ClassImp(VMC::Gflash::MCApplication)
/// \endcond

namespace VMC
{
namespace Gflash
{

//_____________________________________________________________________________
MCApplication::MCApplication(const char *name, const char *title)
  : TVirtualMCApplication(name,title),
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
  : TVirtualMCApplication(origin.GetName(),origin.GetTitle()),
    fRootManager(0),
    fEventNo(0),
    fVerbose(origin.fVerbose),
    fStack(0),
    fDetConstruction(origin.fDetConstruction),
    fSensitiveDetector(0),
    fPrimaryGenerator(0),
    fIsMaster(kFALSE)
{
/// Copy constructor for cloning application on workers (in multithreading mode)
/// \param origin   The source MC application

  // Create new user stack
  fStack = new Ex03MCStack(1000);

  // Create a calorimeter SD
  fSensitiveDetector
    = new SensitiveDetector(*(origin.fSensitiveDetector));

  // Create a primary generator
  fPrimaryGenerator
    = new PrimaryGenerator(*(origin.fPrimaryGenerator), fStack);
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
  
  //cout << "MCApplication::~MCApplication " << this << endl;

  delete fRootManager;
  delete fStack;
  if ( fIsMaster) delete fDetConstruction;
  delete fSensitiveDetector;
  delete fPrimaryGenerator;
  delete gMC;

  //cout << "Done MCApplication::~MCApplication " << this << endl;
}

//
// private methods
//

//_____________________________________________________________________________
void MCApplication::RegisterStack() const
{
/// Register stack in the Root manager.

  if ( fRootManager ) {
    //cout << "MCApplication::RegisterStack: " << endl;
    fRootManager->Register("stack", "Ex03MCStack", &fStack);
  }
}

//_____________________________________________________________________________
void MCApplication::ComputeEventStatistics() const
{
/// Compute event statisics

  cout<<" ------ ExGflashEventAction::End of event nr. "<<fEventNo<<"  -----"<< endl;   
  
  TClonesArray* hitsCollection 
   = fSensitiveDetector->GetHitsCollection();

  // Hits in sensitive Detector
  int n_hit = hitsCollection->GetEntriesFast();
  cout<<"  " << n_hit<< " hits are stored in HitsCollection "<<endl;

  // Get (x,y,z) of vertex of initial particles  
  TVector3 vertexPosition = fPrimaryGenerator->GetVertexPosition();
  TVector3 vertexDirection = fPrimaryGenerator->GetVertexDirection();

  // ExGflashEventAction: Magicnumber
  // Should be retrieved from detector construction
  Double_t energyincrystal[100];
  for (Int_t i=0; i<100; i++) energyincrystal[i]=0.;
  
  // For all Hits in sensitive detector
  Double_t totE = 0;
  for (Int_t i=0; i<n_hit; i++) {
    Hit* hit = static_cast<Hit*>(hitsCollection->At(i));

    Double_t estep = hit->GetEdep();
    if ( estep > 0 ) {
      totE += estep;
      Int_t num = hit->GetCrystalNum();
      energyincrystal[num] += estep;
      // cout << " Crystal Nummer " <<  num  << endl;

      TVector3 hitpos = hit->GetPos();            
      TVector3 l(hitpos);
      // distance from shower start
      l = l - vertexPosition; 
      // projection on shower axis = longitudinal profile
      TVector3 longitudinal(l);  
      // shower profiles (Radial)
      TVector3 radial(vertexPosition.Cross(l));
    }
  }

  //Find crystal with maximum energy
  Double_t  max = 0;
  Int_t index = 0;
  for (Int_t i=0; i<100; i++)  {
    // cout << i <<"  " << energyincrystal[i] << G4endl;
    if ( max < energyincrystal[i] ) {
      max = energyincrystal[i];
      index = i;
    }
  }  
  // cout << " NMAX  " << index << G4endl; 

  // 3x3 matrix of crystals around the crystal with the maximum energy deposit
  Double_t e3x3
    = energyincrystal[index]    + energyincrystal[index+1]  + energyincrystal[index-1]
    + energyincrystal[index-10] + energyincrystal[index-9]  + energyincrystal[index-11]
    + energyincrystal[index+10] + energyincrystal[index+11] + energyincrystal[index+9];
  
  // 5x5 matrix of crystals around the crystal with the maximum energy deposit  
  Double_t e5x5
     = energyincrystal[index] + energyincrystal[index+1] + energyincrystal[index-1] 
     + energyincrystal[index+2] + energyincrystal[index-2] 
     + energyincrystal[index-10] + energyincrystal[index-9] + energyincrystal[index-11] 
     + energyincrystal[index-8] + energyincrystal[index-12] 
     + energyincrystal[index+10] + energyincrystal[index+11] + energyincrystal[index+9] 
     + energyincrystal[index+12] + energyincrystal[index+8];
    
  cout << "   e1  " << energyincrystal[index]  
       << "   e3x3  " << e3x3<<  "   e5x5  "  << e5x5 << " GeV" << endl;  
  
  cout << " Total energy deposited in the calorimeter: " << totE << " (GeV)" << endl;  
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

  if ( TString(setup) != "" ) {
    gROOT->LoadMacro(setup);
    gInterpreter->ProcessLine("Config()");
    if ( ! gMC ) {
      Fatal("InitMC",
            "Processing Config() has failed. (No MC is instantiated.)");
    }
  }  
 
// MT support available from root v 5.34/18
#if ROOT_VERSION_CODE >= 336402
  // Create Root manager
  if ( ! gMC->IsMT() ) {
    fRootManager
      = new TMCRootManager(GetName(), TVirtualMCRootManager::kWrite);
    //fRootManager->SetDebug(true);
  }
#else
  // Create Root manager
  fRootManager
    = new TMCRootManager(GetName(), TVirtualMCRootManager::kWrite);
  //fRootManager->SetDebug(true);
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
  //cout << "MCApplication::FinishRun: " << endl;
  if ( fRootManager ) {
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
  //cout << "MCApplication::InitForWorker " << this << endl;

  // Create Root manager
  fRootManager
    = new TMCRootManagerMT(GetName(), TVirtualMCRootManager::kWrite);
  //fRootManager->SetDebug(true);

  // Set data to MC
  gMC->SetStack(fStack);

  RegisterStack();
}

//_____________________________________________________________________________
void MCApplication::FinishWorkerRun() const
{
  //cout << "MCApplication::FinishWorkerRun: " << endl;
  if ( fRootManager ) {
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
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.
  
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
  if (   TString(gMC->GetName()) == "TGeant3TGeo" && 
         gGeoManager->GetListOfTracks() &&
         gGeoManager->GetTrack(0) &&
       ((TVirtualGeoTrack*)gGeoManager->GetTrack(0))->HasPoints() ) {
       
       gGeoManager->ClearTracks();	  
       //if (gPad) gPad->Clear();	  
  }    

  fEventNo++;
  cout<<" Start generating event Nr "<< fEventNo << endl;

  // start timer
  fEventTimer = new TStopwatch();
  fEventTimer->Start();
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
  //cout << "MCApplication::Stepping" << this << endl;
  static Int_t trackId = 0;
  if ( TString(gMC->GetName()) == "TFluka" &&
       gMC->GetStack()->GetCurrentTrackNumber() != trackId ) {
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

  // VMC 

  fVerbose.FinishEvent();

  fRootManager->Fill();
// The application code

  fEventTimer->Stop();
  cout << endl;
  cout << "******************************************";
  cout << endl;
  cout << "Elapsed Time: " << endl;
  fEventTimer->Print();
  cout << endl;
  cout << "******************************************"<< endl;
  //TO DO
  //fDtime+=fTimerIntern.GetRealElapsed();

  ComputeEventStatistics();

  fSensitiveDetector->EndOfEvent();

  fStack->Reset();  
}

}
}

