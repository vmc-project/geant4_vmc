// $Id: Ex03MCApplication.cxx,v 1.2 2003/06/03 17:05:49 brun Exp $
//
// Geant4 ExampleN03 adapted to Virtual Monte Carlo 
//
// Class Ex03MCApplication
// ----------------------- 
// Implementation of the TVirtualMCApplication
//
// by Ivana Hrivnacova, 6.3.2003

#include <TROOT.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TRandom.h>
#include <TPDGCode.h>
#include <TVector3.h>
#include <Riostream.h>

#include "Ex03MCApplication.h"
#include "Ex03MCStack.h"
#include "Ex03PrimaryGenerator.h"

ClassImp(Ex03MCApplication)

//_____________________________________________________________________________
Ex03MCApplication::Ex03MCApplication(const char *name, const char *title,
                                     FileMode fileMode) 
  : TVirtualMCApplication(name,title),
    fPrintModulo(1),
    fEventNo(0),
    fVerbose(0),
    fStack(0),
    fDetConstruction(0),
    fCalorimeterSD(0),
    fPrimaryGenerator(0),
    fFieldB(0),
    fRootManager("example03", fileMode)
{
// Standard constructor
// ---

  // Create a user stack
  fStack = new Ex03MCStack(1000);
  
  // Create detector construction
  fDetConstruction = new Ex03DetectorConstruction();
  
  // Create a calorimeter SD
  fCalorimeterSD = new Ex03CalorimeterSD("Calorimeter", fDetConstruction); 
  
  // Create a primary generator
  fPrimaryGenerator = new Ex03PrimaryGenerator(fStack);
  
  // Constant magnetic field (in kiloGauss)
  fFieldB = new Double_t[3];
  fFieldB[0] = 0.;
  fFieldB[1] = 0.;
  fFieldB[2] = 0.;
}

//_____________________________________________________________________________
Ex03MCApplication::Ex03MCApplication()
  : TVirtualMCApplication(),
    fPrintModulo(1),
    fEventNo(0),
    fStack(0),
    fDetConstruction(0),
    fCalorimeterSD(0),
    fPrimaryGenerator(0),
    fFieldB(0),
    fRootManager()
{    
// Default constructor
// ---
}

//_____________________________________________________________________________
Ex03MCApplication::~Ex03MCApplication() 
{
// Destructor  
// ---
  
  delete fStack;
  delete fDetConstruction;
  delete fCalorimeterSD;
  delete fPrimaryGenerator;
  delete fFieldB;
  delete gMC;
  gMC = 0;
}

//
// private methods
//

//_____________________________________________________________________________
void Ex03MCApplication::RegisterStack()
{
// Register stack in Root manager.
// ---

  fRootManager.Register("stack", "Ex03MCStack", &fStack);   
}  

//
// public methods
//

//_____________________________________________________________________________
void Ex03MCApplication::InitMC(const char* setup)
{    
// Initialize MC.
// ---

  fVerbose.InitMC();

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
 
  gMC->SetStack(fStack);
  //gMC->DefineParticles();
  gMC->Init();
  gMC->BuildPhysics(); 
  
  RegisterStack();
}

//_____________________________________________________________________________
void Ex03MCApplication::RunMC(Int_t nofEvents)
{    
// MC run.
// ---

  fVerbose.RunMC(nofEvents);

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void Ex03MCApplication::FinishRun()
{    
// Finish MC run.
// ---

  fVerbose.FinishRun();

  fRootManager.Write();
}

//_____________________________________________________________________________
void Ex03MCApplication::ReadEvent(Int_t i) 
{
// Read i-th event from Root file.    
// ---

  fCalorimeterSD->Register();
  RegisterStack();
  fRootManager.ReadEvent(i);
}  
  
//_____________________________________________________________________________
void Ex03MCApplication::ConstructGeometry()
{    
// Construct geometry using detector contruction class
// ---

  fVerbose.ConstructGeometry();

  fDetConstruction->ConstructMaterials();  
  fDetConstruction->ConstructGeometry();  
}

//_____________________________________________________________________________
void Ex03MCApplication::InitGeometry()
{    
// Initialize geometry
// ---
  
  fVerbose.InitGeometry();

  fCalorimeterSD->Initialize();
}

//_____________________________________________________________________________
void Ex03MCApplication::GeneratePrimaries()
{    
// Fill the user stack (derived from TVirtualMCStack) with primary particles.
// ---
  
  fVerbose.GeneratePrimaries();

  TVector3 origin(fDetConstruction->GetWorldSizeX(),
                  fDetConstruction->GetCalorSizeYZ(),
                  fDetConstruction->GetCalorSizeYZ());
		     
  fPrimaryGenerator->GeneratePrimaries(origin);
}

//_____________________________________________________________________________
void Ex03MCApplication::BeginEvent()
{    
// User actions at beginning of event
// ---

  fVerbose.BeginEvent();

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
// User actions at beginning of a primary track
// ---

  fVerbose.BeginPrimary();
}

//_____________________________________________________________________________
void Ex03MCApplication::PreTrack()
{    
// User actions at beginning of each track
// ---

  fVerbose.PreTrack();
}

//_____________________________________________________________________________
void Ex03MCApplication::Stepping()
{    
// User actions at each step
// ---

  fVerbose.Stepping();

  fCalorimeterSD->ProcessHits();
}

//_____________________________________________________________________________
void Ex03MCApplication::PostTrack()
{    
// User actions after finishing of each track
// ---

  fVerbose.PostTrack();
}

//_____________________________________________________________________________
void Ex03MCApplication::FinishPrimary()
{    
// User actions after finishing of a primary track
// ---

  fVerbose.FinishPrimary();
}

//_____________________________________________________________________________
void Ex03MCApplication::FinishEvent()
{    
// User actions after finishing of an event
// ---

  fVerbose.FinishEvent();

  if (TString(gMC->GetName()) == "TGeant3") {
    // add scale (1.4)
    gMC->Gdraw("WRLD", 30., 30., 0, 10., 10., .75, .75);
  }  
 
  fRootManager.Fill();

  if (fEventNo % fPrintModulo == 0) 
    fCalorimeterSD->PrintTotal();

  fCalorimeterSD->EndOfEvent();

  fStack->Reset();
} 

//_____________________________________________________________________________
Double_t Ex03MCApplication::TrackingRmax() const
{ 
// No limit
// ---

  return DBL_MAX; 
}

//_____________________________________________________________________________
Double_t Ex03MCApplication::TrackingZmax() const
{ 
// No limit
// ---

  return DBL_MAX; 
}

//_____________________________________________________________________________
void Ex03MCApplication::Field(const Double_t* x, Double_t* b) const
{
// Uniform magnetic field
// ---
  
   for (Int_t i=0; i<3; i++) b[i] = fFieldB[i];
}
 
