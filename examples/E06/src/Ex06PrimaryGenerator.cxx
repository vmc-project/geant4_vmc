//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex06PrimaryGenerator.cxx
/// \brief Implementation of the  class
///
/// Geant4 ExampleN06 adapted to Virtual Monte Carlo \n
/// Id: ExN06PrimaryGeneratorAction.cc,v 1.3 2004/04/02 11:54:29 maire Exp \n
/// GEANT4 tag Name: geant4-07-00-cand-01
///
/// \date 16/05/2005
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TPDGCode.h>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <TVector3.h>
#include <TMath.h>

#include "Ex06PrimaryGenerator.h"

/// \cond CLASSIMP
ClassImp(Ex06PrimaryGenerator)
/// \endcond

//_____________________________________________________________________________
Ex06PrimaryGenerator::Ex06PrimaryGenerator(TVirtualMCStack* stack)
  : TObject(),
    fStack(stack),
    fPdg(kPositron),
    fKinEnergy(500.e-06),
    fDirX(1.),
    fDirY(0.),
    fDirZ(0.),
    fPolAngle(0.),
    fNofPrimaries(1)
{
/// Standard constructor
/// \param stack  The VMC stack
}

//_____________________________________________________________________________
Ex06PrimaryGenerator::Ex06PrimaryGenerator(const Ex06PrimaryGenerator& origin,
                                           TVirtualMCStack* stack)
  : TObject(origin),
    fStack(stack),
    fPdg(origin.fPdg),
    fKinEnergy(origin.fKinEnergy),
    fDirX(origin.fDirX),
    fDirY(origin.fDirY),
    fDirZ(origin.fDirZ),
    fPolAngle(origin.fPolAngle),
    fNofPrimaries(origin.fNofPrimaries)
{
/// Copy constructor (for clonig on worker thread in MT mode).
/// \param origin  The source object (on master).
/// \param stack   The VMC stack
}

//_____________________________________________________________________________
Ex06PrimaryGenerator::Ex06PrimaryGenerator()
  : TObject(),
    fStack(0),
    fPdg(0),
    fKinEnergy(0.),
    fDirX(0.),
    fDirY(0.),
    fDirZ(0.),
    fPolAngle(0.),
    fNofPrimaries(0)
{
/// Default constructor
}

//_____________________________________________________________________________
Ex06PrimaryGenerator::~Ex06PrimaryGenerator()
{
/// Destructor
}

//
// private methods
//

#include <Riostream.h>
//_____________________________________________________________________________
void Ex06PrimaryGenerator::GeneratePrimary()
{
/// Add one primary particle to the user stack
/// (derived from TVirtualMCStack).

  // Track ID (filled by stack)
  Int_t ntr;

  // Option: to be tracked
  Int_t toBeDone = 1;

  // Particle type
  Int_t pdg  = fPdg;
  TParticlePDG* particlePDG = TDatabasePDG::Instance()->GetParticle(fPdg);

  // Position
  Double_t vx  = 0.;
  Double_t vy  = 0.;
  Double_t vz =  0.;
  Double_t tof = 0.;

  // Energy (in GeV)
  Double_t kinEnergy = fKinEnergy;
  Double_t mass = particlePDG->Mass();
  //Double_t mass = 0.51099906*1e-03;
  Double_t e  = mass + kinEnergy;

  // Particle momentum
  Double_t p0, px, py, pz;
  p0 = sqrt(e*e - mass*mass);
  px = p0 * fDirX;
  py = p0 * fDirY;
  pz = p0 * fDirZ;

  // Polarization
  TVector3 polar;
  if ( fPdg == 50000050 ) {
    TVector3 normal (1., 0., 0.);
    TVector3 kphoton = TVector3(fDirX, fDirY, fDirZ);
    TVector3 product = normal.Cross(kphoton);
    Double_t modul2  = product*product;

    TVector3 e_perpend (0., 0., 1.);
    if (modul2 > 0.) e_perpend = (1./sqrt(modul2))*product;
    TVector3 e_paralle = e_perpend.Cross(kphoton);

    polar =   TMath::Cos(fPolAngle*TMath::DegToRad())*e_paralle
            + TMath::Sin(fPolAngle*TMath::DegToRad())*e_perpend;
  }
  //else
  //  Warning("GeneratePrimary",
  //          "The primary particle is not an opticalphoton");

  // Add particle to stack
  fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof,
                   polar.X(), polar.Y(), polar.Z(),
                   kPPrimary, ntr, 1., 0);
}

//
// public methods
//

//_____________________________________________________________________________
void Ex06PrimaryGenerator::GeneratePrimaries()
{
/// Fill the user stack (derived from TVirtualMCStack) with primary particle

  for (Int_t i=0; i<fNofPrimaries; i++) GeneratePrimary();
}

//_____________________________________________________________________________
void Ex06PrimaryGenerator::SetDirection(
                              Double_t dirX, Double_t dirY, Double_t dirZ)
{
/// Set normalized direction
/// \param dirX  The new direction - x component
/// \param dirY  The new direction - y component
/// \param dirZ  The new direction - z component

  Double_t norm = TMath::Sqrt(dirX*dirX + dirY*dirY + dirZ*dirZ);

  fDirX = dirX/norm;
  fDirY = dirY/norm;
  fDirZ = dirZ/norm;
}


