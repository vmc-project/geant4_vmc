// $Id: basiclibs.C,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
//
// Macro for loading basic libraries used with both Geant3 and Geant4

void basiclibs()
{
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libPhysics");
  gSystem->Load("libEG"); 
  gSystem->Load("libEGPythia6");
  gSystem->Load("libPythia6");  
}  
