// $Id: Ex01MCApplication.cxx,v 1.7 2003/09/27 09:32:47 brun Exp $
//
// Geant4 ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex01MCApplication
// ----------------------- 
// Implementation of the TVirtualMCApplication
//
// by Ivana Hrivnacova, 5.4.2002

#include "Ex01MCApplication.h"
#include "Ex01MCStack.h"

#include <TROOT.h>
#include <Riostream.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TLorentzVector.h>
#include <TGeoManager.h>

ClassImp(Ex01MCApplication)

//_____________________________________________________________________________
Ex01MCApplication::Ex01MCApplication(const char *name, const char *title) 
  : TVirtualMCApplication(name,title),
    fStack(0),
    fImedAr(0),
    fImedAl(0),
    fImedPb(0)
{
//
// Standard constructor
//

  // create a user stack
  fStack = new Ex01MCStack(100);  
}

//_____________________________________________________________________________
Ex01MCApplication::Ex01MCApplication()
  : TVirtualMCApplication(),
    fStack(0),
    fImedAr(0),
    fImedAl(0),
    fImedPb(0)
{    
  //
  // Default constructor
  //
}

//_____________________________________________________________________________
Ex01MCApplication::~Ex01MCApplication() 
{
  //
  // Destructor  
  //
  
  delete fStack;
  delete gMC;
  gMC = 0;
}

//
// private
//

//_____________________________________________________________________________
void Ex01MCApplication::ConstructMaterialsOld()
{
  //
  // Materials
  //

  Double_t a;
  Double_t z;
  Double_t density;
  Double_t radl;
  Double_t absl;
  Float_t* ubuf = 0;

  a = 39.95;
  z = 18.;
  density = 1.782e-03;
  radl = 0.1;
  absl = 0.1;
  Int_t imatAr = 1;
  gMC->Material(imatAr, "ArgonGas", a, z, density, radl, absl, ubuf, 0);

  a = 26.98;
  z = 13.;
  density = 2.7;
  radl = 0.1;
  absl = 0.1;
  Int_t imatAl = 2;
  gMC->Material(imatAl, "Aluminium", a, z, density, radl, absl, ubuf, 0);
  
  a = 207.19;
  z = 82.;
  density = 11.35;
  radl = 0.1;
  absl = 0.1;
  Int_t imatLead = 3;  
  gMC->Material(imatLead, "Lead", a, z, density, radl, absl, ubuf, 0);  

  //
  // Tracking medias
  //

  Int_t ifield = 0;          // No magnetic field
  Double_t fieldm = 0.;      //
  Double_t epsil  = .001;    // Tracking precision, 
  Double_t stemax = -0.01;   // Maximum displacement for multiple scat 
  Double_t tmaxfd = -20.;    // Maximum angle due to field deflection 
  Double_t deemax = -.3;     // Maximum fractional energy loss, DLS 
  Double_t stmin  = -.8;
  fImedAr = 1;
  fImedAl = 2;
  fImedPb = 3;
  gMC->Medium(fImedAr, "ArgonGas",  imatAr, 0, ifield, fieldm, tmaxfd, stemax, deemax, epsil, stmin, ubuf, 0); 
  gMC->Medium(fImedAl, "Aluminium", imatAl, 0, ifield, fieldm, tmaxfd, stemax, deemax, epsil, stmin, ubuf, 0); 
  gMC->Medium(fImedPb, "Lead", imatLead, 0, ifield, fieldm, tmaxfd, stemax, deemax, epsil, stmin, ubuf, 0); 
}


//_____________________________________________________________________________
void Ex01MCApplication::ConstructMaterials()
{
  //
  // Materials
  //

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
  fImedAl = 2;
  fImedPb = 3;
  gGeoManager->Medium("ArgonGas", fImedAr, imatAr, 0, ifield, fieldm, tmaxfd, 
                      stemax, deemax, epsil, stmin); 
  gGeoManager->Medium("Aluminium", fImedAl, imatAl, 0, ifield, fieldm, tmaxfd,
                      stemax, deemax, epsil, stmin); 
  gGeoManager->Medium("Lead", fImedPb, imatLead, 0, ifield, fieldm, tmaxfd,
                      stemax, deemax, epsil, stmin); 
}

//_____________________________________________________________________________
void Ex01MCApplication::ConstructVolumesOld()
{

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along x axis

  Double_t expHall[3];
  expHall[0] = 300.;
  expHall[1] = 100.;
  expHall[2] = 100.;
  gMC->Gsvolu("EXPH","BOX", fImedAr, expHall, 3);
 
  //------------------------------ a tracker tube

  Double_t trackerTube[3];
  trackerTube[0] = 0.;
  trackerTube[1] = 60.;
  trackerTube[2] = 50.;
  gMC->Gsvolu("TRTU","TUBE", fImedAl, trackerTube, 3);

  Double_t posX = -100.;
  Double_t posY =  0.;
  Double_t posZ =  0.;
  gMC->Gspos("TRTU", 1 ,"EXPH", posX, posY, posZ, 0, "ONLY");
  
  //------------------------------ a calorimeter block

  Double_t calBox[3];
  calBox[0] = 100.;
  calBox[1] = 50.;
  calBox[2] = 50.;
  gMC->Gsvolu("CALB","BOX", fImedPb, calBox, 3);

  posX = 100.;
  posY = 0.;
  posZ = 0.;
  gMC->Gspos("CALB", 1 ,"EXPH", posX, posY, posZ, 0, "ONLY");

  //------------------------------ calorimeter layers
 
  Double_t layerBox[3];
  layerBox[0] = 1.;
  layerBox[1] = 40.;
  layerBox[2] = 40.;
  gMC->Gsvolu("LAYB","BOX", fImedAl, layerBox, 3);

  for (Int_t i=0; i<19; i++) {
    posX = (i-9) * 10.;
    posY = 0.;
    posZ = 0.;
    gMC->Gspos("LAYB", i ,"CALB", posX, posY, posZ, 0, "ONLY");
  }  
}

//_____________________________________________________________________________
void Ex01MCApplication::ConstructVolumes()
{

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along x axis

  Double_t* ubuf = 0;

  Double_t expHall[3];
  expHall[0] = 300.;
  expHall[1] = 100.;
  expHall[2] = 100.;
  gGeoManager->Volume("EXPH","BOX", fImedAr, expHall, 3);
 
  //------------------------------ a tracker tube

  Double_t trackerTube[3];
  trackerTube[0] = 0.;
  trackerTube[1] = 60.;
  trackerTube[2] = 50.;
  gGeoManager->Volume("TRTU","TUBE", fImedAl, trackerTube, 3);

  Double_t posX = -100.;
  Double_t posY =  0.;
  Double_t posZ =  0.;
  gGeoManager->Node("TRTU", 1 ,"EXPH", posX, posY, posZ, 0, kFALSE, ubuf);
  
  //------------------------------ a calorimeter block

  Double_t calBox[3];
  calBox[0] = 100.;
  calBox[1] = 50.;
  calBox[2] = 50.;
  gGeoManager->Volume("CALB","BOX", fImedPb, calBox, 3);

  posX = 100.;
  posY = 0.;
  posZ = 0.;
  gGeoManager->Node("CALB", 1 ,"EXPH", posX, posY, posZ, 0, kFALSE, ubuf);

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
    gGeoManager->Node("LAYB", i ,"CALB", posX, posY, posZ, 0, kFALSE, ubuf);
  }  
  
  // notify VMC about Root geometry
  gMC->SetRootGeometry();
}

//
// public
//

//_____________________________________________________________________________
void Ex01MCApplication::InitMC(const char* setup)
{    
  //
  // Initialize MC.
  //


  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
 
  gMC->SetStack(fStack);
  gMC->Init();
  gMC->BuildPhysics();  
}

//_____________________________________________________________________________
void Ex01MCApplication::RunMC(Int_t nofEvents)
{    
  //
  // MC run.
  //


  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishRun()
{    
  //
  // Finish MC run.
  //

 // UGLAST
}

//_____________________________________________________________________________
void Ex01MCApplication::ConstructGeometry()
{    
  //
  // Construct geometry using TVirtualMC functions.
  //
  
  if (gMC->IsRootGeometrySupported()) {
    cout << "Geometry will be defined via TGeo" << endl;
    ConstructMaterials();  
    ConstructVolumes(); 
  }
  else {   
    cout << "Geometry will be defined via VMC" << endl;
    ConstructMaterialsOld();  
    ConstructVolumesOld(); 
  }  
}

//_____________________________________________________________________________
void Ex01MCApplication::InitGeometry()
{    
  //
  // Initialize geometry
  //
  
  // Nothing needed in this example
}

//_____________________________________________________________________________
void Ex01MCApplication::GeneratePrimaries()
{    
  //
  // Fill the user stack (derived from TVirtualMCStack) with primary particles.
  //
  
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
  //
  // User actions at beginning of event
  //

  // nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::BeginPrimary()
{    
  //
  // User actions at beginning of a primary track
  //

  // nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::PreTrack()
{    
  //
  // User actions at beginning of each track
  //

  cout << endl;
  cout << "Starting new track" << endl;
}

//_____________________________________________________________________________
void Ex01MCApplication::Stepping()
{    
  //
  // User actions at each step
  //
  
  TLorentzVector position;
  gMC->TrackPosition(position);
  
  cout << "Track " 
       << position.X() << " " << position.Y() << " " << position.Z() 
       << "  in " <<  gMC->CurrentVolName() << endl;
}

//_____________________________________________________________________________
void Ex01MCApplication::PostTrack()
{    
  //
  // User actions after finishing of each track
  //

  // nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishPrimary()
{    
  //
  // User actions after finishing of a primary track
  //

  // nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishEvent()
{    
  //
  // User actions after finishing of an event
  //

  // nothing to be done this example
} 

//_____________________________________________________________________________
void Ex01MCApplication::Field(const Double_t* x, Double_t* b) const
{
  // 
  // No magnetic field.
  //
  
   b[0] = 0.;
   b[1] = 0.;
   b[2] = 0.;
}
