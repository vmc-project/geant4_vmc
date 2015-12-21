//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RadiatorDescription.cxx
/// \brief Implementation of the TG4RadiatorDescription class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RadiatorDescription.h"

//_____________________________________________________________________________
TG4RadiatorDescription::TG4RadiatorDescription(const G4String& volumeName)
  : fVolumeName(volumeName),
    fFoilMaterialName(),
    fGasMaterialName(),
    fFoilThickness(0.),
    fGasThickness(0.),
    fFoilNumber(0)
{
/// Default constructor
}

//_____________________________________________________________________________
TG4RadiatorDescription::~TG4RadiatorDescription() 
{
/// Destructor
}
