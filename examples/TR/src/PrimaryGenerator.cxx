//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/src/PrimaryGenerator.cxx 
/// \brief Implementation of the PrimaryGenerator class 
///
/// Geant4 TestEm10 adapted to Virtual Monte Carlo.
///
/// \date 18/12/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "PrimaryGenerator.h"

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TPDGCode.h>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>

/// \cond CLASSIMP
ClassImp(TR::PrimaryGenerator)
/// \endcond

namespace TR
{

//_____________________________________________________________________________
PrimaryGenerator::PrimaryGenerator(TVirtualMCStack* stack) 
  : TObject(),
    fStack(stack),
    fNofPrimaries(1)
    
{
/// Standard constructor
/// \param stack  The VMC stack
}

//_____________________________________________________________________________
PrimaryGenerator::PrimaryGenerator(const PrimaryGenerator& origin,
                                   TVirtualMCStack* stack)
  : TObject(origin),
    fStack(stack),
    fNofPrimaries(origin.fNofPrimaries)
{
/// Copy constructor (for clonig on worker thread in MT mode).
/// \param origin    The source object (on master).
/// \param stack  The VMC stack
}

//_____________________________________________________________________________
PrimaryGenerator::PrimaryGenerator()
  : TObject(),
    fStack(0),
    fNofPrimaries(0)
{    
/// Default constructor
}

//_____________________________________________________________________________
PrimaryGenerator::~PrimaryGenerator() 
{
/// Destructor  
}

//
// private methods
//

//_____________________________________________________________________________
void PrimaryGenerator::GenerateOnePrimary()
{    
/// Add one primary particle (kElectron) to the user stack 
/// (derived from TVirtualMCStack).
  
 // Track ID (filled by stack)
 Int_t ntr;
 
 // Option: to be tracked
 Int_t toBeDone = 1; 
 
 // PDG
 Int_t pdg  = kElectron;
 //Int_t pdg  = kRootino;

 // Polarization
 Double_t polx = 0.; 
 Double_t poly = 0.; 
 Double_t polz = 0.; 

 // Position
 Double_t vx  = 0.; 
 Double_t vy  = 0.; 
 Double_t vz =  0.;
 Double_t tof = 0.;

 // Energy (in GeV)
 Double_t kinEnergy = 2.;  
 Double_t mass = 0.51099906*1e-03; 
 Double_t e  = mass + kinEnergy;
 
 // Particle momentum
 Double_t px, py, pz;
 px = 0.; 
 py = 0.; 
 pz = sqrt(e*e - mass*mass); 

 // Add particle to stack 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                   kPPrimary, ntr, 1., 0);
}

//_____________________________________________________________________________
void PrimaryGenerator::GeneratePrimaries()
{    
/// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  for (Int_t i=0; i<fNofPrimaries; i++) {
    GenerateOnePrimary();
  }
}

}
