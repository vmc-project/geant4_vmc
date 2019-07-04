#ifndef PRIMARY_GENERATOR_H
#define PRIMARY_GENERATOR_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/include/PrimaryGenerator.h
/// \brief Definition of the PrimaryGenerator class
///
/// Geant4 TestEm10 adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>

class TVirtualMCStack;

namespace VMC
{
namespace TR
{

/// \ingroup TR
/// \brief The primary generator
///
/// \date 18/12/2015
/// \author I. Hrivnacova; IPN, Orsay

class PrimaryGenerator : public TObject
{
 public:
  PrimaryGenerator(TVirtualMCStack* stack);
  PrimaryGenerator(const PrimaryGenerator& origin, TVirtualMCStack* stack);
  PrimaryGenerator();
  virtual ~PrimaryGenerator();

  // methods
  virtual void GeneratePrimaries();

  void SetNofPrimaries(Int_t nofPrimaries);

 private:
  // methods
  void GenerateOnePrimary();

  // data members
  TVirtualMCStack* fStack; ///< VMC stack
  Int_t fNofPrimaries;     ///< Number of primary particles

  ClassDef(PrimaryGenerator, 1) // PrimaryGenerator
};

// inline functions

/// Set the number of particles to be generated
/// \param nofPrimaries The number of particles to be generated
inline void PrimaryGenerator::SetNofPrimaries(Int_t nofPrimaries)
{
  fNofPrimaries = nofPrimaries;
}

} // namespace TR
} // namespace VMC

#endif // PRIMARY_GENERATOR_H
