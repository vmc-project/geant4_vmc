// $Id: basiclibs.C,v 1.4 2006/12/18 15:43:18 brun Exp $
//
// Macro for loading basic libraries used with both Geant3 and Geant4

void basiclibs()
{
  gSystem->Load("libRIO");
  gSystem->Load("libTree");
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libEG");
  gSystem->Load("libEGPythia6");
  gSystem->Load("libPythia6");  
  gSystem->Load("libPhysics");  
}  
