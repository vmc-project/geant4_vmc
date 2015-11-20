#ifndef GFLASH_FAST_SIMULATION_H
#define GFLASH_FAST_SIMULATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Gflash/geant4/include/FastSimulation.h
/// \brief Definition of the Gflash::FastSimulation class
///
/// Geant4 gflash example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VUserFastSimulation.h"

namespace Gflash
{

/// \ingroup Gflash
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
};

}

#endif //GFLASH_FAST_SIMULATION_H

