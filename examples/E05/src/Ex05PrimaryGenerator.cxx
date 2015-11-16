//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05PrimaryGenerator.cxx 
/// \brief Implementation of the Ex05PrimaryGenerator class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TVirtualMCApplication.h>
#include <TRandom.h>
#include <TPDGCode.h>
#include <TVector3.h>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>

#include "Ex05PrimaryGenerator.h"

/// \cond CLASSIMP
ClassImp(Ex05PrimaryGenerator)
/// \endcond

//_____________________________________________________________________________
Ex05PrimaryGenerator::Ex05PrimaryGenerator(TVirtualMCStack* stack) 
  : TObject(),
    fStack(stack),
    fNofPrimaries(1),
    fVertexPosition(),
    fVertexDirection(TVector3(0,0,1))
{
/// Standard constructor
/// \param stack  The VMC stack
}

//_____________________________________________________________________________
Ex05PrimaryGenerator::Ex05PrimaryGenerator(const Ex05PrimaryGenerator& origin,
                                           TVirtualMCStack* stack)
  : TObject(origin),
    fStack(stack),
    fNofPrimaries(origin.fNofPrimaries),
    fVertexPosition(origin.fVertexPosition),
    fVertexDirection(origin.fVertexDirection)
{
/// Copy constructor (for clonig on worker thread in MT mode).
/// \param origin    The source object (on master).
/// \param stack  The VMC stack
}

//_____________________________________________________________________________
Ex05PrimaryGenerator::Ex05PrimaryGenerator()
  : TObject(),
    fStack(0),
    fNofPrimaries(0),
    fVertexPosition(),
    fVertexDirection()
{    
/// Default constructor
}

//_____________________________________________________________________________
Ex05PrimaryGenerator::~Ex05PrimaryGenerator() 
{
/// Destructor  
}

//
// private methods
//

//_____________________________________________________________________________
void Ex05PrimaryGenerator::GenerateOnePrimary(const TVector3& origin)
{    
/// Add one primary particle (kElectron) to the user stack 
/// (derived from TVirtualMCStack).
/// \param origin  The track position
  
 // Track ID (filled by stack)
 Int_t ntr;
 
 // Option: to be tracked
 Int_t toBeDone = 1; 
 
 // PDG
 Int_t pdg  = kElectron;

 // Polarization
 Double_t polx = 0.; 
 Double_t poly = 0.; 
 Double_t polz = 0.; 

 // Position
 Double_t vx  = origin.X(); 
 Double_t vy  = origin.Y(); 
 Double_t vz =  origin.Z();
 Double_t tof = 0.;
 fVertexPosition = origin;

 // Energy (in GeV)
 Double_t kinEnergy = 50.;  
 Double_t mass = 0.51099906*1e-03; 
 Double_t e  = mass + kinEnergy;
 Double_t pmag = sqrt(e*e - mass*mass); 
 
 // Particle momentum
 Double_t px, py, pz;
 px = pmag*fVertexDirection.X(); 
 py = pmag*fVertexDirection.Y();  
 pz = pmag*fVertexDirection.Z(); 

 // Randomize position
 // if (fIsRandom) {
 //   vy = origin.Y()*(gRandom->Rndm() - 0.5);
 //   vz = origin.Z()*(gRandom->Rndm() - 0.5);
 // }  

 // Add particle to stack 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                  kPPrimary, ntr, 1., 0);
}


//
// public methods
//

//_____________________________________________________________________________
void Ex05PrimaryGenerator::GeneratePrimaries(const TVector3& origin)
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  for (Int_t i=0; i<fNofPrimaries; i++) GenerateOnePrimary(origin);
}
