// $Id: $
//
// Geant4 ExampleN03 adapted to Virtual Monte Carlo 
//
// Id: ExN03PrimaryGeneratorAction.cc,v 1.6 2002/01/09 17:24:13 ranjard Exp 
// GEANT4 tag Name: geant4-05-00 
//
// by Ivana Hrivnacova, 6.3.2003

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TVirtualMCApplication.h>
#include <TRandom.h>
#include <TPDGCode.h>
#include <TVector3.h>

#include "Ex03PrimaryGenerator.h"

ClassImp(Ex03PrimaryGenerator)

//_____________________________________________________________________________
Ex03PrimaryGenerator::Ex03PrimaryGenerator(TVirtualMCStack* stack) 
  : TObject(),
    fStack(stack),
    fIsRandom(false),
    fNofPrimaries(1)
{
// Standard constructor
// ---
}

//_____________________________________________________________________________
Ex03PrimaryGenerator::Ex03PrimaryGenerator()
  : TObject(),
    fStack(0),
    fIsRandom(false),
    fNofPrimaries(0)
{    
// Default constructor
// ---
}

//_____________________________________________________________________________
Ex03PrimaryGenerator::~Ex03PrimaryGenerator() 
{
// Destructor  
// ---
}

//
// private methods
//

//_____________________________________________________________________________
void Ex03PrimaryGenerator::GeneratePrimary(const TVector3& origin)
{    
// Add one primary particle to the user stack (derived from TVirtualMCStack).
// ---
  
 // Track ID (filled by stack)
 Int_t ntr;
 
 // Option: to be tracked
 Int_t toBeDone = 1; 
 
 // Particle type
 Int_t pdg  = kElectron;
 
 // Polarization
 Double_t polx = 0.; 
 Double_t poly = 0.; 
 Double_t polz = 0.; 

 // Position
 Double_t vx  = -0.5 * origin.X(); 
 Double_t vy  = 0.; 
 Double_t vz =  0.;
 Double_t tof = 0.;

 // Energy (in GeV)
 Double_t kinEnergy = 0.050;  
 Double_t mass = 0.51099906*1e-03; 
 Double_t e  = mass + kinEnergy;
 
 // Particle momentum
 Double_t px, py, pz;
 px = sqrt(e*e - mass*mass); 
 py = 0.; 
 pz = 0.; 
 
 // Randomize position
 if (fIsRandom) {
   vy = origin.Y()*(gRandom->Rndm() - 0.5);
   vz = origin.Z()*(gRandom->Rndm() - 0.5);
 }  

 // Add particle to stack 
 fStack->SetTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                  kPPrimary, ntr, 1., 0);
}

//
// public methods
//

//_____________________________________________________________________________
void Ex03PrimaryGenerator::GeneratePrimaries(const TVector3& origin)
{    
// Fill the user stack (derived from TVirtualMCStack) with primary particles.
// ---
  
  for (Int_t i=0; i<fNofPrimaries; i++) GeneratePrimary(origin);  
}

