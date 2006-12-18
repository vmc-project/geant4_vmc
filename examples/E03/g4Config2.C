// $Id: g4Config2.C,v 1.3 2006/12/12 16:21:38 brun Exp $
//
// Configuration macro for Geant4 VirtualMC for Example03
// for geometry defined with Root and selected Geant4 native navigation
// and a user defined physics list in a user run configuration

void Config()
{
  // Load library with a user physics list
  gSystem->Load("libG4phys_builders");
  gSystem->Load("libG4phys_lists");

  // Load library with a user run configuration
  gSystem->Load("libgeant4e03");

  // Run configuration with user physics list
  Ex03RunConfiguration2* runConfiguration 
    = new Ex03RunConfiguration2("geomRootToGeant4");
  
  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;
  
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config2.in");
}
