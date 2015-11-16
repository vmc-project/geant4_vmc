//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05MCApplication.cxx 
/// \brief Implementation of the Ex05MCApplication class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex05MCApplication.h"
#include "Ex03MCStack.h"
#include "Ex05Hit.h"
#include "Ex05PrimaryGenerator.h"

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
ClassImp(Ex05MCApplication)
/// \endcond

//_____________________________________________________________________________
Ex05MCApplication::Ex05MCApplication(const char *name, const char *title)
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
  fDetConstruction = new Ex05DetectorConstruction();
  
  // Create a calorimeter SD
  fSensitiveDetector = new Ex05SensitiveDetector("Calorimeter"); 
  
  // Create a primary generator
  fPrimaryGenerator = new Ex05PrimaryGenerator(fStack);
}

//_____________________________________________________________________________
Ex05MCApplication::Ex05MCApplication(const Ex05MCApplication& origin)
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
/// \param \origin   The source MC application

  // Create new user stack
  fStack = new Ex03MCStack(1000);

  // Create a calorimeter SD
  fSensitiveDetector
    = new Ex05SensitiveDetector(*(origin.fSensitiveDetector));

  // Create a primary generator
  fPrimaryGenerator
    = new Ex05PrimaryGenerator(*(origin.fPrimaryGenerator), fStack);
}

//_____________________________________________________________________________
Ex05MCApplication::Ex05MCApplication()
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
Ex05MCApplication::~Ex05MCApplication() 
{
/// Destructor  
  
  //cout << "Ex05MCApplication::~Ex05MCApplication " << this << endl;

  delete fRootManager;
  delete fStack;
  if ( fIsMaster) delete fDetConstruction;
  delete fSensitiveDetector;
  delete fPrimaryGenerator;
  delete gMC;

  //cout << "Done Ex05MCApplication::~Ex05MCApplication " << this << endl;
}

//
// private methods
//

//_____________________________________________________________________________
void Ex05MCApplication::RegisterStack() const
{
/// Register stack in the Root manager.

  if ( fRootManager ) {
    //cout << "Ex05MCApplication::RegisterStack: " << endl;
    fRootManager->Register("stack", "Ex03MCStack", &fStack);
  }
}

//_____________________________________________________________________________
void Ex05MCApplication::ComputeEventStatistics() const
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
    Ex05Hit* hit = static_cast<Ex05Hit*>(hitsCollection->At(i));

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
void Ex05MCApplication::InitMC(const char* setup)
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
void Ex05MCApplication::RunMC(Int_t nofEvents)
{    
/// Run MC.
/// \param nofEvents Number of events to be processed

  fVerbose.RunMC(nofEvents);

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void Ex05MCApplication::FinishRun()
{    
/// Finish MC run.

  fVerbose.FinishRun();
  //cout << "Ex05MCApplication::FinishRun: " << endl;
  if ( fRootManager ) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

//_____________________________________________________________________________
TVirtualMCApplication* Ex05MCApplication::CloneForWorker() const
{
  return new Ex05MCApplication(*this);
}

//_____________________________________________________________________________
void Ex05MCApplication::InitForWorker() const
{
  //cout << "Ex05MCApplication::InitForWorker " << this << endl;

  // Create Root manager
  fRootManager
    = new TMCRootManagerMT(GetName(), TVirtualMCRootManager::kWrite);
  //fRootManager->SetDebug(true);

  // Set data to MC
  gMC->SetStack(fStack);

  RegisterStack();
}

//_____________________________________________________________________________
void Ex05MCApplication::FinishWorkerRun() const
{
  //cout << "Ex05MCApplication::FinishWorkerRun: " << endl;
  if ( fRootManager ) {
    fRootManager->WriteAll();
    fRootManager->Close();
  }
}

//_____________________________________________________________________________
void Ex05MCApplication::ReadEvent(Int_t i) 
{
/// Read \em i -th event and prints hits.
/// \param i The number of event to be read    

  fSensitiveDetector->Register();
  RegisterStack();
  fRootManager->ReadEvent(i);
}  
  
//_____________________________________________________________________________
void Ex05MCApplication::ConstructGeometry()
{    
/// Construct geometry using detector contruction class.
/// The detector contruction class is using TGeo functions or
/// TVirtualMC functions (if oldGeometry is selected)

  fVerbose.ConstructGeometry();

  fDetConstruction->Construct();  
}

//_____________________________________________________________________________
void Ex05MCApplication::InitGeometry()
{    
/// Initialize geometry
  
  fVerbose.InitGeometry();
  
  fSensitiveDetector->Initialize();
}

//_____________________________________________________________________________
void Ex05MCApplication::GeneratePrimaries()
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.
  
  fVerbose.GeneratePrimaries();

  TVector3 origin;		     
  fPrimaryGenerator->GeneratePrimaries(origin);
}

//_____________________________________________________________________________
void Ex05MCApplication::BeginEvent()
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
void Ex05MCApplication::BeginPrimary()
{    
/// User actions at beginning of a primary track.
/// If test for user defined decay is activated,
/// the primary track ID is printed on the screen.

  fVerbose.BeginPrimary();   
}

//_____________________________________________________________________________
void Ex05MCApplication::PreTrack()
{    
/// User actions at beginning of each track
/// If test for user defined decay is activated,
/// the decay products of the primary track (K0Short)
/// are printed on the screen.

  fVerbose.PreTrack();
}

//_____________________________________________________________________________
void Ex05MCApplication::Stepping()
{    
/// User actions at each step

  // Work around for Fluka VMC, which does not call
  // MCApplication::PreTrack()
  //
  //cout << "Ex05MCApplication::Stepping" << this << endl;
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
void Ex05MCApplication::PostTrack()
{    
/// User actions after finishing of each track

  fVerbose.PostTrack();
}

//_____________________________________________________________________________
void Ex05MCApplication::FinishPrimary()
{    
/// User actions after finishing of a primary track

  fVerbose.FinishPrimary();
}

//_____________________________________________________________________________
void Ex05MCApplication::FinishEvent()
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

