#ifndef TG4_SCORE_WEIGHT_CALCULATOR_H
#define TG4_SCORE_WEIGHT_CALCULATOR_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ScoreWeightCalculator.h
/// \brief Definition of the score weight calculator type
///
/// \author I. Hrivnacova; IPN, Orsay

#include <Rtypes.h>

#include <functional>

using TG4ScoreWeightCalculator = std::function<Double_t(Int_t pdg, Double_t ekin)>;

#endif // TG4_SCORE_WEIGHT_CALCULATOR_H
