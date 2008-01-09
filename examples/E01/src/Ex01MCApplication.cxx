// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex01MCApplication.cxx
/// \brief Implementation of the Ex01MCApplication class 
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo \n
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex01MCApplication.h"
#include "Ex01MCStack.h"
#include "Ex01DetectorConstructionOld.h"

#include <TROOT.h>
#include <Riostream.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TLorentzVector.h>
#include <TArrayD.h>
#include <TGeoManager.h>
#include <TGeoMatrix.h>

/// \cond CLASSIMP
ClassImp(Ex01MCApplication)
/// \endcond

//_____________________________________________________________________________
Ex01MCApplication::Ex01MCApplication(const char *name, const char *title) 
  : TVirtualMCApplication(name,title),
    fStack(0),
    fImedAr(0),
    fImedAl(0),
    fImedPb(0),
    fOldGeometry(kFALSE)
{
/// Standard constructor
/// \param name   The MC application name 
/// \param title  The MC application description

  // create a user stack
  fStack = new Ex01MCStack(100);  
}

//_____________________________________________________________________________
Ex01MCApplication::Ex01MCApplication()
  : TVirtualMCApplication(),
    fStack(0),
    fImedAr(0),
    fImedAl(0),
    fImedPb(0),
    fOldGeometry(kFALSE)
{    
/// Default constructor
}

//_____________________________________________________________________________
Ex01MCApplication::~Ex01MCApplication() 
{
/// Destructor  

  delete fStack;
  delete gMC;
  gMC = 0;
}

//
// private methods
//

//_____________________________________________________________________________
void Ex01MCApplication::ConstructMaterials()
{
/// Construct materials using TGeo modeller

  // Create Root geometry manager 
  new TGeoManager("TGeo", "Root geometry manager");
   
  Double_t a;
  Double_t z;
  Double_t density;
  Double_t radl;
  Double_t absl;
  
  a = 39.95;
  z = 18.;
  density = 1.782e-03;
  radl = 0.1;
  absl = 0.1;
  Int_t imatAr = 1;
  gGeoManager->Material("ArgonGas", a, z, density, imatAr, radl, absl);

  a = 26.98;
  z = 13.;
  density = 2.7;
  radl = 0.1;
  absl = 0.1;
  Int_t imatAl = 2;
  gGeoManager->Material("Aluminium", a, z, density, imatAl, radl, absl);
  
  a = 207.19;
  z = 82.;
  density = 11.35;
  radl = 0.1;
  absl = 0.1;
  Int_t imatLead = 3;  
  gGeoManager->Material("Lead", a, z, density, imatLead, radl, absl);  

  //
  // Tracking medias
  //

  Int_t ifield = 0;         // No magnetic field 
  Double_t fieldm = 0.;     //
  Double_t epsil  = .001;    // Tracking precision, 
  Double_t stemax = -0.01;   // Maximum displacement for multiple scat 
  Double_t tmaxfd = -20.;    // Maximum angle due to field deflection 
  Double_t deemax = -.3;     // Maximum fractional energy loss, DLS 
  Double_t stmin  = -.8;
  fImedAr = 1;
  gGeoManager->Medium("ArgonGas", fImedAr, imatAr, 0, ifield, fieldm, tmaxfd, 
                      stemax, deemax, epsil, stmin); 
  fImedAl = 2;
  gGeoManager->Medium("Aluminium", fImedAl, imatAl, 0, ifield, fieldm, tmaxfd,
                      stemax, deemax, epsil, stmin); 
  fImedPb = 3;
  gGeoManager->Medium("Lead", fImedPb, imatLead, 0, ifield, fieldm, tmaxfd,
                      stemax, deemax, epsil, stmin); 
}

//_____________________________________________________________________________
void Ex01MCApplication::ConstructVolumes()
{
/// Contruct volumes using TGeo modeller

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along x axis

  Double_t* ubuf = 0;

  Double_t expHall[3];
  expHall[0] = 300.;
  expHall[1] = 100.;
  expHall[2] = 100.;
  TGeoVolume *top = gGeoManager->Volume("EXPH","BOX", fImedAr, expHall, 3);
  gGeoManager->SetTopVolume(top);
   
  //------------------------------ a tracker tube

  Double_t trackerTube[3];
  trackerTube[0] = 0.;
  trackerTube[1] = 60.;
  trackerTube[2] = 50.;
  gGeoManager->Volume("TRTU","TUBE", fImedAl, trackerTube, 3);

  Double_t posX = -100.;
  Double_t posY =  0.;
  Double_t posZ =  0.;
  gGeoManager->Node("TRTU", 1 ,"EXPH", posX, posY, posZ, 0, kTRUE, ubuf);
  
  //------------------------------ a calorimeter block

  Double_t calBox[3];
  calBox[0] = 100.;
  calBox[1] = 50.;
  calBox[2] = 50.;
  gGeoManager->Volume("CALB","BOX", fImedPb, calBox, 3);

  posX = 100.;
  posY = 0.;
  posZ = 0.;
  gGeoManager->Node("CALB", 1 ,"EXPH", posX, posY, posZ, 0, kTRUE, ubuf);

  //------------------------------ calorimeter layers
 
  Double_t layerBox[3];
  layerBox[0] = 1.;
  layerBox[1] = 40.;
  layerBox[2] = 40.;
  gGeoManager->Volume("LAYB","BOX", fImedAl, layerBox, 3);

  for (Int_t i=0; i<19; i++) {
    posX = (i-9) * 10.;
    posY = 0.;
    posZ = 0.;
    gGeoManager->Node("LAYB", i ,"CALB", posX, posY, posZ, 0, kTRUE, ubuf);
  }  

  // close geometry
  gGeoManager->CloseGeometry();
    
  // notify VMC about Root geometry
  gMC->SetRootGeometry();
}

//
// public methods
//

//_____________________________________________________________________________
void Ex01MCApplication::InitMC(const char* setup)
{    
/// Initialize MC.
/// The selection of the concrete MC is done in the macro.
/// \param setup The name of the configuration macro 

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
 
  gMC->SetStack(fStack);
  gMC->Init();
  gMC->BuildPhysics();  
}

//__________________________________________________________________________
void Ex01MCApplication::RunMC(Int_t nofEvents)
{    
/// Run MC.
/// \param nofEvents Number of events to be processed

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishRun()
{    
/// Finish MC run.
}

//_____________________________________________________________________________
void Ex01MCApplication::ConstructGeometry()
{    
/// Construct geometry using TGeo functions or
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
    ConstructMaterials();  
    ConstructVolumes(); 
  }
  else {   
    cout << "Geometry will be defined via VMC" << endl;
    Ex01DetectorConstructionOld detConstructionOld;
    detConstructionOld.ConstructMaterials();  
    detConstructionOld.ConstructVolumes(); 
  }  
}

//_____________________________________________________________________________
void Ex01MCApplication::InitGeometry()
{    
/// Initialize geometry.
  
  fImedAr = gMC->MediumId("ArgonGas");
  fImedAl = gMC->MediumId("Aluminium");
  fImedPb = gMC->MediumId("Lead");
}

//_____________________________________________________________________________
void Ex01MCApplication::GeneratePrimaries()
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.
  
 // Track ID (filled by stack)
 Int_t ntr;
 
 // Option: to be tracked
 Int_t toBeDone = 1; 
 
 // Geantino
 Int_t pdg  = 0;
 
 // Polarization
 Double_t polx = 0.; 
 Double_t poly = 0.; 
 Double_t polz = 0.; 

 // Position
 Double_t vx  = -200.; 
 Double_t vy  = 0.; 
 Double_t vz  = 0.; 
 Double_t tof = 0.;

 // Momentum 
 Double_t px, py, pz, e;
 px = 1.; 
 py = 0.; 
 pz = 0.; 
 e  = 1.;

 // Add particle to stack 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                  kPPrimary, ntr, 1., 0);

 // Change direction and add particle to stack 
 px = 1.; 
 py = 0.1; 
 pz = 0.; 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz,
                  kPPrimary, ntr, 1., 0);

 // Change direction and add particle to stack 
 px = 1.; 
 py = 0.; 
 pz = 0.1; 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz,
                  kPPrimary, ntr, 1., 0);
}

//_____________________________________________________________________________
void Ex01MCApplication::BeginEvent()
{    
/// User actions at beginning of event.
/// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::BeginPrimary()
{    
/// User actions at beginning of a primary track.
/// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::PreTrack()
{    
/// User actions at beginning of each track.
/// Print info message.

  cout << endl;
  cout << "Starting new track" << endl;
}

//_____________________________________________________________________________
void Ex01MCApplication::Stepping()
{    
/// User actions at each step.
/// Print track position, the current volume and current medium names.
  
  TLorentzVector position;
  gMC->TrackPosition(position);
  
  cout << "Track " 
       << position.X() << " " << position.Y() << " " << position.Z() 
       << "  in " <<  gMC->CurrentVolName() << "  ";
       
  if (gMC->CurrentMedium() == fImedAr) cout <<  "ArgonGas";      
  if (gMC->CurrentMedium() == fImedAl) cout <<  "Aluminium";	  
  if (gMC->CurrentMedium() == fImedPb) cout <<  "Lead";      

  cout << endl;
}

//_____________________________________________________________________________
void Ex01MCApplication::PostTrack()
{    
/// User actions after finishing of each track
/// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishPrimary()
{    
/// User actions after finishing of a primary track.
/// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishEvent()
{    
/// User actions after finishing of an event
/// Nothing to be done this example
} 

//_____________________________________________________________________________
void Ex01MCApplication::Field(const Double_t* /*x*/, Double_t* b) const
{
/// No magnetic field.
  
   b[0] = 0.;
   b[1] = 0.;
   b[2] = 0.;
}

//_____________________________________________________________________________
void Ex01MCApplication::TestVMCGeometryGetters()
{
/// Test (new) TVirtualMC functions:
/// GetTransform(), GetShape(), GetMaterial(), GetMedium() 

  // Get transformation of 10th layer
  //
  TString volPath = "/EXPH_1/CALB_1/LAYB_9";
  TGeoHMatrix matrix;
  Bool_t result = gMC->GetTransformation(volPath, matrix);
  if (result) {
    cout << "Transformation for " << volPath.Data() << ": " << endl;
    matrix.Print();
  }
  else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;
 
  volPath = "/EXPH_1/CALB_1/LAYB_100";
  result = gMC->GetTransformation(volPath, matrix);
  if (result) {
    cout << "Transformation for " << volPath.Data() << ": " << endl;
    matrix.Print();
  }
  else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;

  volPath = "/EXPH_1/CALB_1/LAYB_9";
  result = gMC->GetTransformation(volPath, matrix);
  if (result) {
    cout << "Transformation for " << volPath.Data() << ": " << endl;
    matrix.Print();
  }
  else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;


  // Get shape
  //
  volPath = "/EXPH_1/CALB_1/LAYB_9";
  TString shapeType;
  TArrayD par;
  result = gMC->GetShape(volPath, shapeType, par);
  if (result) {
    cout << "Shape for " << volPath.Data() << ": " << endl;
    cout << shapeType.Data() << "  parameters: ";
    for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
      cout << par.At(ipar) << ",  ";
    cout << endl;  
  }
  else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;

  // Get material
  //
  TString volName = "LAYB";
  TString matName;
  Int_t imat;
  Double_t a, z, density, radl, inter;
  TArrayD mpar;
  result = gMC->GetMaterial(volName, matName, imat, a, z, density,
		            radl, inter, mpar);
  if (result) {
    cout << "Material for " << volName.Data() << " volume: " << endl;
    cout << matName.Data() << "  " << imat 
         << "  Aeff = " << a 
	 << "  Zeff = " << z 
	 << "  density = " << density 
	 << "  radl = " << radl
	 << "  inter = " << inter 
	 << endl;
    if ( mpar.GetSize() > 0 ) {
      cout  << " User defined parameters: ";
      for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
        cout << mpar.At(ipar) << ",  ";
      cout << endl; 
    }   
  }
  else {
    cerr << "Volume " << volName.Data() << " not found" << endl;
  }
  cout << endl;

  // Get medium
  //
  TString medName;
  Int_t imed, nmat, isvol, ifield;
  Double_t fieldm, tmaxfd, stemax, deemax, epsil, stmin;
  result = gMC->GetMedium(volName,
                          medName, imed, nmat, isvol, ifield,
		          fieldm, tmaxfd, stemax, deemax, epsil, stmin, mpar);
  if (result) {
    cout << "Medium for " << volName.Data() << " volume: " << endl;
    cout << medName.Data() << "  " << imed 
         << "  nmat = " << nmat
	 << "  isvol = " << isvol 
	 << "  ifield = " << ifield 
	 << "  fieldm = " << fieldm
	 << "  tmaxfd = " << tmaxfd 
	 << "  stemax = " << stemax
	 << "  deemax = " << deemax
	 << "  epsil = " << epsil
	 << "  stmin = " << stmin
	 << endl;
    if ( mpar.GetSize() > 0 ) {
      cout  << " User defined parameters: ";
      for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
        cout << mpar.At(ipar) << ",  ";
      cout << endl; 
    }   
  }
  else {
    cerr << "Volume " << volName.Data() << " not found" << endl;
  }
  cout << endl;

  // Test getters non-existing position/volume
  //
  
  // Transformation
  volPath = "/EXPH_1/CALB_1/LAYB_100";
  result = gMC->GetTransformation(volPath, matrix);
  cout << "GetTransformation: Volume path " << volPath.Data(); 
  if (!result) 
    cout << " not found" << endl;
  else 
    cout << " found" << endl;
  
  // Shape
  result = gMC->GetShape(volPath, shapeType, par);
  cout << "GetShape: Volume path " << volPath.Data(); 
  if (!result) 
    cout << " not found" << endl;
  else 
    cout << " found" << endl;

  // Material
  volName = "XYZ";
  result = gMC->GetMaterial(volName, matName, imat, a, z, density,
		            radl, inter, mpar);
  cout << "GetMaterial: Volume name " << volName.Data(); 
  if (!result) 
    cout << " not found" << endl;
  else 
    cout << " found" << endl;

  // Medium
  result = gMC->GetMedium(volName,
                          medName, imed, nmat, isvol, ifield,
		          fieldm, tmaxfd, stemax, deemax, epsil, stmin, mpar);
  cout << "GetMedium: Volume name " << volName.Data(); 
  if (!result) 
    cout << " not found" << endl;
  else 
    cout << " found" << endl;
 }  

