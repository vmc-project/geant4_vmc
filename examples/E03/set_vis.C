// $Id: set_vis.C,v 1.2 2005/05/17 13:44:26 brun Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Macro for setting visualization for Example03 

void set_vis()
{
  if ( TString(gMC->GetName()) == "TGeant3" ) {
    // Set drawing options
    gMC->Gsatt("*", "seen", 16);
    gMC->Gsatt("ABSO", "seen", 5);
    gMC->Gsatt("GAPX", "seen", 6);
  }    

  if ( TString(gMC->GetName()) == "TGeant3TGeo" ) {
    // Set drawing options
    TGeoVolume* vol;
    vol = gGeoManager->GetVolume("ABSO");
    if (vol) vol->SetLineColor(kMagenta);
    vol = gGeoManager->GetVolume("GAPX");
    if (vol) vol->SetLineColor(kYellow);
  }    

  if (TString(gMC->GetName()) == "TGeant4") {
    // Setting Geant4 visualization
   ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");
  }  
}  
