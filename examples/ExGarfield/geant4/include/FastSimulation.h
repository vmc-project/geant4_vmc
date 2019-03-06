#ifndef GARFIELD_FAST_SIMULATION_H
#define GARFIELD_FAST_SIMULATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file ExGarfield/geant4/include/FastSimulation.h
/// \brief Definition of the ExGarfield::FastSimulation class
///
/// ExGarfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VUserFastSimulation.h"

class GarfieldMessenger;

namespace VMC
{
namespace ExGarfield
{

/// \ingroup ExGarfield
/// \brief Special class for definition of fast simulation models
///
/// \author I. Hrivnacova; IPN, Orsay

class FastSimulation : public TG4VUserFastSimulation
{
  public:
    FastSimulation();
    virtual ~FastSimulation();

    // methods
    virtual void Construct();

  private:
  	GarfieldMessenger* fMessenger;
};

}
}

#endif //GFLASH_FAST_SIMULATION_H

