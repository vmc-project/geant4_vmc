// $Id: g4Config.C,v 1.1 2003/03/17 14:56:50 brun Exp $
//
// Configuration macro for Geant4 VirtualMC for Example03

void Config()
{
  // Load library with a user physics list
  gSystem->Load("$G4INSTALL/lib/plists/Linux-g++/libPackaging");
  gSystem->Load("$G4INSTALL/lib/plists/Linux-g++/libLHEP_BERT");

  // Load library with a user run configuration
  gSystem->Load("libgeant4e03");

  // Run configuration with user physics list
  Ex03RunConfiguration* runConfiguration = new Ex03RunConfiguration();
  
  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;
  
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
