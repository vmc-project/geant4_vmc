// $Id: Ex02MCApplication.cxx,v 1.9 2005/05/17 13:44:25 brun Exp $
//
// Geant4 ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex02MCApplication
// ----------------------- 
// Implementation of the TVirtualMCApplication
//
// by Ivana Hrivnacova, 21.4.2002

#include "Ex02MCApplication.h"
#include "Ex02MCStack.h"

#include <TROOT.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TPDGCode.h>
#include <TGeoManager.h>
#include <TVirtualGeoTrack.h>

ClassImp(Ex02MCApplication)

//_____________________________________________________________________________
Ex02MCApplication::Ex02MCApplication(const char *name, const char *title,
                                     FileMode fileMode) 
  : TVirtualMCApplication(name,title),
    fStack(0),
    fDetConstruction(),
    fTrackerSD("Tracker Chamber"),
    fFieldB(0),
    fRootManager("example02", fileMode)
{
// Standard constructor
// ---

  // Create a user stack
  fStack = new Ex02MCStack(100); 
  
  // Constant magnetic field (in kiloGauss)
  fFieldB = new Double_t[3];
  fFieldB[0] = 20.;
  fFieldB[1] = 0.;
  fFieldB[2] = 0.;
}

//_____________________________________________________________________________
Ex02MCApplication::Ex02MCApplication()
  : TVirtualMCApplication(),
    fStack(0),
    fDetConstruction(),
    fTrackerSD(),
    fFieldB(0),
    fRootManager()
{    
// Default constructor
// ---
}

//_____________________________________________________________________________
Ex02MCApplication::~Ex02MCApplication() 
{
// Destructor  
// ---
  
  delete fStack;
  delete fFieldB;
  delete gMC;
  gMC = 0;
}

//
// private methods
//
//_____________________________________________________________________________
void Ex02MCApplication::RegisterStack()
{
// Registers stack in Root manager.
// ---

  fRootManager.Register("stack", "Ex02MCStack", &fStack);   
}  

//
// public methods
//

//_____________________________________________________________________________
void Ex02MCApplication::InitMC(const char* setup)
{    
// Initialize MC.
// ---

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
 
  gMC->SetStack(fStack);
  gMC->Init();
  gMC->BuildPhysics(); 
  
  RegisterStack();
}

//_____________________________________________________________________________
void Ex02MCApplication::RunMC(Int_t nofEvents)
{    
// MC run.
// ---

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void Ex02MCApplication::FinishRun()
{    
// Finish MC run.
// ---

  fRootManager.WriteAll();
}

//_____________________________________________________________________________
void Ex02MCApplication::ConstructGeometry()
{    
// Construct geometry using detector contruction class
// ---

  fDetConstruction.ConstructMaterials();  
  fDetConstruction.ConstructGeometry();  
}

//_____________________________________________________________________________
void Ex02MCApplication::InitGeometry()
{    
// Initialize geometry
// ---
  
  fTrackerSD.Initialize();

  if (TString(gMC->GetName()) == "TGeant3") {
    // Set drawing options
    gMC->Gsatt("*", "seen", 16);
    gMC->Gsatt("CHMB", "seen", 5);
  }  
}

//_____________________________________________________________________________
void Ex02MCApplication::GeneratePrimaries()
{    
// Fill the user stack (derived from TVirtualMCStack) with primary particles.
// ---
  
 // Track ID (filled by stack)
 Int_t ntr;
 
 // Option: to be tracked
 Int_t toBeDone = 1; 
 
 // Particle type
 //Int_t pdg  = 0;    // geantino
 Int_t pdg  = kProton;
 
 // Polarization
 Double_t polx = 0.; 
 Double_t poly = 0.; 
 Double_t polz = 0.; 

 // Position
 Double_t vx  = 0.; 
 Double_t vy  = 0.; 
 Double_t vz = -0.5*(fDetConstruction.GetWorldFullLength());
 Double_t tof = 0.;

 // Energy
 Double_t kinEnergy = 3.0;
 Double_t mass = 0.9382723;
 Double_t e  = mass + kinEnergy;
 
 // Momentum
 Double_t px, py, pz;
 px = 0.; 
 py = 0.; 
 pz = sqrt(e*e - mass*mass); 

 // Add particle to stack 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                  kPPrimary, ntr, 1., 0);
}

//_____________________________________________________________________________
void Ex02MCApplication::BeginEvent()
{    
// User actions at beginning of event
// ---

  // nothing to be done this example
}

//_____________________________________________________________________________
void Ex02MCApplication::BeginPrimary()
{    
// User actions at beginning of a primary track
// ---

  // nothing to be done this example
}

//_____________________________________________________________________________
void Ex02MCApplication::PreTrack()
{    
// User actions at beginning of each track
// ---

  // nothing to be done this example
}

//_____________________________________________________________________________
void Ex02MCApplication::Stepping()
{    
// User actions at each step
// ---

  fTrackerSD.ProcessHits();
}

//_____________________________________________________________________________
void Ex02MCApplication::PostTrack()
{    
// User actions after finishing of each track
// ---

  // nothing to be done this example
}

//_____________________________________________________________________________
void Ex02MCApplication::FinishPrimary()
{    
// User actions after finishing of a primary track
// ---

  // nothing to be done this example
}

//_____________________________________________________________________________
void Ex02MCApplication::FinishEvent()
{    
// User actions after finishing of an event
// ---

  // Geant3
  // (visualization functions interfaced via VMC)
  if (TString(gMC->GetName()) == "TGeant3") {
    // add scale (1.4)
    gMC->Gdraw("WRLD", 90., 180.);
  }  
 
  // Geant3 + TGeo
  // (use TGeo functions for visualization)
  if ( TString(gMC->GetName()) == "TGeant3TGeo" && 
       gGeoManager->GetListOfTracks() &&
       gGeoManager->GetTrack(0) &&
       ((TVirtualGeoTrack*)gGeoManager->GetTrack(0))->HasPoints() ) {
       
       gGeoManager->SetVisOption(0);	 
       gGeoManager->SetTopVisible();
       gGeoManager->DrawTracks("/*");  // this means all tracks
          // Drawing G3 tracks via TGeo is available only
	  // if geant3 is compile with -DCOLLECT_TRACK flag
	  // (to be activated in geant3/TGeant3/TGeant3gu.cxx)
  }    

  // Geant4 visualization is activated via G4 macro (g4config.in)
 
  fRootManager.Fill();

  fTrackerSD.EndOfEvent();

  fStack->Print();  
  fStack->Reset();
} 

//_____________________________________________________________________________
void Ex02MCApplication::Field(const Double_t* x, Double_t* b) const
{
// Uniform magnetic field
// ---
  
   for (Int_t i=0; i<3; i++) b[i] = fFieldB[i];
}

//_____________________________________________________________________________
void  Ex02MCApplication::ReadEvent(Int_t i) 
{
// Reads i-th event and prints hits.    
// ---
  
  fTrackerSD.Register();
  RegisterStack();
  fRootManager.ReadEvent(i);

  fStack->Print();  
  fTrackerSD.Print();
}  
  
