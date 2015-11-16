#ifndef EX03_PRIMARY_GENERATOR_H
#define EX03_PRIMARY_GENERATOR_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05PrimaryGenerator.h 
/// \brief Definition of the Ex05PrimaryGenerator class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>
#include <TVector3.h> 

class TVirtualMCStack;

class Ex05DetectorConstruction;

/// \ingroup E05
/// \brief The primary generator
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

class Ex05PrimaryGenerator : public TObject
{
  public:
    Ex05PrimaryGenerator(TVirtualMCStack* stack); 
    Ex05PrimaryGenerator(const Ex05PrimaryGenerator& origin,
                         TVirtualMCStack* stack);
    Ex05PrimaryGenerator();
    virtual ~Ex05PrimaryGenerator();

    // methods
    virtual void GeneratePrimaries(const TVector3& worldSize);

    // set methods
    void  SetNofPrimaries(Int_t nofPrimaries);
    void  SetVertexPosition(TVector3 position);
    void  SetVertexDirection(TVector3 direction);

    // get methods
    TVector3 GetVertexPosition() const;
    TVector3 GetVertexDirection() const;
 
  private:
    // methods
    void GenerateOnePrimary(const TVector3& origin);

    // data members
    TVirtualMCStack*  fStack;   ///< VMC stack
    Int_t     fNofPrimaries;    ///< Number of primary particles
    TVector3  fVertexPosition;  ///< Vertex position
    TVector3  fVertexDirection; ///< Vertex direction

  ClassDef(Ex05PrimaryGenerator,1)  //Ex05PrimaryGenerator
};

// inline functions

/// Set the number of particles to be generated
/// \param nofPrimaries The number of particles to be generated
inline void  Ex05PrimaryGenerator::SetNofPrimaries(Int_t nofPrimaries)
{ fNofPrimaries = nofPrimaries; }

/// Set the Vertex position
/// \param position The Vertex position
inline void  Ex05PrimaryGenerator::SetVertexPosition(TVector3 position)
{ fVertexPosition = position; }

/// Set the Vertex direction
/// \param direction The Vertex direction
inline void  Ex05PrimaryGenerator::SetVertexDirection(TVector3 direction)
{ fVertexDirection = direction; }

/// Return the Vertex position
inline TVector3 Ex05PrimaryGenerator::GetVertexPosition() const
{ return fVertexPosition; }

/// Return the Vertex direction
inline TVector3 Ex05PrimaryGenerator::GetVertexDirection() const
{ return fVertexDirection; }

#endif //EX03_PRIMARY_GENERATOR_H

