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
#include "TG4Globals.h"

//_____________________________________________________________________________
TG4RadiatorDescription::TG4RadiatorDescription(const G4String& volumeName)
  : fVolumeName(volumeName),
    fXtrModel(""),
    fFoilNumber(0),
    fLayers(),
    fStrawTube{"", 0., 0.}
{
/// Default constructor
}

//_____________________________________________________________________________
TG4RadiatorDescription::~TG4RadiatorDescription()
{
/// Destructor
}

//_____________________________________________________________________________
void TG4RadiatorDescription::SetLayer(const G4String& materialName,
	                              G4double thickness, G4double fluctuation)
{
  fLayers.push_back(std::make_tuple(materialName, thickness, fluctuation));
}

//_____________________________________________________________________________
void TG4RadiatorDescription::SetStrawTube(const G4String& materialName,
	                            G4double wallThickness, G4double gasThickness)
{
  fStrawTube = std::make_tuple(materialName, wallThickness, gasThickness);
}

//_____________________________________________________________________________
TG4RadiatorDescription::Component TG4RadiatorDescription::GetLayer(G4int i) const
{
  if ( i >= G4int(fLayers.size()) ) {
    TString text = "The layer ";
    text += i;
    text += " is not defined.";
    TG4Globals::Warning("TG4RadiatorDescription", "GetLayer", text);
    return std::make_tuple("", 0., 0.);
  }
  return fLayers[i];
}
