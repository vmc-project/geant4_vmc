//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file E03/set_g3_vis.C
/// \brief Macro for setting visualization for Example03

#include "TVirtualMC.h"

void set_g3_vis()
{
/// Macro for setting G3 visualization for Example03

  // Set drawing options
  if ( TString(gMC->GetName()) == "TGeant3TGeo" ) {
    TGeoVolume* vol;
    vol = gGeoManager->GetVolume("ABSO");
    if (vol) vol->SetLineColor(kMagenta);
    vol = gGeoManager->GetVolume("GAPX");
    if (vol) vol->SetLineColor(kYellow);
    gMC->SetCollectTracks(kTRUE);
  }
}
