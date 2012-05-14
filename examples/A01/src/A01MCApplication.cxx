// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2012 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file A01MCApplication.cxx 
/// \brief Implementation of the A01MCApplication class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <TROOT.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TRandom.h>
#include <TPDGCode.h>
#include <TVector3.h>
#include <Riostream.h>
#include <TGeoManager.h>
#include <TGeoUniformMagField.h>
#include <TVirtualGeoTrack.h>
#include <TParticle.h>
#include <TParticlePDG.h>

#include "Ex03MCStack.h"
#include "A01MCApplication.h"
#include "A01PrimaryGenerator.h"
#include "A01RootDetectorConstruction.h"
#include "A01MagField.h"
#include "A01DriftChamberSD.h"
#include "A01EmCalorimeterSD.h"
#include "A01HadCalorimeterSD.h"
#include "A01HodoscopeSD.h"


/// \cond CLASSIMP
ClassImp(A01MCApplication)
/// \endcond

//_____________________________________________________________________________
A01MCApplication::A01MCApplication(const char *name, const char *title,
                                   FileMode fileMode) 
  : TVirtualMCApplication(name,title),
    fWriteStack(true),
    fWriteHits(true),
    fVerbose(0),
    fStack(0),
    fDetConstruction(0),
    fDriftChamberSD1(0),
    fDriftChamberSD2(0),
    fEmCalorimeterSD(0),
    fHadCalorimeterSD(0),
    fHodoscopeSD1(0),
    fHodoscopeSD2(0),
    fPrimaryGenerator(0),
    fMagField(0),
    fRootManager("A01", fileMode)
{
/// Standard constructor
/// \param name   The MC application name 
/// \param title  The MC application description
/// \param fileMode  Option for opening Root file (read or write mode)

  // Create a user stack
  fStack = new Ex03MCStack(1000);
  
  // Create detector construction
  fDetConstruction = new A01RootDetectorConstruction("A01geometry.root");
  
  // Create SDs
  fDriftChamberSD1 = new A01DriftChamberSD("Chamber1", "wirePlane1Logical");
  fDriftChamberSD2 = new A01DriftChamberSD("Chamber2", "wirePlane2Logical");
  fEmCalorimeterSD = new A01EmCalorimeterSD("EmCalorimeter"); 
  fHadCalorimeterSD = new A01HadCalorimeterSD("HadCalorimeter"); 
  fHodoscopeSD1 = new A01HodoscopeSD("Hodoscope1", "hodoscope1Logical");
  fHodoscopeSD2 = new A01HodoscopeSD("Hodoscope2", "hodoscope2Logical");
  
  // Create a primary generator
  fPrimaryGenerator = new A01PrimaryGenerator(fStack);
  
  // Constant magnetic field (in kiloGauss)
  // field value: 1.0*tesla (= 10.0 kiloGauss) in y
  fMagField = new A01MagField(0, 10.0, 0);
}

//_____________________________________________________________________________
A01MCApplication::A01MCApplication()
  : TVirtualMCApplication(),
    fWriteStack(true),
    fWriteHits(true),
    fStack(0),
    fDetConstruction(0),
    fDriftChamberSD1(0),
    fDriftChamberSD2(0),
    fEmCalorimeterSD(0),
    fHadCalorimeterSD(0),
    fHodoscopeSD1(0),
    fHodoscopeSD2(0),
    fPrimaryGenerator(0),
    fMagField(0),
    fRootManager()
{    
/// Default constructor
}

//_____________________________________________________________________________
A01MCApplication::~A01MCApplication() 
{
/// Destructor  
  
  delete fStack;
  delete fDetConstruction;
  delete fDriftChamberSD1;
  delete fDriftChamberSD2;
  delete fEmCalorimeterSD;
  delete fHadCalorimeterSD;
  delete fHodoscopeSD1;
  delete fHodoscopeSD2;
  delete fPrimaryGenerator;
  delete fMagField;
  delete gMC;
  gMC = 0;
}

//
// private methods
//

//_____________________________________________________________________________
void A01MCApplication::RegisterStack()
{
/// Register stack in the Root manager.

  if ( fWriteStack ) {
    fRootManager.Register("stack", "Ex03MCStack", &fStack);   
  }  
}  

//
// public methods
//

//_____________________________________________________________________________
void A01MCApplication::InitMC(const char* setup)
{    
/// Initialize MC.
/// The selection of the concrete MC is done in the macro.
/// \param setup The name of the configuration macro 

  fVerbose.InitMC();

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
 
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);
  gMC->Init();
  gMC->BuildPhysics(); 
  
  RegisterStack();
}                                   

//_____________________________________________________________________________
void A01MCApplication::RunMC(Int_t nofEvents)
{    
/// Run MC.
/// \param nofEvents Number of events to be processed

  fVerbose.RunMC(nofEvents);

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void A01MCApplication::FinishRun()
{    
/// Finish MC run.

  fVerbose.FinishRun();

  if ( fWriteStack || fWriteHits ) {
    fRootManager.WriteAll();
  } 
}

//_____________________________________________________________________________
void A01MCApplication::ReadEvent(Int_t i) 
{
/// Read \em i -th event and print hits.
/// \param i The number of event to be read    

  fDriftChamberSD1->Register();
  fDriftChamberSD2->Register();
  fEmCalorimeterSD->Register();
  fHadCalorimeterSD->Register();
  fHodoscopeSD1->Register();
  fHodoscopeSD2->Register();
  RegisterStack();
  fRootManager.ReadEvent(i);
}  
  
//_____________________________________________________________________________
void A01MCApplication::ConstructGeometry()
{    
/// Construct geometry using detector contruction class.

  fVerbose.ConstructGeometry();
  fDetConstruction->ConstructGeometry();  
}

//_____________________________________________________________________________
void A01MCApplication::InitGeometry()
{    
/// Initialize geometry
  
  fVerbose.InitGeometry();
  
  // Set cuts in G3 equivalent to 1mm cut in G4
  if ( TString(gMC->GetName()) == "TGeant3TGeo" ) SetCuts();
 
  fDriftChamberSD1->Initialize();
  fDriftChamberSD2->Initialize();
  fEmCalorimeterSD->Initialize();
  fHadCalorimeterSD->Initialize();
  fHodoscopeSD1->Initialize();
  fHodoscopeSD2->Initialize();
}

//_____________________________________________________________________________
void A01MCApplication::AddParticles()
{    
/// Nothing to be done
  
  fVerbose.AddParticles();
}

//_____________________________________________________________________________
void A01MCApplication::AddIons()
{    
/// Nothing to be done
  
  fVerbose.AddIons();
}

//_____________________________________________________________________________
void A01MCApplication::GeneratePrimaries()
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.
  
  fVerbose.GeneratePrimaries();
  fPrimaryGenerator->GeneratePrimaries();
}

//_____________________________________________________________________________
void A01MCApplication::BeginEvent()
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
}

//_____________________________________________________________________________
void A01MCApplication::BeginPrimary()
{    
/// User actions at beginning of a primary track.

  fVerbose.BeginPrimary();
}

//_____________________________________________________________________________
void A01MCApplication::PreTrack()
{    
/// User actions at beginning of each track

  fVerbose.PreTrack();
}

//_____________________________________________________________________________
void A01MCApplication::Stepping()
{    
/// User actions at each step

  fVerbose.Stepping();
  fDriftChamberSD1->ProcessHits();
  fDriftChamberSD2->ProcessHits();
  fEmCalorimeterSD->ProcessHits();
  fHadCalorimeterSD->ProcessHits();
  fHodoscopeSD1->ProcessHits();
  fHodoscopeSD2->ProcessHits();
}

//_____________________________________________________________________________
void A01MCApplication::PostTrack()
{    
/// User actions after finishing each track

  fVerbose.PostTrack();
}

//_____________________________________________________________________________
void A01MCApplication::FinishPrimary()
{    
/// User actions after finishing each primary track

  fVerbose.FinishPrimary();
}

//_____________________________________________________________________________
void A01MCApplication::FinishEvent()
{    
/// User actions after finishing an event

  fVerbose.FinishEvent();

  // Geant3 + TGeo
  // (use TGeo functions for visualization)
  if ( TString(gMC->GetName()) == "TGeant3TGeo") {
  
     // Draw volume 
     gGeoManager->SetVisOption(0);	 
     gGeoManager->SetTopVisible();
     gGeoManager->GetTopVolume()->Draw();

     // Draw tracks (if filled)
     if ( gGeoManager->GetListOfTracks() &&
          gGeoManager->GetTrack(0) &&
        ((TVirtualGeoTrack*)gGeoManager->GetTrack(0))->HasPoints() ) {
       
       gGeoManager->DrawTracks("/*");  // this means all tracks
          // Drawing G3 tracks via TGeo is available only
	  // if geant3 is compile with -DCOLLECT_TRACK flag
	  // (to be activated in geant3/TGeant3/TGeant3gu.cxx)
    }	  
  }    
 
  if ( fWriteStack || fWriteHits ) {
    fRootManager.Fill();
  }  

  // Print info about primary particle
  TParticle* primary = fStack->GetParticle(0);
  cout << endl
       << ">>> Event " << gMC->CurrentEvent() 
       << " >>> Simulation truth : "
       << primary->GetPDG()->GetName() << " (" 
       << primary->Px()*1e03 << ", " 
       << primary->Py()*1e03 << ", " 
       << primary->Pz()*1e03 << ") MeV" << endl;

  // Call detectors
  fHodoscopeSD1->EndOfEvent();
  fHodoscopeSD2->EndOfEvent();
  fDriftChamberSD1->EndOfEvent();
  fDriftChamberSD2->EndOfEvent();
  fEmCalorimeterSD->EndOfEvent();
  fHadCalorimeterSD->EndOfEvent();

  fStack->Reset();
} 

//_____________________________________________________________________________
void  A01MCApplication::SetWriteHits(Bool_t writeHits)
{ 
/// (In)Activate writing hits on file and propagate this option
/// to all SDs
/// \param writeHits  The new value of the option

  fWriteHits = writeHits; 
  fHodoscopeSD1->SetWriteHits(writeHits);
  fHodoscopeSD2->SetWriteHits(writeHits);
  fDriftChamberSD1->SetWriteHits(writeHits);
  fDriftChamberSD2->SetWriteHits(writeHits);
  fEmCalorimeterSD->SetWriteHits(writeHits);
  fHadCalorimeterSD->SetWriteHits(writeHits);  
}

//_____________________________________________________________________________
void A01MCApplication::SetCuts()
{
/// Set cuts for e-, gamma equivalent to 1mm cut in G4.

  Int_t mediumId = gMC->MediumId("Air");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 990.e-09);
    gMC->Gstpar(mediumId, "BCUTE",  990.e-09);
    gMC->Gstpar(mediumId, "CUTELE", 990.e-09);
    gMC->Gstpar(mediumId, "DCUTE",  990.e-09);
  }
  
  mediumId = gMC->MediumId("G4_Galactic");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 990.e-09);
    gMC->Gstpar(mediumId, "BCUTE",  990.e-09);
    gMC->Gstpar(mediumId, "CUTELE", 990.e-09);
    gMC->Gstpar(mediumId, "DCUTE",  990.e-09);
  }

  mediumId = gMC->MediumId("Scintillator");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 2.40367-06);
    gMC->Gstpar(mediumId, "BCUTE",  356.639e-06);
    gMC->Gstpar(mediumId, "CUTELE", 356.639e-06);
    gMC->Gstpar(mediumId, "DCUTE", 356.639e-06);
  }

  mediumId = gMC->MediumId("ArgonGas");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 990.e-09);
    gMC->Gstpar(mediumId, "BCUTE",  990.e-09);
    gMC->Gstpar(mediumId, "CUTELE", 990.e-09);
    gMC->Gstpar(mediumId, "DCUTE", 990.e-09);
  }

  mediumId = gMC->MediumId("CsI");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 38.5665e-06);
    gMC->Gstpar(mediumId, "BCUTE",  689.033e-06);
    gMC->Gstpar(mediumId, "CUTELE", 689.033e-06);
    gMC->Gstpar(mediumId, "DCUTE", 689.033e-06);
  }

  mediumId = gMC->MediumId("Lead");
  if ( mediumId ) {
    gMC->Gstpar(mediumId, "CUTGAM", 101.843e-06);
    gMC->Gstpar(mediumId, "BCUTE",  1.36749e-03);
    gMC->Gstpar(mediumId, "CUTELE", 1.36749e-03);
    gMC->Gstpar(mediumId, "DCUTE", 1.36749e-03);
  }

}    

