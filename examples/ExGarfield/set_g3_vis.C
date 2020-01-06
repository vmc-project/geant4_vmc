//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file ExGarfield/set_g3_vis.C
/// \brief Macro for setting G3 visualization for ExGarfield example

#include "TVirtualMC.h"

void set_g3_vis()
{
/// Macro for setting G3 visualization for ExGarfield example

  if ( TString(gMC->GetName()) == "TGeant3TGeo" ) {
    // Set drawing options
    TGeoVolume* vol;
    vol = gGeoManager->GetVolume("Crystal_log");
    if (vol) vol->SetLineColor(kYellow);
    gMC->SetCollectTracks(kTRUE);
  }
}
