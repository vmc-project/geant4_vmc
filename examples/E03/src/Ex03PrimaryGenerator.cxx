// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex03PrimaryGenerator.cxx 
/// \brief Implementation of the Ex03PrimaryGenerator class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
/// Id: ExN03PrimaryGeneratorAction.cc,v 1.6 2002/01/09 17:24:13 ranjard Exp \n
/// GEANT4 tag Name: geant4-05-00 
///
/// \date 06/03/2002
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TVirtualMCApplication.h>
#include <TRandom.h>
#include <TPDGCode.h>
#include <TVector3.h>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>

#include "Ex03PrimaryGenerator.h"

/// \cond CLASSIMP
ClassImp(Ex03PrimaryGenerator)
/// \endcond

//_____________________________________________________________________________
Ex03PrimaryGenerator::Ex03PrimaryGenerator(TVirtualMCStack* stack) 
  : TObject(),
    fStack(stack),
    fIsRandom(false),
    fUserParticles(false),
    fNofPrimaries(1)
    
{
/// Standard constructor
/// \param stack  The VMC stack
}

//_____________________________________________________________________________
Ex03PrimaryGenerator::Ex03PrimaryGenerator()
  : TObject(),
    fStack(0),
    fIsRandom(false),
    fNofPrimaries(0)
{    
/// Default constructor
}

//_____________________________________________________________________________
Ex03PrimaryGenerator::~Ex03PrimaryGenerator() 
{
/// Destructor  
}

//
// private methods
//

//_____________________________________________________________________________
void Ex03PrimaryGenerator::GeneratePrimary1(const TVector3& origin)
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
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                  kPPrimary, ntr, 1., 0);
}

//_____________________________________________________________________________
void Ex03PrimaryGenerator::GeneratePrimary2(const TVector3& origin)
{    
/// Add user defined particle and ion as primaries to the user stack 
/// (derived from TVirtualMCStack).
/// \param origin  The track position
  
 // Track ID (filled by stack)
 Int_t ntr;
 
 // Option: to be tracked
 Int_t toBeDone = 1; 
 
 // User defined particle
 TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle("He5");
 Int_t pdg = particle->PdgCode ();
 
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
 Double_t mass = particle->Mass(); 
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
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                  kPPrimary, ntr, 1., 0);

 // User defined ion
 particle = TDatabasePDG::Instance()->GetParticle("MyIon");
 pdg = particle->PdgCode ();
  
 // Energy (in GeV)
 kinEnergy = 1.050;  
 mass = particle->Mass(); 
 e  = mass + kinEnergy;

 px = sqrt(e*e - mass*mass); 
 py = 0.; 
 pz = 0.; 

 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                  kPPrimary, ntr, 1., 0);
}

//_____________________________________________________________________________
void Ex03PrimaryGenerator::GeneratePrimary3(const TVector3& origin)
{    
/// Add one primary particle (kK0Short) to the user stack 
/// (derived from TVirtualMCStack).
/// \param origin  The track position
  
 // Track ID (filled by stack)
 Int_t ntr;
 
 // Option: to be tracked
 Int_t toBeDone = 1; 
 
 // PDG
 Int_t pdg  = kK0Short;  // 310

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
 Double_t mass = 0.497614; 
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
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                  kPPrimary, ntr, 1., 0);
}


//
// public methods
//

//_____________________________________________________________________________
void Ex03PrimaryGenerator::GeneratePrimaries(const TVector3& origin)
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  if ( fUserParticles ) {
    for (Int_t i=0; i<fNofPrimaries; i++) GeneratePrimary2(origin);
    return;
  }
  
  if ( fUserDecay ) {
    for (Int_t i=0; i<fNofPrimaries; i++) GeneratePrimary3(origin);
    return;
  }
  
  for (Int_t i=0; i<fNofPrimaries/2; i++) GeneratePrimary1(origin);
}

//_____________________________________________________________________________
void  Ex03PrimaryGenerator::SetUserParticles(Bool_t userParticles)
{ 
/// Switch on/off the user particles
/// \param userParticles  If true, the user defined particles are shooted

  fUserParticles = userParticles; 
  fUserDecay = ! userParticles;
}

//_____________________________________________________________________________
void  Ex03PrimaryGenerator::SetUserDecay(Bool_t userDecay)
{ 
/// Switch on/off the particle with a user decay
/// \param userDecay  If true, the particles with user defined decay are shooted

  fUserDecay = userDecay; 
  fUserParticles = ! userDecay; 
  
}

