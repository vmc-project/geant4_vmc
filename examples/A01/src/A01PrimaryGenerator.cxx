//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01PrimaryGenerator.cxx 
/// \brief Implementation of the A01PrimaryGenerator class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo \n
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TVirtualMCApplication.h>
#include <TRandom.h>
#include <TPDGCode.h>
#include <TVector3.h>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>
#include <TPDGCode.h>
#include <Riostream.h>

#include "A01PrimaryGenerator.h"

/// \cond CLASSIMP
ClassImp(A01PrimaryGenerator)
/// \endcond

//_____________________________________________________________________________
A01PrimaryGenerator::A01PrimaryGenerator(TVirtualMCStack* stack) 
  : TObject(),
    fStack(stack),
    fNofPrimaries(1),
    fDefaultParticle(kMuonPlus),  
    fMomentum(1.),           // 1 GeV
    fSigmaMomentum(50.e-03), // 50 MeV;
    fSigmaAngle(2.),         // 2 deg
    fRandomizePrimary(true)    
{
/// Standard constructor
/// \param stack  The VMC stack
}

//_____________________________________________________________________________
A01PrimaryGenerator::A01PrimaryGenerator(const A01PrimaryGenerator& origin,
                                         TVirtualMCStack* stack) 
  : TObject(origin),
    fStack(stack),
    fNofPrimaries(origin.fNofPrimaries),
    fDefaultParticle(origin.fDefaultParticle),  
    fMomentum(origin.fMomentum),           
    fSigmaMomentum(origin.fSigmaMomentum), 
    fSigmaAngle(origin.fSigmaAngle),
    fRandomizePrimary(origin.fRandomizePrimary)    
{
/// Copy constructor (for clonig on worker thread in MT mode).
/// \param origin    The source object (on master).
/// \param stack  The VMC stack
}

//_____________________________________________________________________________
A01PrimaryGenerator::A01PrimaryGenerator()
  : TObject(),
    fStack(0),
    fNofPrimaries(0),
    fDefaultParticle(kMuonPlus),  
    fMomentum(1.),           // 1 GeV
    fSigmaMomentum(50.e-03), // 50 MeV;
    fSigmaAngle(2.),         // 2 deg
    fRandomizePrimary(true)
{    
/// Default constructor
}

//_____________________________________________________________________________
A01PrimaryGenerator::~A01PrimaryGenerator() 
{
/// Destructor  
}

//
// public methods
//

//_____________________________________________________________________________
void A01PrimaryGenerator::GeneratePrimaries()
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.
/// All primaries in one event have the same properties.

  // Track ID (filled by stack)
  Int_t ntr;
 
  // Option: to be tracked
  Int_t toBeDone = 1; 
 
  Int_t pdg  = fDefaultParticle;  
  if(fRandomizePrimary) {
     //Int_t i = (Int_t)(2.*gMC->GetRandom()->Rndm());    
     //Int_t i = (Int_t)(2.*0.3);
     static Int_t counter = 0;
     Int_t i = (counter++)%5;
     switch(i)
     {
       case 0:
         pdg = kPositron;
         break;
       case 1:
         pdg = kMuonPlus;
         break;
       case 2:
         pdg = kPiPlus;
         break;
       case 3:
         pdg = kKPlus;
         break;
       case 4:
         pdg = kProton;
         break;
       default:
         pdg = kPositron;
         break;
     }
  }   

  // Polarization
  Double_t polx = 0.; 
  Double_t poly = 0.; 
  Double_t polz = 0.; 

  // Position
  Double_t vx  = 0;  
  Double_t vy  = 0.; 
  Double_t vz = -800.0; //- 8.*m;  
  Double_t tof = 0.;

  // Particle momentum
  TDatabasePDG* databasePDG = TDatabasePDG::Instance(); 
  TParticlePDG* particlePDG = databasePDG->GetParticle(pdg);
  Double_t mass = particlePDG->Mass();
  //Double_t pp = fMomentum + (gMC->GetRandom()->Rndm()-0.5)*fSigmaMomentum;
  Double_t pp = fMomentum + (0.3-0.5)*fSigmaMomentum;
  Double_t e = TMath::Sqrt(pp*pp+mass*mass); 
 
  //Double_t angle = (gMC->GetRandom()->Rndm()-0.5)*fSigmaAngle;
  Double_t angle = (0.3-0.5)*fSigmaAngle;
  Double_t px, py, pz;
  px = pp*TMath::Sin(angle*TMath::DegToRad()); 
  py = 0.; 
  pz = pp*TMath::Cos(angle*TMath::DegToRad()); 
 
  for (Int_t i=0; i<fNofPrimaries; i++) { 
    // Add particle to stack 
    fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                      kPPrimary, ntr, 1., 0);
  }
}
