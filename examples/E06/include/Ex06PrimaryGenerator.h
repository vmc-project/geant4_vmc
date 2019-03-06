#ifndef EX06_PRIMARY_GENERATOR_H
#define EX06_PRIMARY_GENERATOR_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex06PrimaryGenerator.h
/// \brief Definition of the Ex06PrimaryGenerator class
///
/// Geant4 ExampleN06 adapted to Virtual Monte Carlo \n
/// Id: ExN06PrimaryGeneratorAction.hh,v 1.4 2003/01/23 15:34:23 maire Exp \n
/// GEANT4 tag Name: geant4-07-00-cand-01
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>

class TVirtualMCStack;
class TVector3;

/// \ingroup E06
/// \brief The primary generator
///
/// \date 16/05/2005
/// \author I. Hrivnacova; IPN, Orsay

class Ex06PrimaryGenerator : public TObject
{
  public:
    Ex06PrimaryGenerator(TVirtualMCStack* stack);
    Ex06PrimaryGenerator(const Ex06PrimaryGenerator& origin,
                         TVirtualMCStack* stack);
    Ex06PrimaryGenerator();
    virtual ~Ex06PrimaryGenerator();

    // methods
    void GeneratePrimaries();

    // set methods
    void SetParticle(Int_t pdg);
    void SetKinEnergy(Double_t kinEnergy);
    void SetDirection(Double_t dirX, Double_t dirY, Double_t dirZ);
    void SetOptPhotonPolar(Double_t angle);
    void SetNofPrimaries(Int_t nofPrimaries);

  private:
    // methods
    void GeneratePrimary();

    // data members
    TVirtualMCStack*  fStack;        ///< VMC stack
    Int_t             fPdg;          ///< Particle PDG encoding
    Double_t          fKinEnergy;    ///< Particle kinetic energy
    Double_t          fDirX;         ///< Particle direction - x component
    Double_t          fDirY;         ///< Particle direction - y component
    Double_t          fDirZ;         ///< Particle direction - z component
    Double_t          fPolAngle;     ///< Particle polarization angle
    Int_t             fNofPrimaries; ///< Number of primary particles

  ClassDef(Ex06PrimaryGenerator,1)  //Ex06PrimaryGenerator
};

// inline functions

/// Set particle type
/// \param pdg  The new particle PDG encoding
inline void Ex06PrimaryGenerator::SetParticle(Int_t pdg)
{ fPdg = pdg; }

/// Set kinetic energy
/// \param kinEnergy  The new particle kinetic energy (in GeV)
inline void Ex06PrimaryGenerator::SetKinEnergy(Double_t kinEnergy)
{ fKinEnergy = kinEnergy; }

/// Set photon polarization
/// \param angle  The new polarization angle (in degrees)
inline void Ex06PrimaryGenerator::SetOptPhotonPolar(Double_t angle)
{ fPolAngle = angle; }

/// Set the number of particles to be generated
/// \param nofPrimaries The number of particles to be generated
inline void Ex06PrimaryGenerator::SetNofPrimaries(Int_t nofPrimaries)
{ fNofPrimaries = nofPrimaries; }

#endif //EX06_PRIMARY_GENERATOR_H

