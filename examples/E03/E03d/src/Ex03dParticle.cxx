//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03dParticle.cxx
/// \brief Implementation of the Ex03dParticle class
///
/// Simplified version of ROOT's TParticle to store in RNTuple
///
/// \date 07/07/2026
/// \author Radoslaw Karabowicz; GSI

#include "Ex03dParticle.h"

//_____________________________________________________________________________
Ex03dParticle::Ex03dParticle()
  : fPdgCode(0),
    fStatusCode(0),
    fWeight(0),
    fCalcMass(0),
    fPx(0),
    fPy(0),
    fPz(0),
    fE(0),
    fVx(0),
    fVy(0),
    fVz(0),
    fVt(0),
    fPolarTheta(0),
    fPolarPhi(0)
{
  fMother[0] = 0;
  fMother[1] = 0;
  fDaughter[0] = 0;
  fDaughter[1] = 0;
  fParticlePDG = 0;
}

//_____________________________________________________________________________
Ex03dParticle::Ex03dParticle(Int_t pdg, Int_t status, Int_t mother1,
  Int_t mother2, Int_t daughter1, Int_t daughter2, Double_t px, Double_t py,
  Double_t pz, Double_t etot, Double_t vx, Double_t vy, Double_t vz,
  Double_t time)
  : fPdgCode(pdg),
    fStatusCode(status),
    fWeight(1.),
    fPx(px),
    fPy(py),
    fPz(pz),
    fE(etot),
    fVx(vx),
    fVy(vy),
    fVz(vz),
    fVt(time)
{
  fMother[0] = mother1;
  fMother[1] = mother2;
  fDaughter[0] = daughter1;
  fDaughter[1] = daughter2;

  SetPolarisation(0, 0, 0);

  SetPdgCode(pdg);
}

//_____________________________________________________________________________
Ex03dParticle::Ex03dParticle(Int_t pdg, Int_t status, Int_t mother1,
  Int_t mother2, Int_t daughter1, Int_t daughter2, const TLorentzVector& p,
  const TLorentzVector& v)
  : fPdgCode(pdg),
    fStatusCode(status),
    fWeight(1.),
    fPx(p.Px()),
    fPy(p.Py()),
    fPz(p.Pz()),
    fE(p.E()),
    fVx(v.X()),
    fVy(v.Y()),
    fVz(v.Z()),
    fVt(v.T())
{
  fMother[0] = mother1;
  fMother[1] = mother2;
  fDaughter[0] = daughter1;
  fDaughter[1] = daughter2;

  SetPolarisation(0, 0, 0);

  SetPdgCode(pdg);
}

//_____________________________________________________________________________
Ex03dParticle::Ex03dParticle(const Ex03dParticle& p)
  : fPdgCode(p.fPdgCode),
    fStatusCode(p.fStatusCode),
    fWeight(p.fWeight),
    fCalcMass(p.fCalcMass),
    fPx(p.fPx),
    fPy(p.fPy),
    fPz(p.fPz),
    fE(p.fE),
    fVx(p.fVx),
    fVy(p.fVy),
    fVz(p.fVz),
    fVt(p.fVt),
    fPolarTheta(p.fPolarTheta),
    fPolarPhi(p.fPolarPhi),
    fParticlePDG(p.fParticlePDG)
{
  fMother[0] = p.fMother[0];
  fMother[1] = p.fMother[1];
  fDaughter[0] = p.fDaughter[0];
  fDaughter[1] = p.fDaughter[1];
}

//_____________________________________________________________________________
Ex03dParticle& Ex03dParticle::operator=(const Ex03dParticle& p)
{
  if (this != &p) {
    fPdgCode = p.fPdgCode;
    fStatusCode = p.fStatusCode;
    fMother[0] = p.fMother[0];
    fMother[1] = p.fMother[1];
    fDaughter[0] = p.fDaughter[0];
    fDaughter[1] = p.fDaughter[1];
    fWeight = p.fWeight;

    fCalcMass = p.fCalcMass;

    fPx = p.fPx;
    fPy = p.fPy;
    fPz = p.fPz;
    fE = p.fE;

    fVx = p.fVx;
    fVy = p.fVy;
    fVz = p.fVz;
    fVt = p.fVt;

    fPolarTheta = p.fPolarTheta;
    fPolarPhi = p.fPolarPhi;

    fParticlePDG = p.fParticlePDG;
  }
  return *this;
}

//_____________________________________________________________________________
Ex03dParticle::~Ex03dParticle() {}

//_____________________________________________________________________________
Double_t Ex03dParticle::GetMass() const
{
  return 0.; // GetPDG()->Mass();
}

//_____________________________________________________________________________
Int_t Ex03dParticle::Beauty() const
{
  return 0; // GetPDG()->Beauty();
}

//_____________________________________________________________________________
Int_t Ex03dParticle::Charm() const
{
  return 0; // GetPDG()->Charm();
}

//_____________________________________________________________________________
Int_t Ex03dParticle::Strangeness() const
{
  return 0; // GetPDG()->Strangeness();
}

//_____________________________________________________________________________
void Ex03dParticle::GetPolarisation(TVector3& v) const
{
  if (fPolarTheta == -99 && fPolarPhi == -99)
    // No polarisation to return
    v.SetXYZ(0., 0., 0.);
  else
    v.SetXYZ(TMath::Cos(fPolarPhi) * TMath::Sin(fPolarTheta),
      TMath::Sin(fPolarPhi) * TMath::Sin(fPolarTheta), TMath::Cos(fPolarTheta));
}

//_____________________________________________________________________________
void Ex03dParticle::SetPdgCode(Int_t pdg)
{
  // fParticlePDG = pdg;
  /*    fParticlePDG = TDatabasePDG::Instance()->GetParticle(pdg);
 if (fParticlePDG) {
    fCalcMass    = fParticlePDG->Mass();
 } else {
    if (nWarnings < 10) {
       Warning("SetPdgCode","PDG code %d unknown from TDatabasePDG",pdg);
       nWarnings++;
    }
    Double_t a2 = fE*fE -fPx*fPx -fPy*fPy -fPz*fPz;
    if (a2 >= 0) fCalcMass =  TMath::Sqrt(a2);
    else         fCalcMass = -TMath::Sqrt(-a2);
    }*/
}

//_____________________________________________________________________________
void Ex03dParticle::SetPolarisation(Double_t polx, Double_t poly, Double_t polz)
{
  if (polx || poly || polz) {
    fPolarTheta =
      TMath::ACos(polz / TMath::Sqrt(polx * polx + poly * poly + polz * polz));
    fPolarPhi = TMath::Pi() + TMath::ATan2(-poly, -polx);
  }
  else {
    fPolarTheta = -99;
    fPolarPhi = -99;
  }
}
