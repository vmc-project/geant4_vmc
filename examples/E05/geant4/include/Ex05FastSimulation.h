#ifndef EX05_FAST_SIMULATION_H
#define EX05_FAST_SIMULATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05FastSimulation.h 
/// \brief Definition of the Ex05FastSimulation class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VUserFastSimulation.h"

/// \ingroup E05
/// \brief Special class for definition of fast simulation models
///
/// \author I. Hrivnacova; IPN, Orsay

class Ex05FastSimulation : public TG4VUserFastSimulation
{
  public:
    Ex05FastSimulation();
    virtual ~Ex05FastSimulation();

    // methods
    virtual void Construct();
};

#endif //EX05_FAST_SIMULATION_H

