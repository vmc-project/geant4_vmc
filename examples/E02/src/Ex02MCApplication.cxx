// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex02MCApplication.cxx 
/// \brief Implementation of the Ex02MCApplication class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex02MCApplication.h"
#include "Ex02MCStack.h"
#include "Ex02DetectorConstructionOld.h"

#include <TROOT.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TPDGCode.h>
#include <TGeoManager.h>
#include <TGeoUniformMagField.h>
#include <TVirtualGeoTrack.h>
#include <Riostream.h>

/// \cond CLASSIMP
ClassImp(Ex02MCApplication)
/// \endcond

//_____________________________________________________________________________
Ex02MCApplication::Ex02MCApplication(const char *name, const char *title,
                                     FileMode fileMode) 
  : TVirtualMCApplication(name,title),
    fStack(0),
    fDetConstruction(),
    fTrackerSD("Tracker Chamber"),
    fMagField(0),
    fRootManager("example02", fileMode),
    fOldGeometry(kFALSE)
{
/// Standard constructor
/// \param name   The MC application name 
/// \param title  The MC application description
/// \param fileMode  Option for opening Root file (read or write mode)

  // Create a user stack
  fStack = new Ex02MCStack(100); 
  
  // Constant magnetic field (in kiloGauss)
  fMagField = new TGeoUniformMagField(20., 0., 0.);
}

//_____________________________________________________________________________
Ex02MCApplication::Ex02MCApplication()
  : TVirtualMCApplication(),
    fStack(0),
    fDetConstruction(),
    fTrackerSD(),
    fMagField(0),
    fRootManager(),
    fOldGeometry(kFALSE)
{    
/// Default constructor
}

//_____________________________________________________________________________
Ex02MCApplication::~Ex02MCApplication() 
{
/// Destructor  
  
  delete fStack;
  delete fMagField;
  delete gMC;
  gMC = 0;
}

//
// private methods
//
//_____________________________________________________________________________
void Ex02MCApplication::RegisterStack()
{
/// Register stack in the Root manager.

  fRootManager.Register("stack", "Ex02MCStack", &fStack);   
}  

//
// public methods
//

//_____________________________________________________________________________
void Ex02MCApplication::InitMC(const char* setup)
{    
/// Initialize MC.
/// The selection of the concrete MC is done in the macro.
/// \param setup The name of the configuration macro 

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
 
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);
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

  fRootManager.WriteAll();
}

//_____________________________________________________________________________
void Ex02MCApplication::ConstructGeometry()
{    
/// Construct geometry using detector contruction class.
/// The detector contruction class is using TGeo functions or
/// TVirtualMC functions (if oldGeometry is selected)

  // Cannot use Root geometry if not supported with 
  // selected MC
  if ( !fOldGeometry && ! gMC->IsRootGeometrySupported() ) {
    cerr << "Selected MC does not support TGeo geometry"<< endl;
    cerr << "Exiting program"<< endl;
    exit(1);
  } 

  if ( ! fOldGeometry ) {
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
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.
  
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
/// User actions at beginning of event.
/// Nothing to be done this example
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

  fTrackerSD.ProcessHits();
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
void  Ex02MCApplication::ReadEvent(Int_t i) 
{
/// Read \em i -th event and prints hits.
/// \param i The number of event to be read    
  
  fTrackerSD.Register();
  RegisterStack();
  fRootManager.ReadEvent(i);

  fStack->Print();  
  fTrackerSD.Print();
}  
  
