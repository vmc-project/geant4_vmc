//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file E03/src/Ex03dMCStack.cxx
/// \brief Implementation of the Ex03dMCStack class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \date 07/07/2026
/// \author Radoslaw Karabowicz; GSI

#include <Riostream.h>
#include <TError.h>
#include <TParticle.h>

#include "Ex03dMCStack.h"
#include "TMCRootManager.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(Ex03dMCStack)
  /// \endcond

  //_____________________________________________________________________________
  Ex03dMCStack::Ex03dMCStack(Int_t size)
{
  /// Standard constructor
  /// \param size  The stack size
}

//_____________________________________________________________________________
Ex03dMCStack::Ex03dMCStack()
{
  /// Default constructor
}

//_____________________________________________________________________________
Ex03dMCStack::~Ex03dMCStack()
{
  /// Destructor

  delete fParticles;
}

// private methods

// public methods
void Ex03dMCStack::Register()
{
  TMCRootManager::Instance()->Register("particles", fExParticles);
}

//_____________________________________________________________________________
void Ex03dMCStack::PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  Double_t px, Double_t py, Double_t pz, Double_t e, Double_t vx, Double_t vy,
  Double_t vz, Double_t tof, Double_t polx, Double_t poly, Double_t polz,
  TMCProcess mech, Int_t& ntr, Double_t weight, Int_t is)
{
  /// Create a new particle and push into stack;
  /// adds it to the particles array (fParticles) and if not done to the
  /// stack (fStack).
  /// Use TParticle::fMother[1] to store Track ID.
  /// \param toBeDone  1 if particles should go to tracking, 0 otherwise
  /// \param parent    number of the parent track, -1 if track is primary
  /// \param pdg       PDG encoding
  /// \param px        particle momentum - x component [GeV/c]
  /// \param py        particle momentum - y component [GeV/c]
  /// \param pz        particle momentum - z component [GeV/c]
  /// \param e         total energy [GeV]
  /// \param vx        position - x component [cm]
  /// \param vy        position - y component  [cm]
  /// \param vz        position - z component  [cm]
  /// \param tof       time of flight [s]
  /// \param polx      polarization - x component
  /// \param poly      polarization - y component
  /// \param polz      polarization - z component
  /// \param mech      creator process VMC code
  /// \param ntr       track number (is filled by the stack
  /// \param weight    particle weight
  /// \param is        generation status code

  const Int_t kFirstDaughter = -1;
  const Int_t kLastDaughter = -1;

  Int_t trackId = GetNtrack();
  fParticles->push_back(TParticle(pdg, is, parent, trackId, kFirstDaughter,
    kLastDaughter, px, py, pz, e, vx, vy, vz, tof));
  auto particle = &fParticles->back();

  particle->SetPolarisation(polx, poly, polz);
  particle->SetWeight(weight);
  particle->SetUniqueID(mech);

  fExParticles->push_back(Ex03dParticle(pdg, is, parent, trackId,
    kFirstDaughter, kLastDaughter, px, py, pz, e, vx, vy, vz, tof));
  auto exParticle = &fExParticles->back();

  exParticle->SetPolarisation(polx, poly, polz);
  exParticle->SetWeight(weight);

  if (parent < 0) fNPrimary++;

  if (toBeDone) fStack.push(particle);

  ntr = GetNtrack() - 1;
}

//_____________________________________________________________________________
TParticle* Ex03dMCStack::PopNextTrack(Int_t& itrack)
{
  /// Get next particle for tracking from the stack.
  /// \return       The popped particle object
  /// \param track  The index of the popped track

  itrack = -1;
  if (fStack.empty()) return 0;

  TParticle* particle = fStack.top();
  fStack.pop();

  if (!particle) return 0;

  fCurrentTrack = particle->GetSecondMother();
  itrack = fCurrentTrack;

  return particle;
}

//_____________________________________________________________________________
TParticle* Ex03dMCStack::PopPrimaryForTracking(Int_t i)
{
  /// Return \em i -th particle in fParticles.
  /// \return   The popped primary particle object
  /// \param i  The index of primary particle to be popped

  if (i < 0 || i >= fNPrimary)
    Fatal("GetPrimaryForTracking", "Index out of range");

  return &(*fParticles)[i];
}

//_____________________________________________________________________________
void Ex03dMCStack::Print(Option_t* /*option*/) const
{
  /// Print info for all particles.

  cout << "Ex03dMCStack Info  " << endl;
  cout << "Total number of particles:   " << GetNtrack() << endl;
  cout << "Number of primary particles: " << GetNprimary() << endl;

  for (Int_t i = 0; i < GetNtrack(); i++) GetParticle(i)->Print();
}

//_____________________________________________________________________________
void Ex03dMCStack::Reset()
{
  /// Delete contained particles, reset particles array and stack.

  fCurrentTrack = -1;
  fNPrimary = 0;
  fParticles->clear();
  fExParticles->clear();
}

//_____________________________________________________________________________
void Ex03dMCStack::SetCurrentTrack(Int_t track)
{
  /// Set the current track number to a given value.
  /// \param  track The current track number

  fCurrentTrack = track;
}

//_____________________________________________________________________________
Int_t Ex03dMCStack::GetNtrack() const
{
  /// \return  The total number of all tracks.
  return fParticles->size();
}

//_____________________________________________________________________________
Int_t Ex03dMCStack::GetNprimary() const
{
  /// \return  The total number of primary tracks.

  return fNPrimary;
}

//_____________________________________________________________________________
TParticle* Ex03dMCStack::GetCurrentTrack() const
{
  /// \return  The current track particle

  TParticle* current = GetParticle(fCurrentTrack);

  if (!current)
    Warning("GetCurrentTrack", "Current track not found in the stack");

  return current;
}

//_____________________________________________________________________________
Int_t Ex03dMCStack::GetCurrentTrackNumber() const
{
  /// \return  The current track number

  return fCurrentTrack;
}

//_____________________________________________________________________________
Int_t Ex03dMCStack::GetCurrentParentTrackNumber() const
{
  /// \return  The current track parent ID.

  TParticle* current = GetCurrentTrack();

  if (current)
    return current->GetFirstMother();
  else
    return -1;
}

//_____________________________________________________________________________
TParticle* Ex03dMCStack::GetParticle(Int_t id) const
{
  /// \return   The \em id -th particle in fParticles
  /// \param id The index of the particle to be returned

  if (id < 0 || id >= fParticles->size())
    Fatal("GetParticle", "Index out of range");

  return &(*fParticles)[id];
}
