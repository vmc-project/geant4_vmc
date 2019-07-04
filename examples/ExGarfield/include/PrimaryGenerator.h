#ifndef GARFIELD_PRIMARY_GENERATOR_H
#define GARFIELD_PRIMARY_GENERATOR_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file ExGarfield/include/PrimaryGenerator.h
/// \brief Definition of the ExGarfield::PrimaryGenerator class
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVector3.h>
#include <TVirtualMCApplication.h>

class TVirtualMCStack;

namespace VMC
{
namespace ExGarfield
{

class DetectorConstruction;

/// \ingroup ExGarfield
/// \brief The primary generator
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

class PrimaryGenerator : public TObject
{
 public:
  PrimaryGenerator(TVirtualMCStack* stack);
  PrimaryGenerator(const PrimaryGenerator& origin, TVirtualMCStack* stack);
  PrimaryGenerator();
  virtual ~PrimaryGenerator();

  // methods
  virtual void GeneratePrimaries(const TVector3& worldSize);

  // set methods
  void SetNofPrimaries(Int_t nofPrimaries);
  void SetVertexPosition(TVector3 position);
  void SetVertexDirection(TVector3 direction);

  // get methods
  TVector3 GetVertexPosition() const;
  TVector3 GetVertexDirection() const;

 private:
  // methods
  void GenerateOnePrimary(const TVector3& origin);

  // data members
  TVirtualMCStack* fStack;   ///< VMC stack
  Int_t fNofPrimaries;       ///< Number of primary particles
  TVector3 fVertexPosition;  ///< Vertex position
  TVector3 fVertexDirection; ///< Vertex direction

  ClassDef(PrimaryGenerator, 1) // PrimaryGenerator
};

// inline functions

/// Set the number of particles to be generated
/// \param nofPrimaries The number of particles to be generated
inline void PrimaryGenerator::SetNofPrimaries(Int_t nofPrimaries)
{
  fNofPrimaries = nofPrimaries;
}

/// Set the Vertex position
/// \param position The Vertex position
inline void PrimaryGenerator::SetVertexPosition(TVector3 position)
{
  fVertexPosition = position;
}

/// Set the Vertex direction
/// \param direction The Vertex direction
inline void PrimaryGenerator::SetVertexDirection(TVector3 direction)
{
  fVertexDirection = direction;
}

/// Return the Vertex position
inline TVector3 PrimaryGenerator::GetVertexPosition() const
{
  return fVertexPosition;
}

/// Return the Vertex direction
inline TVector3 PrimaryGenerator::GetVertexDirection() const
{
  return fVertexDirection;
}

} // namespace ExGarfield
} // namespace VMC

#endif // GARFIELD_PRIMARY_GENERATOR_H
