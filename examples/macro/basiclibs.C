// $Id: run_g3.C,v 1.2 2002/08/29 10:12:26 ivana Exp $
//
// Macro for loading basic libraries used with both Geant3 and Geant4

void basiclibs()
{
  gSystem->Load("libMC");
  gSystem->Load("libPhysics");
  gSystem->Load("libEG"); 
  gSystem->Load("libEGPythia6");
  gSystem->Load("libPythia6");  
}  
