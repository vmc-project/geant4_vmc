// $Id: basiclibs.C,v 1.2 2003/07/22 06:38:47 brun Exp $
//
// Macro for loading basic libraries used with both Geant3 and Geant4

void basiclibs()
{
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libEGPythia6");
  gSystem->Load("libPythia6");  
}  
