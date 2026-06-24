// @(#)root/eg:$Id$
// Author: Rene Brun , Federico Carminati  26/04/99

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

/** \class  Ex03Particle
    \ingroup eg

Description of the dynamic properties of a particle.

A dynamic particle class created by event generators and used during
the propagation in detectors. The static attributes of a Ex03Particle
are described by TParticlePDG.

\verbatim
 Int_t          fPdgCode;              // PDG code of the particle
 Int_t          fStatusCode;           // generation status code
 Int_t          fMother[2];            // Indices of the mother particles
 Int_t          fDaughter[2];          // Indices of the daughter particles
 Float_t        fWeight;               // particle weight

 Double_t       fCalcMass;             // Calculated mass

 Double_t       fPx;                   // x component of momentum
 Double_t       fPy;                   // y component of momentum
 Double_t       fPz;                   // z component of momentum
 Double_t       fE;                    // Energy

 Double_t       fVx;                   // x of production vertex
 Double_t       fVy;                   // y of production vertex
 Double_t       fVz;                   // z of production vertex
 Double_t       fVt;                   // t of production vertex

 Double_t       fPolarTheta;           // Polar angle of polarisation
 Double_t       fPolarPhi;             // azymutal angle of polarisation

 TParticlePDG*  fParticlePDG;          //! reference to the particle record in PDG database
\endverbatim
*/

#include "Ex03Particle.h"

////////////////////////////////////////////////////////////////////////////////
///default constructor

Ex03Particle::Ex03Particle() :
  fPdgCode(0), fStatusCode(0), fWeight(0),fCalcMass(0), fPx(0), fPy(0),
  fPz(0), fE(0), fVx(0), fVy(0), fVz(0), fVt(0), fPolarTheta(0), fPolarPhi(0)
{
   fMother[0]   = 0;
   fMother[1]   = 0;
   fDaughter[0] = 0;
   fDaughter[1] = 0;
   fParticlePDG = 0;
}

////////////////////////////////////////////////////////////////////////////////
///constructor

Ex03Particle::Ex03Particle(Int_t pdg,       Int_t status,
                     Int_t mother1,   Int_t mother2,
                     Int_t daughter1, Int_t daughter2,
                     Double_t px, Double_t py, Double_t pz, Double_t etot,
                     Double_t vx, Double_t vy, Double_t vz, Double_t time):
  fPdgCode(pdg), fStatusCode(status), fWeight(1.),fPx(px), fPy(py),
  fPz(pz), fE(etot), fVx(vx), fVy(vy), fVz(vz), fVt(time)
{
   fMother[0]   = mother1;
   fMother[1]   = mother2;
   fDaughter[0] = daughter1;
   fDaughter[1] = daughter2;

   SetPolarisation(0,0,0);

   SetPdgCode(pdg);
}

////////////////////////////////////////////////////////////////////////////////
///constructor

Ex03Particle::Ex03Particle(Int_t pdg,       Int_t status,
                     Int_t mother1,   Int_t mother2,
                     Int_t daughter1, Int_t daughter2,
                     const TLorentzVector &p,
                     const TLorentzVector &v) :
  fPdgCode(pdg), fStatusCode(status), fWeight(1.),fPx(p.Px()), fPy(p.Py()),
  fPz(p.Pz()), fE(p.E()), fVx(v.X()), fVy(v.Y()), fVz(v.Z()), fVt(v.T())
{
   fMother[0]   = mother1;
   fMother[1]   = mother2;
   fDaughter[0] = daughter1;
   fDaughter[1] = daughter2;

   SetPolarisation(0,0,0);

   SetPdgCode(pdg);
}

////////////////////////////////////////////////////////////////////////////////
/// copy constructor

Ex03Particle::Ex03Particle(const Ex03Particle &p) :
  fPdgCode(p.fPdgCode), fStatusCode(p.fStatusCode),
  fWeight(p.fWeight), fCalcMass(p.fCalcMass), fPx(p.fPx), fPy(p.fPy), fPz(p.fPz),
  fE(p.fE), fVx(p.fVx), fVy(p.fVy), fVz(p.fVz), fVt(p.fVt), fPolarTheta(p.fPolarTheta),
  fPolarPhi(p.fPolarPhi), fParticlePDG(p.fParticlePDG)
{
   fMother[0]=p.fMother[0];
   fMother[1]=p.fMother[1];
   fDaughter[0]=p.fDaughter[0];
   fDaughter[1]=p.fDaughter[1];
}

////////////////////////////////////////////////////////////////////////////////
/// Equal operator

Ex03Particle& Ex03Particle::operator=(const Ex03Particle &p)
{
   if(this!=&p) {
      fPdgCode=p.fPdgCode;
      fStatusCode=p.fStatusCode;
      fMother[0]=p.fMother[0];
      fMother[1]=p.fMother[1];
      fDaughter[0]=p.fDaughter[0];
      fDaughter[1]=p.fDaughter[1];
      fWeight=p.fWeight;

      fCalcMass=p.fCalcMass;

      fPx=p.fPx;
      fPy=p.fPy;
      fPz=p.fPz;
      fE=p.fE;

      fVx=p.fVx;
      fVy=p.fVy;
      fVz=p.fVz;
      fVt=p.fVt;

      fPolarTheta=p.fPolarTheta;
      fPolarPhi=p.fPolarPhi;

      fParticlePDG=p.fParticlePDG;
   }
   return   *this;
}

////////////////////////////////////////////////////////////////////////////////
///destructor

Ex03Particle::~Ex03Particle()
{
}

////////////////////////////////////////////////////////////////////////////////
/// Return nominal particle mass from PDG table.

Double_t Ex03Particle::GetMass() const
{
    return 0.;//GetPDG()->Mass();
}

////////////////////////////////////////////////////////////////////////////////
/// Return beauty quantum number.

Int_t Ex03Particle::Beauty() const
{
   return 0;//GetPDG()->Beauty();
}

////////////////////////////////////////////////////////////////////////////////
/// Return charm quantum number.

Int_t Ex03Particle::Charm() const
{
   return 0;//GetPDG()->Charm();
}

////////////////////////////////////////////////////////////////////////////////
/// Return strangeness quantum number.

Int_t Ex03Particle::Strangeness() const
{
   return 0;// GetPDG()->Strangeness();
}

////////////////////////////////////////////////////////////////////////////////
/// Return particle polarisation

void Ex03Particle::GetPolarisation(TVector3 &v) const
{
   if(fPolarTheta == -99 && fPolarPhi == -99)
      //No polarisation to return
      v.SetXYZ(0.,0.,0.);
   else
      v.SetXYZ(TMath::Cos(fPolarPhi)*TMath::Sin(fPolarTheta),
               TMath::Sin(fPolarPhi)*TMath::Sin(fPolarTheta),
               TMath::Cos(fPolarTheta));
}

////////////////////////////////////////////////////////////////////////////////
/// Change the PDG code for this particle
///
/// Get a new pointer to a TParticlePDG from TDatabasePDG.
/// Recompute the mass.

void Ex03Particle::SetPdgCode(Int_t pdg)
{
    //fParticlePDG = pdg;
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

////////////////////////////////////////////////////////////////////////////////
/// Set particle polarisation

void Ex03Particle::SetPolarisation(Double_t polx, Double_t poly, Double_t polz)
{
   if(polx || poly || polz) {
      fPolarTheta = TMath::ACos(polz/TMath::Sqrt(polx*polx+poly*poly+polz*polz));
      fPolarPhi   = TMath::Pi()+TMath::ATan2(-poly,-polx);
   } else {
      fPolarTheta = -99;
      fPolarPhi = -99;
   }
}
